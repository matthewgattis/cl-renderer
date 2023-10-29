#pragma once

#include <string>
#include <vector>
#include <memory>

class MessageFactory;

class App
{
public:
    App();

    void run(const std::vector<std::string> &args);

private:
    std::shared_ptr<MessageFactory> message_factory_;
};

