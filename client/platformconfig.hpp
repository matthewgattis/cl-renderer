#pragma once

#include <string>
#include <vector>

#include <json/json.h>

#include "config.hpp"

class PlatformConfig : public Config
{
public:
    PlatformConfig(const std::string& filename);

    std::vector<std::string> getDeviceList() const;

    virtual std::istream& deserialize(std::istream& is) override;

private:
    Json::Value root_;
};

