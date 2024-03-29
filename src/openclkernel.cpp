#include "openclkernel.hpp"

#include <CL/cl.h>
#include <memory>

#include "openclmem.hpp"
#include "openclprogram.hpp"
#include "openclcommandqueue.hpp"

#define LOG_MODULE_NAME ("OpenCLKernel")
#include "log.hpp"

OpenCLKernel::OpenCLKernel(
    const std::shared_ptr<OpenCLProgram> &program,
    const std::string &kernel_name,
    const std::shared_ptr<OpenCLMem> &mem,
    int width,
    int height)
    :
        kernel_(nullptr),
        program_(program)
{
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

    err = clSetKernelArg(
        kernel_,
        0,
        sizeof (cl_mem),
        &(mem->get()));
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clSetKernelArg. (" << err << ")" << std::endl;
        throw std::exception();
    }
    LOG_INFO << "Set memory. (" << &mem->get() << ")" << std::endl;

    cl_int arg_value = mem->tile_size();
    err = clSetKernelArg(
        kernel_,
        1,
        sizeof (cl_int),
        &arg_value);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clSetKernelArg. (" << err << ")" << std::endl;
        throw std::exception();
    }
    LOG_INFO << "Set tile size. (" << arg_value << ")" << std::endl;

    arg_value = width;
    err = clSetKernelArg(
        kernel_,
        2,
        sizeof (cl_int),
        &arg_value);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clSetKernelArg. (" << err << ")" << std::endl;
        throw std::exception();
    }
    LOG_INFO << "Set width. (" << arg_value << ")" << std::endl;

    arg_value = height;
    err = clSetKernelArg(
        kernel_,
        3,
        sizeof (cl_int),
        &arg_value);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clSetKernelArg. (" << err << ")" << std::endl;
        throw std::exception();
    }
    LOG_INFO << "Set height. (" << arg_value << ")" << std::endl;
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

void OpenCLKernel::setFrameNumber(int frame)
{
    cl_int arg_value = frame;
    cl_int err = clSetKernelArg(
        kernel_,
        4,
        sizeof (cl_int),
        &arg_value);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clSetKernelArg. (" << err << ")" << std::endl;
        throw std::exception();
    }
    LOG_INFO << "Set frame. (" << arg_value << ")" << std::endl;
}

void OpenCLKernel::enqueueKernel(
    const std::shared_ptr<OpenCLCommandQueue>& command_queue,
    const size_t* global_work_offset,
    const size_t* global_work_size)
{
    cl_int err = clEnqueueNDRangeKernel(
        command_queue->get(),
        kernel_,
        3,
        global_work_offset,
        global_work_size,
        nullptr,
        0,
        nullptr,
        nullptr);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clEnqueueNDRangeKernel. (" << err << ")" << std::endl;
        throw std::exception();
    }
}

const cl_kernel &OpenCLKernel::get() const
{
    return kernel_;
}

