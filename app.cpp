#include "app.hpp"

#include <boolinq/boolinq.h>
#include <memory>

#include "cldevice.hpp"
#include "clplatform.hpp"

#define LOG_MODULE_NAME ("App")
#include "log.hpp"

App::App()
{
    LOG_INFO << "Instance created." << std::endl;
}

void App::run(const std::vector<std::string> &args)
{
    auto platform = std::make_shared<CLPlatform>("NVIDIA CUDA");
    auto device = std::make_shared<CLDevice>(platform, "NVIDIA GeForce GTX 970");
}

