#pragma once

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include <memory>
#include <string>

class OpenCLPlatform;

class OpenCLDevice
{
public:
    OpenCLDevice(
        const std::shared_ptr<OpenCLPlatform> &platform,
        const std::string &device_name);

    const cl_device_id &get() const;

private:
    cl_device_id device_;
};

