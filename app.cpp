#include "app.hpp"

#include <cstdlib>
#include <fstream>
#include <memory>
#include <future>

#include <json/json.h>
#include <png++/png.hpp>

#include "openclcommandqueue.hpp"
#include "openclcontext.hpp"
#include "opencldevice.hpp"
#include "openclkernel.hpp"
#include "openclmem.hpp"
#include "openclplatform.hpp"
#include "openclprogram.hpp"
#include "pngimagewriter.hpp"

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
    Json::Value project_config;
    {
        std::ifstream ifs("project.json");
        ifs >> project_config;
    }

    Json::Value device_config;
    {
        std::ifstream ifs("device.json");
        ifs >> device_config;
    }

    const std::string program_name = project_config["program"].asString();
    const std::string kernel_name = project_config["kernel"].asString();
    const int image_count = project_config["image_count"].asInt();
    const std::string output = project_config["output"].asString();
    const int width = project_config["width"].asInt();
    const int height = project_config["height"].asInt();
    const int tile_size = project_config["tile_size"].asInt();
    const int samples = project_config["samples"].asInt();

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

    auto platform = std::make_shared<OpenCLPlatform>(device_config["platform"].asString());
    auto device = std::make_shared<OpenCLDevice>(platform, device_config["device"].asString());
    auto context = std::make_shared<OpenCLContext>(device);
    std::shared_ptr<OpenCLProgram> program;
    {
        std::ifstream ifs(program_name);
		program = std::make_shared<OpenCLProgram>(context, device, ifs);
    }
    auto command_queue = std::make_shared<OpenCLCommandQueue>(context, device);
    auto mem = std::make_shared<OpenCLMem>(context, command_queue, tile_size);
    auto kernel = std::make_shared<OpenCLKernel>(program, kernel_name, mem, width, height);

    png::image<png::rgb_pixel> image(width, height);

    cl_int err;

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

                err = clEnqueueNDRangeKernel(
                    command_queue->get(),
                    kernel->get(),
                    3,
                    global_work_offset,
                    global_work_size,
                    nullptr,
                    0,
                    nullptr,
                    nullptr);
                if (err != CL_SUCCESS)
                {
                    LOG_ERROR <<
                        "Error in clEnqueueNDRangeKernel. (" << err << ")" << std::endl;
                    throw std::exception();
                }
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
        std::sprintf(filename, output.c_str(), 1);
        LOG_INFO << "png++ image write (" << filename << ")." << std::endl;
        image.write(filename);
        delete[] filename;
    }
}

