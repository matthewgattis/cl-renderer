#pragma once

#include <string>
#include <vector>

#include "messagefactory.hpp"

class App
{
public:
    App();

    void run(const std::vector<std::string> &args);
};

