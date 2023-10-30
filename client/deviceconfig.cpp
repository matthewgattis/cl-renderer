#include "deviceconfig.hpp"

DeviceConfig::DeviceConfig(const std::string& filename) :
    Config(filename)
{
}

const std::string DeviceConfig::getPlatform() const
{
    return root_["platform"].asString();
}

const std::string DeviceConfig::getDevice() const
{
    return root_["device"].asString();
}

std::istream& DeviceConfig::deserialize(std::istream& is)
{
    is >> root_;
    return is;
}
