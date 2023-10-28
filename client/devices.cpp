#include "devices.hpp"

#include <fstream>

std::vector<std::string> Devices::get() const
{
    std::vector<std::string> devices;
    for (const auto &device : root_)
        devices.push_back(device.asString());

    return devices;
}

Devices::Devices(const std::string& filename) :
    Message(),
    Config(filename)
{
}

Devices::Devices(std::istream& is) :
    Message(),
    Config(is)
{
}

std::ostream& Devices::serialize(std::ostream& os) const
{
    os << root_;
    return os;
}

std::istream& Devices::deserialize(std::istream& is)
{
    is >> root_;
    return is;
}

