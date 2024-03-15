#pragma once

#include <string>
#include <vector>

class PNGImageWriter
{
public:
    static void writeImage(
        const std::string &filename,
        const std::vector<float> &pixels,
        int width);
};

