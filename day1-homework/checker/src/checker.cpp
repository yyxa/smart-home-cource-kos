#include <iostream>

#include <kosipc/make_application.h>
#include <kosipc/serve_static_channel.h>
#include <kosipc/connect_static_channel.h>

#include <highlevel/Checker.edl.cpp.h>
#include <highlevel/ServiceKeys.idl.cpp.h>
#include <highlevel/ServiceKettle.idl.cpp.h>

using namespace kosipc::stdcpp;
using namespace highlevel;

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
          kosipc::Application app1 = kosipc::MakeApplicationAutodetect();
          auto proxy1 = app1.MakeProxy<ServiceKettle>(kosipc::ConnectStaticChannel("kettle_connection", "server.service"));
          uint8_t res1;
          proxy1->On(res1);
	        result = res1;
        }
        else {
          result = 100;
        }
    }

    void Off(uint8_t &result)
    {
        std::cerr << "Checker: Off()" << std::endl;

        kosipc::Application app1 = kosipc::MakeApplicationAutodetect();
        auto proxy1 = app1.MakeProxy<ServiceKettle>(kosipc::ConnectStaticChannel("kettle_connection", "server.service"));
        uint8_t res;
        proxy1->Off(res);
	      result = res;
    }

    void Temperature(uint8_t &temperature)
    {
        kosipc::Application app1 = kosipc::MakeApplicationAutodetect();
        auto proxy1 = app1.MakeProxy<ServiceKettle>(kosipc::ConnectStaticChannel("kettle_connection", "server.service"));
	      uint8_t res;
        proxy1->Temperature(res);
        current_temperature = res;
        temperature = current_temperature;
        std::cerr << "Checker: Get_temperature() = " << static_cast<int>(current_temperature) << std::endl;

    }
private:
    

    
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