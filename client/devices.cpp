#include "devices.hpp"

#include <fstream>

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

