#pragma once

#include <string>
#include <utility>

#include <json/json.h>

#include "config.hpp"

class ProjectConfig : public Config
{
public:
    ProjectConfig(const std::string& filename);

    std::string getProgram() const;
    int getWidth() const;
    int getHeight() const;
    int getTileHeight() const;
    int getSamples() const;
    int 

public:
    virtual std::istream& deserialize(std::istream& is) override;

private:
    Json::Value root_;
};

