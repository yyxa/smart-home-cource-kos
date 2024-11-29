#include <iostream>

#include <kosipc/make_application.h>
#include <kosipc/serve_static_channel.h>
#include <kosipc/connect_static_channel.h>

#include <highlevel/Checker.edl.cpp.h>
#include <highlevel/ServiceKeys.idl.cpp.h>

using namespace kosipc::stdcpp;
using namespace highlevel;

uint8_t status_code = 0;
uint8_t current_temperature = 20;

class MyServiceChecker : public ServiceChecker
{
public:
    void On(uint8_t &result)
    {   
        std::cerr << "Checker: On()" << std::endl;

        kosipc::Application app = kosipc::MakeApplicationAutodetect();
        auto proxy = app.MakeProxy<ServiceKeys>(kosipc::ConnectStaticChannel("keys_connection", "server.service"));

        uint8_t res;
        proxy->Status(res);

        if(res == 1) {
          status_code = 1;
	        result = status_code;
        }
        else {
          result = status_code;
        }
    }

    void Off(uint8_t &result)
    {
        std::cerr << "Checker: Off()" << std::endl;
        status_code = 0;
	      result = status_code;
    }

    void Temperature(uint8_t &temperature)
    {
	      current_temperature++;
        temperature = current_temperature;
        std::cerr << "Checker: Get_temperature() = " << static_cast<int>(current_temperature) << std::endl;

        current_temperature == 100 ? current_temperature = 20 : 0;
    }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] const char *argv[])
{
    kosipc::Application app = kosipc::MakeApplicationAutodetect();

    MyServiceChecker service;

    kosipc::components::Root root;

    root.server.service = &service;

    kosipc::EventLoop loop = app.MakeEventLoop(ServeStaticChannel("checker_connection", root));

    std::cerr << "Hello! I'm Checker" << std::endl;
    loop.Run();

    return EXIT_SUCCESS;
}