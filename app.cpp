#include "app.hpp"

#include <boolinq/boolinq.h>

#include "clplatform.hpp"

#define LOG_MODULE_NAME ("App")
#include "log.hpp"

App::App()
{
    LOG_INFO << "Instance created." << std::endl;
}

void App::run(const std::vector<std::string> &args)
{
    auto a = boolinq::from(args)
        .selectMany([](const std::string &x)
            {
                return boolinq::from(x + std::string(" "))
                    .where([](char y)
                        {
                            return y != 0;
                        });
            }).toStdVector();
    a.pop_back();
    std::string b(a.begin(), a.end());
    LOG_INFO << b << std::endl;

    CLPlatform platform("NVIDIA CUDA");
}


