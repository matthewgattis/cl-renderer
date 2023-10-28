#pragma once

#include <string>
#include <vector>

#include <json/json.h>

#include "config.hpp"
#include "message.hpp"

class Devices : public Config, public Message
{
public:
    std::vector<std::string> get() const;

public:
    Devices(const std::string& filename);
    Devices(std::istream& is);

    virtual std::ostream& serialize(std::ostream& os) const override;
    virtual std::istream& deserialize(std::istream& is) override;

private:
    Json::Value root_;
};

