#include "app.hpp"

#include <boolinq/boolinq.h>
#include <fstream>
#include <memory>

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
    LOG_INFO << "Instance created." << std::endl;
}

void App::run(const std::vector<std::string> &args)
{
    const int width = 800;
    const int height = 600;
    const int tile_size = 128;
    const int samples = 1;

    auto platform = std::make_shared<OpenCLPlatform>("NVIDIA CUDA");
    auto device = std::make_shared<OpenCLDevice>(platform, "NVIDIA GeForce GTX 970");
    auto context = std::make_shared<OpenCLContext>(device);
    std::ifstream ifs("program.cl");
    auto program = std::make_shared<OpenCLProgram>(context, device, ifs);
    auto mem = std::make_shared<OpenCLMem>(context, tile_size);
    auto kernel = std::make_shared<OpenCLKernel>(program, "mainimage", mem, width, height);
    auto command_queue = std::make_shared<OpenCLCommandQueue>(context, device);

    int tile_x_count =
        width / tile_size + ((width % tile_size) > 0 ? 1 : 0);
    int tile_y_count =
        height / tile_size + ((height % tile_size) > 0 ? 1 : 0);

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

            for (int z = 0; z < samples; z++)
            {
                global_work_offset[2] = z;

                LOG_INFO <<
                    "Enqueuing tile (" << x << " " << y << ") sample (" << z << ")." << std::endl;

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
        }
    }

    err = clFinish(command_queue->get());
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clFinish. (" << err << ")" << std::endl;
        throw std::exception();
    }
}

