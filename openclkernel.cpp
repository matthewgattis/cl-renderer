#include "openclkernel.hpp"

#include <CL/cl.h>
#include <memory>

#include "openclprogram.hpp"

#define LOG_MODULE_NAME ("OpenCLKernel")
#include "log.hpp"

OpenCLKernel::OpenCLKernel(
    const std::shared_ptr<OpenCLProgram> &program,
    const std::string &kernel_name)
    : kernel_(nullptr),
      program_(program)
{
    LOG_INFO << "Instance created." << std::endl;

    cl_int err;
    // Create kernel.
    kernel_ = clCreateKernel(
        program->get(),
        kernel_name.c_str(),
        &err);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clCreateKernel. (" << err << ")" << std::endl;
        throw std::exception();
    }
    LOG_INFO << "Created kernel. (" << kernel_ << ")" << std::endl;
}

OpenCLKernel::~OpenCLKernel()
{
    if (kernel_ != nullptr)
    {
        // Free kernel.
        clReleaseKernel(kernel_);
        LOG_INFO << "Kernel released." << std::endl;
    }
}

const cl_kernel &OpenCLKernel::get() const
{
    return kernel_;
}

