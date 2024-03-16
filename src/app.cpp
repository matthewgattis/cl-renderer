#include "app.hpp"

#include <cstdlib>
#include <fstream>
#include <memory>
#include <future>
#include <thread>
#include <chrono>

#include <json/json.h>
#include <png++/png.hpp>
#include <argparse/argparse.hpp>

#include "openclcommandqueue.hpp"
#include "openclcontext.hpp"
#include "opencldevice.hpp"
#include "openclkernel.hpp"
#include "openclmem.hpp"
#include "openclplatform.hpp"
#include "openclprogram.hpp"

#define LOG_MODULE_NAME ("App")
#include "log.hpp"

App::App()
{
}

template <typename T>
inline float min(T a, T b)
{
    return a < b ? a : b;
}

inline float max(float a, float b)
{
    return a > b ? a : b;
}

template <typename T>
std::future<T> &wait_for_any(const std::vector<std::future<T>>& fs)
{
    while (true)
    {
        for (auto& f : fs)
        {
            if (f.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
                return &f;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void App::run(const std::vector<std::string> &args)
{
    argparse::ArgumentParser argument_parser("cl-renderer", "cl-renderer v0.1.0");

    argument_parser.add_argument("-k", "--kernel")
        .help("name of kernel entry point")
        .default_value(std::string("mainimage"));

    argument_parser.add_argument("-c", "--image-count")
        .help("number of image frames to generate")
        .default_value(1)
        .scan<'i', int>();

    argument_parser.add_argument("-o", "--output")
        .help("output file name(s)")
        .default_value(std::string("frame-%04d.png"));

    argument_parser.add_argument("-w", "--width")
        .help("image(s) width")
        .default_value(960)
        .scan<'i', int>();

    argument_parser.add_argument("-h", "--height")
        .help("image(s) height")
        .default_value(640)
        .scan<'i', int>();

    argument_parser.add_argument("-t", "--tile-size")
        .help("tile, or chunk, size to use when splitting up the image")
        .default_value(128)
        .scan<'i', int>();

    argument_parser.add_argument("-s", "--samples")
        .help("number of sample to run  for each pixel")
        .default_value(64)
        .scan<'i', int>();

    argument_parser.add_argument("-p", "--platform")
        .help("name OpenCL platform to use")
        .default_value(std::string(""));

    argument_parser.add_argument("-d", "--device")
        .help("name of OpenCL device to use")
        .default_value(std::string(""));

    argument_parser.add_argument("program")
        .help("file name of OpenCL program to load")
        .default_value(std::string("program.cl"));

    argument_parser.parse_args(args);
    
    const std::string program_name = argument_parser.get<std::string>("program");
    const std::string kernel_name = argument_parser.get<std::string>("-k");
    const int image_count = argument_parser.get<int>("-c");
    const std::string output = argument_parser.get<std::string>("-o");
    const int width = argument_parser.get<int>("-w");
    const int height = argument_parser.get<int>("-h");
    const int tile_size = argument_parser.get<int>("-t");
    const int samples = argument_parser.get<int>("-s");

    const int tile_x_count = width / tile_size + ((width % tile_size) > 0 ? 1 : 0);
    const int tile_y_count = height / tile_size + ((height % tile_size) > 0 ? 1 : 0);
    const int tiles_total = tile_x_count * tile_y_count;

    LOG_INFO << "program_name (" << program_name << ")" << std::endl;
    LOG_INFO << "kernel_name (" << kernel_name << ")" << std::endl;
    LOG_INFO << "image_count (" << image_count << ")" << std::endl;
    LOG_INFO << "output (" << output << ")" << std::endl;
    LOG_INFO << "width (" << width << ")" << std::endl;
    LOG_INFO << "height (" << height << ")" << std::endl;
    LOG_INFO << "tile_size (" << tile_size << ")" << std::endl;
    LOG_INFO << "samples (" << samples << ")" << std::endl;

    LOG_INFO << "tile_x_count (" << tile_x_count << ")" << std::endl;
    LOG_INFO << "tile_y_count (" << tile_y_count << ")" << std::endl;
    LOG_INFO << "tiles_total (" << tiles_total << ")" << std::endl;

    const std::string platform_name = argument_parser.get<std::string>("-p");
    const std::string device_name = argument_parser.get<std::string>("-d");

    auto platform = std::make_shared<OpenCLPlatform>(platform_name);
    auto device = std::make_shared<OpenCLDevice>(platform, device_name);
    auto context = std::make_shared<OpenCLContext>(device);
    std::shared_ptr<OpenCLProgram> program;
    {
        std::ifstream ifs(program_name);
		program = std::make_shared<OpenCLProgram>(context, device, ifs);
    }
    auto command_queue = std::make_shared<OpenCLCommandQueue>(context, device);
    auto mem = std::make_shared<OpenCLMem>(context, command_queue, tile_size);
    auto kernel = std::make_shared<OpenCLKernel>(program, kernel_name, mem, width, height);

    cl_int err;

    for (int i = 0; i < image_count; i++)
    {
        kernel->setFrameNumber(i);

        png::image<png::rgb_pixel> image(width, height);

        for (int x = 0; x < tile_x_count; x++)
        {
            size_t global_work_size[3];
            global_work_size[2] = 1;

            for (int y = 0; y < tile_y_count; y++)
            {
                global_work_size[0] = min<int>(width - x * tile_size, tile_size);
                global_work_size[1] = min<int>(height - y * tile_size, tile_size);

                size_t global_work_offset[3];
                global_work_offset[0] = x * tile_size;
                global_work_offset[1] = y * tile_size;

                LOG_INFO <<
                    "Enqueuing tile (" << x << " " << y << ") sample (" << samples << ")." << std::endl;

                for (int z = 0; z < samples; z++)
                {
                    global_work_offset[2] = z;

                    kernel->enqueueKernel(
                        command_queue,
                        global_work_offset,
                        global_work_size);
                }

                command_queue->finish();
                std::vector<float> tile = mem->pixels();

                for (int j = 0; j < (int)global_work_size[1]; j++)
                {
                    for (int i = 0; i < (int)global_work_size[0]; i++)
                    {
                        int k = x * tile_size + i;
                        int l = y * tile_size + j;
                        float r = min<float>(max(0.0f, tile[4 * (i + tile_size * j) + 0] * 255.0f), 255.0f);
                        float g = min<float>(max(0.0f, tile[4 * (i + tile_size * j) + 1] * 255.0f), 255.0f);
                        float b = min<float>(max(0.0f, tile[4 * (i + tile_size * j) + 2] * 255.0f), 255.0f);
                        image[height - l - 1][k] = png::rgb_pixel(r, g, b);
                    }
                }
            }
        }

        command_queue->finish();

        {
            char* filename = new char[32768];
            std::sprintf(filename, output.c_str(), i);
            LOG_INFO << "png++ image write (" << filename << ")." << std::endl;
            image.write(filename);
            delete[] filename;
        }
    }
}

