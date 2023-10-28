#pragma once

#include <vector>

class Image
{
public:
    virtual ~Image() { };

    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual std::vector<float> getBuffer() const = 0;
};

