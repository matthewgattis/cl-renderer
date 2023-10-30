#include "devicemessage.hpp"

#include <fstream>
#include <memory>

const std::function<std::shared_ptr<Message>(std::istream&)> DeviceMessage::INSTANTIATOR = 
    std::function<std::shared_ptr<Message>(std::istream&)>(
        [](std::istream& is) -> std::shared_ptr<Message>
        {
            return std::make_shared<DeviceMessage>(is);
        });
const std::string DeviceMessage::TYPE = "device-message";

std::vector<std::string> DeviceMessage::getDevices() const
{
    std::vector<std::string> devices;
    for (const auto &device : root_)
        devices.push_back(device.asString());

    return devices;
}

DeviceMessage::DeviceMessage(std::istream& is) :
    Message(is)
{
}

std::string DeviceMessage::getType() const
{
    return TYPE;
}

std::ostream& DeviceMessage::serialize() const
{
    os << root_;
    return os;
}

std::istream& DeviceMessage::deserialize(std::istream& is)
{
    is >> root_;
    return is;
}

