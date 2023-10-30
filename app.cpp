#include "app.hpp"

#include <fstream>
#include <memory>

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

inline float min(float a, float b)
{
    return a < b ? a : b;
}

inline float max(float a, float b)
{
    return a > b ? a : b;
}

void App::run(const std::vector<std::string> &args)
{
    const int width = 1366;
    const int height = 768;
    const int tile_size = 128;
    const int samples = 64;
    //const int samples = 1;

    int tile_x_count =
        width / tile_size + ((width % tile_size) > 0 ? 1 : 0);
    int tile_y_count =
        height / tile_size + ((height % tile_size) > 0 ? 1 : 0);

    LOG_INFO << "width (" << width << ")" << std::endl;
    LOG_INFO << "height (" << height << ")" << std::endl;
    LOG_INFO << "tile_size (" << tile_size << ")" << std::endl;
    LOG_INFO << "samples (" << samples << ")" << std::endl;
    LOG_INFO << "tile_x_count (" << tile_x_count << ")" << std::endl;
    LOG_INFO << "tile_y_count (" << tile_y_count << ")" << std::endl;

    auto platform = std::make_shared<OpenCLPlatform>("Intel(R) OpenCL Graphics");
    auto device = std::make_shared<OpenCLDevice>(platform, "Intel(R) Graphics [0x46a6]");
    auto context = std::make_shared<OpenCLContext>(device);
    std::ifstream ifs("program.cl");
    auto program = std::make_shared<OpenCLProgram>(context, device, ifs);
    auto mem = std::make_shared<OpenCLMem>(context, tile_size);
    auto kernel = std::make_shared<OpenCLKernel>(program, "mainimage", mem, width, height);
    auto command_queue = std::make_shared<OpenCLCommandQueue>(context, device);

    png::image<png::rgb_pixel> image(width, height);

    cl_int err;

    for (int x = 0; x < tile_x_count; x++)
    {
        size_t global_work_size[3];
        global_work_size[2] = 1;

        for (int y = 0; y < tile_y_count; y++)
        {
            global_work_size[0] = width - x * tile_size;
            if (global_work_size[0] > tile_size)
                global_work_size[0] = tile_size;

            global_work_size[1] = height - y * tile_size;
            if (global_work_size[1] > tile_size)
                global_work_size[1] = tile_size;

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
            std::vector<float> tile = mem->value(command_queue->get());

            int tile_width = width - x * tile_size;
            tile_width = tile_width > tile_size ? tile_size : tile_width;
            int tile_height = height - y * tile_size;
            tile_height = tile_height > tile_size ? tile_size : tile_height;

            for (int j = 0; j < tile_height; j++)
            {
                for (int i = 0; i < tile_width; i++)
                {
                    int k = x * tile_size + i;
                    int l = y * tile_size + j;
                    float r = min(max(0.0f, tile[4 * (i + tile_size * j) + 0] * 255.0f), 255.0f);
                    float g = min(max(0.0f, tile[4 * (i + tile_size * j) + 1] * 255.0f), 255.0f);
                    float b = min(max(0.0f, tile[4 * (i + tile_size * j) + 2] * 255.0f), 255.0f);
                    image[height - l - 1][k] = png::rgb_pixel(r, g, b);
                }
            }
        }
    }

    command_queue->finish();

    LOG_INFO << "png++ image write (" << "out.png" << ")." << std::endl;
    image.write("out.png");
}

