#pragma once

#include <fstream>

#include "deserializable.hpp"

class Config : public Deserializable
{
protected:
    Config(const std::string& filename)
    {
        std::ifstream ifs(filename);
        ifs >> *this;
    }

    Config(std::istream& is) :
        Deserializable(is)
    {
    }
};

