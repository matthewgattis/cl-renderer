#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>

class Message;

class MessageFactory
{
public:
    MessageFactory(
        const std::unordered_map<std::string, const std::function<std::shared_ptr<Message>(std::istream&)>>& instantiators);

    std::shared_ptr<Message> instantiate(const std::string& type, std::istream& is) const;

private:
    std::unordered_map<std::string, const std::function<std::shared_ptr<Message>(std::istream&)>> instantiators_;
};

