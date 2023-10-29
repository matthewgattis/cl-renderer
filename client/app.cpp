#include "app.hpp"

#include <fstream>
#include <memory>
#include <unordered_map>

#include "openclplatform.hpp"
#include "devices.hpp"
#include "messagefactory.hpp"

#define LOG_MODULE_NAME ("App")
#include "log.hpp"

App::App()
{
    message_factory_ = std::make_shared<MessageFactory>(
        std::unordered_map<std::string, const std::function<std::shared_ptr<Message>(std::istream&)>>
        {
            { "devices", Devices::INSTANTIATOR }
        });
}

void App::run(const std::vector<std::string> &args)
{
}

