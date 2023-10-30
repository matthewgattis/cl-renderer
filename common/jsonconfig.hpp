#pragma once

#include "config.hpp"

class JsonConfig : Config
{
public:
    JsonConfig(const std::string& filename) :
        Config(filename)
    {
    }

    JsonConfig(std::istream& is) :
        Config(is)
    {
    }

    virtual std::istream& deserialize(std::istream& is) override
    {
        is >> root_;
        return is;
    }

protected:
    Json::Value root_;
};

