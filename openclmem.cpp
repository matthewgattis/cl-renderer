#include "openclmem.hpp"

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include "openclcontext.hpp"

#define LOG_MODULE_NAME ("OpenCLMem")
#include "log.hpp"

OpenCLMem::OpenCLMem(
    const std::shared_ptr<OpenCLContext> &context,
    int tile_size)
    :
        context_(context),
        tile_size_(tile_size)
{
    LOG_INFO << "Instance created." << std::endl;

    cl_int err;
    mem_ = clCreateBuffer(
        context_->get(),
        CL_MEM_READ_WRITE,
        4 * tile_size * tile_size * sizeof(cl_float),
        nullptr,
        &err);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clCreateBuffer. (" << err << ")" << std::endl;
        throw std::exception();
    }
    LOG_INFO << "Created memory buffer. (" << mem_ << ")" << std::endl;
}

OpenCLMem::~OpenCLMem()
{
    if (mem_ != nullptr)
    {
        // Free memobj.
        clReleaseMemObject(mem_);
        LOG_INFO << "Memory released." << std::endl;
    }
}

const cl_mem &OpenCLMem::get() const
{
    return mem_;
}

int OpenCLMem::tile_size() const
{
    return tile_size_;
}

