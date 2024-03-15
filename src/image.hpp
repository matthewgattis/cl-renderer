#pragma once

#include <vector>

class Image
{
public:
    virtual int width() const = 0;

    virtual int height() const = 0;

    virtual std::vector<float> pixels() const = 0;

    virtual ~Image()
    {
    }

protected:
    Image()
    {
    }
};

