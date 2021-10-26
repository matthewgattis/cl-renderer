#include "app.hpp"

#include <boolinq/boolinq.h>
#include <fstream>
#include <memory>

#include "openclcontext.hpp"
#include "opencldevice.hpp"
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
    auto platform = std::make_shared<OpenCLPlatform>("NVIDIA CUDA");
    auto device = std::make_shared<OpenCLDevice>(platform, "NVIDIA GeForce GTX 970");
    auto context = std::make_shared<OpenCLContext>(device);
    std::ifstream ifs("kernel.cl");
    auto program = std::make_shared<OpenCLProgram>(context, device, ifs);
}

