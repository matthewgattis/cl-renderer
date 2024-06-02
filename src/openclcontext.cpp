#include "openclcontext.hpp"

#include <memory>

#define CL_TARGET_OPENCL_VERSION 300
#include <OpenCL/cl.h>

#include "opencldevice.hpp"

#define LOG_MODULE_NAME ("OpenCLContext")
#include "log.hpp"

OpenCLContext::OpenCLContext(
    const std::shared_ptr<OpenCLDevice> &device)
    : context_(nullptr)
{
    cl_int err;
    // Create context.
    context_ = clCreateContext(
        nullptr,
        1,
        &(device->get()),
        nullptr,
        nullptr,
        &err);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clCreateContext. (" << err << ")" << std::endl;
        throw std::exception();
    }
    LOG_INFO << "Context created. (" << context_ << ")" << std::endl;
}

OpenCLContext::~OpenCLContext()
{
    if (context_ != nullptr)
    {
        // Free context.
        clReleaseContext(context_);
        LOG_INFO << "Context released." << std::endl;
    }
}

const cl_context &OpenCLContext::get() const
{
    return context_;
}

