#pragma once

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include <memory>
#include <string>

class CLPlatform;

class CLDevice
{
public:
    CLDevice(
        const std::shared_ptr<CLPlatform> &platform,
        const std::string &device_name);

    const cl_device_id &get() const;

private:
    cl_device_id device_;
};

