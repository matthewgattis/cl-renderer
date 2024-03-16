#pragma once

#include <memory>
#include <string>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

class OpenCLMem;
class OpenCLProgram;
class OpenCLCommandQueue;

class OpenCLKernel
{
public:
    OpenCLKernel(
        const std::shared_ptr<OpenCLProgram> &program,
        const std::string &kernel_name,
        const std::shared_ptr<OpenCLMem> &mem,
        int width,
        int height);

    void setFrameNumber(int frame);

    void enqueueKernel(
        const std::shared_ptr<OpenCLCommandQueue>& command_queue,
        const size_t* global_work_offset,
        const size_t* global_work_size);

    ~OpenCLKernel();

    const cl_kernel &get() const;

private:
    cl_kernel kernel_;

    std::shared_ptr<OpenCLProgram> program_;
};

