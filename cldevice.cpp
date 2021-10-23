#include "cldevice.hpp"

#include <CL/cl.h>
#include <memory>

#define CL_TARGET_OPENCL_VERSION 300
#include <boolinq/boolinq.h>

#include "clplatform.hpp"

#define LOG_MODULE_NAME ("CLDevice")
#include "log.hpp"

CLDevice::CLDevice(
    const std::shared_ptr<CLPlatform> &platform,
    const std::string &device_name)
    : device_(nullptr)
{
    LOG_INFO << "Instance created." << std::endl;

    cl_int err;
    cl_device_id devices[64];
    cl_uint num_devices;
    err = clGetDeviceIDs(
        platform->get(),
        CL_DEVICE_TYPE_ALL,
        64,
        devices,
        &num_devices);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clGetDeviceIDs. (" << err << ")" << std::endl;
        throw std::exception();
    }

    device_ = boolinq::from(devices)
        .where_i([&num_devices](const cl_device_id &x, int i)
            {
                return i < num_devices;
            })
        .first([&device_name](const cl_device_id &x)
            {
                cl_int err;
                char param_value[32767];
                size_t param_value_size_ret;
                clGetDeviceInfo(
                    x,
                    CL_DEVICE_NAME,
                    32767,
                    param_value,
                    &param_value_size_ret);
                if (err != CL_SUCCESS)
                {
                    LOG_ERROR <<
                        "Error in clGetDeviceInfo. (" << err << ")" << std::endl;
                    throw std::exception();
                }
                return std::string(param_value).compare(device_name) == 0;
            });

    LOG_INFO << "Selected device (" << device_ << ")." << std::endl;

    size_t param_value_size_ret;
    {
        cl_device_type param_value;
        clGetDeviceInfo(
            device_,
            CL_DEVICE_TYPE,
            sizeof (cl_device_type),
            &param_value,
            &param_value_size_ret);
        if (err != CL_SUCCESS)
        {
            LOG_ERROR <<
                "Error in clGetDeviceInfo. (" << err << ")" << std::endl;
            throw std::exception();
        }
        std::string device_type;
        switch (param_value)
        {
            case CL_DEVICE_TYPE_CPU:
                device_type = "CL_DEVICE_TYPE_CPU";
                break;
            case CL_DEVICE_TYPE_GPU:
                device_type = "CL_DEVICE_TYPE_GPU";
                break;
            case CL_DEVICE_TYPE_ACCELERATOR:
                device_type = "CL_DEVICE_TYPE_ACCELERATOR";
                break;
            case CL_DEVICE_TYPE_DEFAULT:
                device_type = "CL_DEVICE_TYPE_DEFAULT";
                break;
            case CL_DEVICE_TYPE_CUSTOM:
                device_type = "CL_DEVICE_TYPE_CUSTOM";
                break;
            default:
                break;
        }
        LOG_INFO <<
            "Device type (" << device_type << ")." << std::endl;
    }

    std::vector<std::pair<cl_device_info, std::string>> param_names =
    {
        std::make_pair(CL_DEVICE_NAME, "name"),
        std::make_pair(CL_DEVICE_VENDOR, "vendor"),
        std::make_pair(CL_DRIVER_VERSION, "driver version"),
        std::make_pair(CL_DEVICE_PROFILE, "profile"),
        std::make_pair(CL_DEVICE_VERSION, "version"),
        //std::make_pair(CL_DEVICE_OPENCL_C_VERSION, "opencl c version"),
        std::make_pair(CL_DEVICE_EXTENSIONS, "extensions")
    };

    try
    {
        boolinq::from(param_names)
            .for_each([this](const std::pair<cl_platform_info, std::string> &x)
                {
                    cl_int err;
                    char param_value[32767];
                    size_t  param_value_size_ret;
                    err = clGetDeviceInfo(
                        this->device_,
                        x.first,
                        32767,
                        param_value,
                        &param_value_size_ret);
                    if (err != CL_SUCCESS)
                    {
                        LOG_ERROR <<
                            "Error in clGetDeviceInfo. (" << err << ")" << std::endl;
                        throw std::exception();
                    }
                    LOG_INFO <<
                        "Device " << x.second << " (" << std::string(param_value) << ")." << std::endl;
                });
    }
    catch (const std::exception &e)
    {
        LOG_WARNING <<
            "Uncaught exception enumerating device info log. (" << e.what() << ")" << std::endl;
    }
}

