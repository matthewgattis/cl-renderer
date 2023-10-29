#include "messagefactory.hpp"

#include "message.hpp"

MessageFactory::MessageFactory(
    const std::unordered_map<std::string, const std::function<std::shared_ptr<Message>(std::istream&)>>& instantiators) :
    instantiators_(instantiators)
{
}

std::shared_ptr<Message> MessageFactory::instantiate(const std::string& type, std::istream& is) const
{
    return instantiators_.at(type)(is);
}

