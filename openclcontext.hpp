#pragma once

#include <memory>
#include <string>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

class OpenCLDevice;

class OpenCLContext
{
public:
    OpenCLContext(const std::shared_ptr<OpenCLDevice> &device);

    ~OpenCLContext();

    const cl_context &get() const;

private:
    cl_context context_;
};

