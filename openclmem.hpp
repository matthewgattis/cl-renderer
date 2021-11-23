#pragma once

#include <memory>
#include <string>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

class OpenCLContext;

class OpenCLMem
{
public:
    OpenCLMem(
        const std::shared_ptr<OpenCLContext> &context,
        int tile_size);

    ~OpenCLMem();

    const cl_mem &get() const;

    int tile_size() const;

private:
    cl_mem mem_;
    int tile_size_;

    std::shared_ptr<OpenCLContext> context_;
};

