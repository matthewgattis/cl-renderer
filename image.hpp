#pragma once

#include <vector>

struct color4
{
    float r;
    float g;
    float b;
    float a;
};

class Image
{
public:
    virtual ~Image()
    {
    };

    virtual int getWidth() const = 0;

    virtual int getHeight() const = 0;

    virtual std::vector<float> getBuffer() const = 0;
};

