#include <iostream>
#include <thread>
#include <chrono>

#include <kosipc/connect_static_channel.h>
#include <kosipc/make_application.h>


#include <highlevel/Network.edl.cpp.h>
#include <highlevel/ServiceChecker.idl.cpp.h>

using namespace kosipc::stdcpp;
using namespace highlevel;

void Communication(kosipc::Application& app)
{
    std::cerr << "Hello! I'm Network." << std::endl;
    // int i;

    auto proxy = app.MakeProxy<ServiceChecker>(kosipc::ConnectStaticChannel("checker_connection", "server.service"));

    uint8_t result, temperature;
    proxy->On(result);

    while(result == 1) {
      proxy->Temperature(temperature);
      std::cerr << "Network: temperature = " << static_cast<int>(temperature) << std::endl;
      temperature == 100 ? proxy->Off(result) : std::this_thread::sleep_for(std::chrono::seconds(1));;
    }

    // for (i = 0; i < 10; ++i)
    // {
    //     std::cerr << "Network: value = " << i << std::endl;
    //     uint32_t res;
    //     proxy->Echo((uint32_t)i, res);
    //     std::cerr << "Network: echo result = " << res << std::endl;
    // }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char *argv[]) {

    kosipc::Application app = kosipc::MakeApplicationAutodetect();

    Communication(app);

    return EXIT_SUCCESS;
}
