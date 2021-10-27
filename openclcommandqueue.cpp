#include "openclcommandqueue.hpp"

#include "openclcontext.hpp"
#include "opencldevice.hpp"

#define LOG_MODULE_NAME ("OpenCLCommandQueue")
#include "log.hpp"

OpenCLCommandQueue::OpenCLCommandQueue(
    const std::shared_ptr<OpenCLContext> &context,
    const std::shared_ptr<OpenCLDevice> &device)
    : command_queue_(nullptr)
{
    LOG_INFO << "Instance created." << std::endl;

    cl_int err;
    // Create command queue.
    command_queue_ = clCreateCommandQueueWithProperties(
        context->get(),
        device->get(),
        nullptr,
        &err);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clCreateCommandQueueWithProperties. (" << err << ")" << std::endl;
        throw std::exception();
    }
    LOG_INFO << "Created command queue. (" << command_queue_ << ")" << std::endl;
}

OpenCLCommandQueue::~OpenCLCommandQueue()
{
    if (command_queue_ != nullptr)
    {
        // Free program.
        clReleaseCommandQueue(command_queue_);
        LOG_INFO << "Command queue released." << std::endl;
    }
}

const cl_command_queue &OpenCLCommandQueue::get() const
{
    return command_queue_;
}

