#include "openclplatform.hpp"

#include <exception>
#include <tuple>
#include <utility>
#include <vector>

#include <boolinq/boolinq.h>
#include <CL/cl.h>

#define LOG_MODULE_NAME ("OpenCLPlatform")
#include "log.hpp"

OpenCLPlatform::OpenCLPlatform(
    const std::string &platform_name)
    : platform_(nullptr)
{
    LOG_INFO << "Searching for platform by name. (" << platform_name << ")" << std::endl;

    cl_int err;
    cl_uint num_platforms;
    // Get number of platforms.
    err = clGetPlatformIDs(
        0,
        nullptr,
        &num_platforms);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clGetPlatformIDs. (" << err << ")" << std::endl;
        throw std::exception();
    }

    std::vector<cl_platform_id> platforms(num_platforms);
    // Get platforms.
    err = clGetPlatformIDs(
        num_platforms,
        platforms.data(),
        nullptr);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clGetPlatformIDs. (" << err << ")" << std::endl;
        throw std::exception();
    }

    // Get first platform matching out search criteria.
    platform_ = boolinq::from(platforms)
        .first([&platform_name](const cl_platform_id &x)
            {
                if (platform_name.length() == 0)
                    return true;

                cl_int err;
                char param_value[32767];
                size_t  param_value_size_ret;
                // Get platform name.
                err = clGetPlatformInfo(
                    x,
                    CL_PLATFORM_NAME,
                    32767,
                    param_value,
                    &param_value_size_ret);
                if (err != CL_SUCCESS)
                {
                    LOG_ERROR <<
                        "Error in clGetPlatformInfo. (" << err << ")" << std::endl;
                    throw std::exception();
                }
                LOG_INFO << "Platform name. (" << param_value << ")" << std::endl;
                return std::string(param_value).compare(
                    platform_name) == 0;
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
        // Enumerate platform info.
        boolinq::from(param_names)
            .for_each([this](const std::pair<cl_platform_info, std::string> &x)
                {
                    cl_int err;
                    char param_value[32767];
                    size_t  param_value_size_ret;
                    err = clGetPlatformInfo(
                        this->platform_,
                        x.first,
                        32767,
                        param_value,
                        &param_value_size_ret);
                    if (err != CL_SUCCESS)
                    {
                        LOG_ERROR <<
                            "Error in clGetPlatformInfo. (" << err << ")" << std::endl;
                        throw std::exception();
                    }
                    LOG_INFO <<
                        "Platform " << x.second << " (" << std::string(param_value) << ")." << std::endl;
                });
    }
    catch (const std::exception &e)
    {
        LOG_WARNING <<
            "Uncaught exception enumerating platform info. (" << e.what() << ")" << std::endl;
    }
}

const cl_platform_id &OpenCLPlatform::get() const
{
    return platform_;
}

