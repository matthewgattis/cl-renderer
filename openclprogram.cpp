#include "openclprogram.hpp"

#include <sstream>
#include <string>
#include <vector>

#include <CL/cl.h>

#include "openclcontext.hpp"
#include "opencldevice.hpp"

#define LOG_MODULE_NAME ("OpenCLProgram")
#include "log.hpp"

OpenCLProgram::OpenCLProgram(
    const std::shared_ptr<OpenCLContext> &context,
    const std::shared_ptr<OpenCLDevice> &device,
    std::istream &is)
    : program_(nullptr),
      context_(context)
{
    LOG_INFO << "Instance created." << std::endl;

    // Read source code from stream.
    std::stringstream ss;
    ss << is.rdbuf();
    char *strings = new char[ss.str().size() + 1]();
    ss.str().copy(strings, ss.str().size());

    // Log source code.
    //LOG_INFO << ss.str() << std::endl;

    // Create program handle.
    cl_int err;
    program_ = clCreateProgramWithSource(
        context->get(),
        1,
        (const char **)&strings,
        nullptr,
        &err);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clCreateProgramWithSource. (" << err << ")" << std::endl;
        throw std::exception();
    }
    LOG_INFO << "Program created. (" << program_ << ")" << std::endl;

    // Build program.
    err = clBuildProgram(
        program_,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr);
    if (err != CL_SUCCESS)
    {
        LOG_WARNING <<
            "Warning in clBuildProgram. (" << err << ")" << std::endl;
    }
    
    // Program build log size.
    size_t param_value_size_ret;
    // Get program build log size.
    err = clGetProgramBuildInfo(
        program_,
        device->get(),
        CL_PROGRAM_BUILD_LOG,
        0,
        nullptr,
        &param_value_size_ret);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clGetProgramBuildInfo. (" << err << ")" << std::endl;
        throw std::exception();
    }
    
    // Get program build log.
    char *param_value =
        new char[param_value_size_ret + 1]();
    err = clGetProgramBuildInfo(
        program_,
        device->get(),
        CL_PROGRAM_BUILD_LOG,
        param_value_size_ret,
        param_value,
        &param_value_size_ret);
    if (err != CL_SUCCESS)
    {
        LOG_ERROR <<
            "Error in clGetProgramBuildInfo. (" << err << ")" << std::endl;
        throw std::exception();
    }
    LOG_INFO << std::string(param_value) << std::endl;

    delete[] param_value;
}

OpenCLProgram::~OpenCLProgram()
{
    if (program_ != nullptr)
    {
        // Free program.
        clReleaseProgram(program_);
        LOG_INFO << "Program released." << std::endl;
    }
}

const cl_program &OpenCLProgram::get() const
{
    return program_;
}

