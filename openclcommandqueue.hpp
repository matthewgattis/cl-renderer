#pragma once

#include <memory>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

class OpenCLContext;
class OpenCLDevice;

class OpenCLCommandQueue
{
public:
    OpenCLCommandQueue(
        const std::shared_ptr<OpenCLContext> &context,
        const std::shared_ptr<OpenCLDevice> &device);

    ~OpenCLCommandQueue();

    const cl_command_queue &get() const;

private:
    cl_command_queue command_queue_;
};

