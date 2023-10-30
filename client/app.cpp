#include "app.hpp"

#include <fstream>
#include <memory>
#include <unordered_map>

#include <boolinq/boolinq.h>
#include <json/json.h>

#include "openclcommandqueue.hpp"
#include "openclcontext.hpp"
#include "opencldevice.hpp"
#include "openclkernel.hpp"
#include "openclmem.hpp"
#include "openclplatform.hpp"
#include "openclprogram.hpp"

#include "devicemessage.hpp"
#include "messagefactory.hpp"
#include "deviceconfig.hpp"

#define LOG_MODULE_NAME ("App")
#include "log.hpp"

App::App()
{
    message_factory_ = std::make_shared<MessageFactory>(
        std::unordered_map<std::string, const std::function<std::shared_ptr<Message>(std::istream&)>>
        {
            { "device-message", DeviceMessage::INSTANTIATOR }
        });
}

void App::run(const std::vector<std::string> &args)
{
    DeviceConfig device_config("devices.json");

    auto platform = std::make_shared<OpenCLPlatform>(device_config.getPlatform());
    auto device = std::make_shared<OpenCLDevice>(platform,device_config.getDevice());
    auto context = std::make_shared<OpenCLContext>(device);
    std::ifstream ifs("program.cl");
    auto program = std::make_shared<OpenCLProgram>(context, device, ifs);
    auto command_queue = std::make_shared<OpenCLCommandQueue>(context, device);
    auto mem = std::make_shared<OpenCLMem>(context, command_queue, tile_size);
    auto kernel = std::make_shared<OpenCLKernel>(program, "mainimage", mem, width, height);

    // boolinq::from(platforms.begin(), platforms.end())
    //     .select([](const Json::Value& x) -> std::pair<std::shared_ptr<OpenCLPlatform>, std::vector<std::shared_ptr<OpenCLDevice>>>
    //     {
    //         auto platform = std::make_shared<OpenCLPlatform>(x["name"].asString());
    //         auto z = x["devices"];
    //         auto devices = boolinq::from(z.begin(), z.end())
    //             .select([&platform](const Json::Value& y) -> std::shared_ptr<OpenCLDevice>
    //             {
    //                 return std::make_shared<OpenCLDevice>(platform, y);
    //             }).toStdVector();
    //         return platform;
    //     }).toStdVector();
}

