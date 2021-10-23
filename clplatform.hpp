#pragma once

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include <string>

class CLPlatform
{
public:
    CLPlatform(const std::string &platform_name);

    const cl_platform_id &get() const;

private:
    cl_platform_id platform_;
};

