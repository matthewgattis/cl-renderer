#pragma once

#include <string>
#include <vector>
#include <utility>

#include <json/json.h>

#include "config.hpp"

class DeviceConfig : public Config
{
public:
    DeviceConfig(const std::string& filename);

    const std::string getPlatform() const;
    const std::string getDevice() const;

public:
    virtual std::istream& deserialize(std::istream& is) override;

private:
    Json::Value root_;
};

