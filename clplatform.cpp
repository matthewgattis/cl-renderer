#include "clplatform.hpp"

#include <exception>
#include <tuple>
#include <utility>
#include <vector>

#include <boolinq/boolinq.h>
#include <CL/cl.h>

#define LOG_MODULE_NAME ("CLPlatform")
#include "log.hpp"

CLPlatform::CLPlatform(
    const std::string &platform_name)
    : platform_(nullptr)
{
    LOG_INFO << "Instance created." << std::endl;

    cl_int err;
    cl_platform_id platforms[64];
    cl_uint num_platforms;
    err = clGetPlatformIDs(64, platforms, &num_platforms);

    platform_ = boolinq::from(platforms)
        .where_i([&num_platforms](const cl_platform_id &x, int i)
            {
                return i < num_platforms;
            })
        .first([&platform_name](const cl_platform_id &x)
            {
                cl_int err;
                char param_value[32767];
                size_t  param_value_size_ret;
                err = clGetPlatformInfo(x, CL_PLATFORM_NAME, 32767, param_value, &param_value_size_ret);
                if (err != CL_SUCCESS)
                {
                    LOG_ERROR << "Error in clGetPlatformInfo (" << err << ")." << std::endl;
                    throw std::exception();
                }
                return std::string(param_value).compare(platform_name) == 0;
            });

    LOG_INFO << "Selected platform (" << platform_ << ")." << std::endl;

    std::vector<std::pair<cl_platform_info, std::string>> param_names =
    {
        std::make_pair(CL_PLATFORM_PROFILE, "profile"),
        std::make_pair(CL_PLATFORM_VERSION, "version"),
        std::make_pair(CL_PLATFORM_NAME, "name"),
        std::make_pair(CL_PLATFORM_VENDOR, "vendor"),
        std::make_pair(CL_PLATFORM_EXTENSIONS, "extensions")
    };

    try
    {
        boolinq::from(param_names)
            .for_each([this](const std::pair<cl_platform_info, std::string> &x)
                {
                    cl_int err;
                    char param_value[32767];
                    size_t  param_value_size_ret;
                    err = clGetPlatformInfo(this->platform_, x.first, 32767, param_value, &param_value_size_ret);
                    if (err != CL_SUCCESS)
                    {
                        LOG_ERROR << "Error in clGetPlatformInfo. (" << err << ")" << std::endl;
                        throw std::exception();
                    }
                    LOG_INFO << "Platform " << x.second << ". (" << std::string(param_value) << ")" << std::endl;
                });
    }
    catch (const std::exception &e)
    {
        LOG_WARNING << "Uncaught exception enumerating platform info log. (" << e.what() << ")" << std::endl;
    }
}

const cl_platform_id &CLPlatform::get() const
{
    return platform_;
}

