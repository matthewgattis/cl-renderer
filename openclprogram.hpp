#pragma once

#include <memory>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

class OpenCLContext;
class OpenCLDevice;

class OpenCLProgram
{
public:
    OpenCLProgram(
        const std::shared_ptr<OpenCLContext> &context,
        const std::shared_ptr<OpenCLDevice> &device,
        std::istream &is);

    ~OpenCLProgram();

    const cl_context &get() const;

private:
    cl_program program_;

    std::shared_ptr<OpenCLContext> context_;
};

