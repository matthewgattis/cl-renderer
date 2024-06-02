#pragma once

#include <memory>
#include <string>
#include <vector>

#include "image.hpp"

#define CL_TARGET_OPENCL_VERSION 300
#include <OpenCL/cl.h>

class OpenCLContext;
class OpenCLCommandQueue;

class OpenCLMem : public Image
{
public:
    OpenCLMem(
        const std::shared_ptr<OpenCLContext> &context,
        const std::shared_ptr<OpenCLCommandQueue> &command_queue,
        int tile_size);

    virtual ~OpenCLMem();

    virtual std::vector<float> pixels() const override;

    inline const cl_mem &get() const
    {
        return mem_;
    }

    inline int tile_size() const
    {
        return tile_size_;
    }

    inline virtual int width() const override
    {
        return tile_size_;
    }

    inline virtual int height() const override
    {
        return tile_size_;
    }

private:
    cl_mem mem_;
    int tile_size_;

    std::shared_ptr<OpenCLContext> context_;
    std::shared_ptr<OpenCLCommandQueue> command_queue_;
};

