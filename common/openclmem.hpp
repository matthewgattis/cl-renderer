#pragma once

#include <memory>
#include <string>
#include <vector>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include "image.hpp"

class OpenCLContext;
class OpenCLCommandQueue;

class OpenCLMem : public Image
{
public:
    OpenCLMem(
        const std::shared_ptr<OpenCLContext> &context,
        const std::shared_ptr<OpenCLCommandQueue> &command_queue,
        int tile_size);

    ~OpenCLMem();

    std::vector<float> getBuffer() const override;

    int getWidth() const override;

    int getHeight() const override;

    const cl_mem &get() const;

    int tile_size() const;

private:
    cl_mem mem_;
    int tile_size_;

    std::shared_ptr<OpenCLContext> context_;
    std::shared_ptr<OpenCLCommandQueue> command_queue_;
};

