#include "openclmem.hpp"

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include "openclcontext.hpp"
#include "openclcommandqueue.hpp"

#define LOG_MODULE_NAME ("OpenCLMem")
#include "log.hpp"

OpenCLMem::OpenCLMem(
    const std::shared_ptr<OpenCLContext> &context,
    const std::shared_ptr<OpenCLCommandQueue> &command_queue,
    int tile_size) :
    context_(context),
    command_queue_(command_queue),
    tile_size_(tile_size)
{
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

std::vector<float> OpenCLMem::pixels() const
{
    std::vector<float> a;
    a.resize(4 * tile_size_ * tile_size_);

    clEnqueueReadBuffer(
        command_queue_->get(),
        mem_,
        true,
        0,
        4 * tile_size_ * tile_size_ * sizeof (cl_float),
        a.data(),
        0,
        nullptr,
        nullptr);

    return a;
}

