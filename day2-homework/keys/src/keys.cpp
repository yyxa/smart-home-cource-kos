#include <iostream>

#include <kosipc/make_application.h>
#include <kosipc/serve_static_channel.h>

#include <highlevel/Keys.edl.cpp.h>

using namespace kosipc::stdcpp;
using namespace highlevel;

uint8_t status_code = 1;

class MyServiceKeys : public ServiceKeys
{
public:
    void Status(uint8_t &result)
    {   
        std::cerr << "Keys: Status() = " << static_cast<int>(status_code) << std::endl;
	      result = status_code;
    }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] const char *argv[])
{       
    kosipc::Application app = kosipc::MakeApplicationAutodetect();

    MyServiceKeys service;

    kosipc::components::Root root;

    root.server.service = &service;

    kosipc::EventLoop loop = app.MakeEventLoop(ServeStaticChannel("keys_connection", root));

    std::cerr << "Hello! I'm Keys" << std::endl;
    loop.Run();

    return EXIT_SUCCESS;
}
