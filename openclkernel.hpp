#pragma once

#include <memory>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

class OpenCLProgram;

class OpenCLKernel
{
public:
    OpenCLKernel(
        const std::shared_ptr<OpenCLProgram> &program,
        const std::string &kernel_name);

    ~OpenCLKernel();

    const cl_kernel &get() const;

private:
    cl_kernel kernel_;

    std::shared_ptr<OpenCLProgram> program_;
};

