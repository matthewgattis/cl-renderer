#include "devices.hpp"

#include <fstream>
#include <memory>

const std::function<std::shared_ptr<Message>(std::istream&)> Devices::INSTANTIATOR = 
    std::function<std::shared_ptr<Message>(std::istream&)>(
        [](std::istream& is) -> std::shared_ptr<Message>
        {
            return std::make_shared<Devices>(is);
        });

std::vector<std::string> Devices::getDevices() const
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

std::string Devices::getType() const
{
    return "devices";
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

