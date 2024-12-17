#include <iostream>
#include <chrono>
#include <algorithm>

#include <kettle/include/logic.h>

#include <kosipc/make_application.h>
#include <kosipc/serve_static_channel.h>

#include <highlevel/Kettle.edl.cpp.h>

using namespace kosipc::stdcpp;
using namespace highlevel;
using namespace std::chrono;

class MyServiceKettle : public ServiceKettle
{
  public:
    void On(uint8_t &result)
    {   
      std::cerr << "Kettle: On()" << std::endl;

      if (status_code == 0) {
        auto now = steady_clock::now();
        if (status_code == 0) {
          UpdateTemperature(now, current_temperature, last_time, off_time, status_code);
          last_time = now;
        }
        status_code = 1;
      }

      result = status_code;
    }

    void Off(uint8_t &result)
    {
      std::cerr << "Kettle: Off()" << std::endl;
      
      if (status_code == 1) {
        auto now = steady_clock::now();
        UpdateTemperature(now, current_temperature, last_time, off_time, status_code);
        if (off_time == steady_clock::time_point{}) {
          off_time = now;
        }
      }

      status_code = 0;
      result = status_code;
    }

    void Temperature(uint8_t &temperature)
    {
      auto now = steady_clock::now();
      UpdateTemperature(now, current_temperature, last_time, off_time, status_code);
      temperature = current_temperature;
      std::cerr << "Kettle: Get_temperature() = " << static_cast<int>(current_temperature) << std::endl;
    }

  private:
    uint8_t status_code = 0;
    uint8_t current_temperature = 20;
    steady_clock::time_point last_time;    // Время последней операции
    steady_clock::time_point off_time;    // Время выключения чайника
};

int main(
  [[maybe_unused]] int argc, 
  [[maybe_unused]] const char *argv[])
{       
  kosipc::Application app = kosipc::MakeApplicationAutodetect();

  MyServiceKettle service;

  kosipc::components::Root root;

  root.server.service = &service;

  kosipc::EventLoop loop = app.MakeEventLoop(ServeStaticChannel("kettle_connection", root));

  std::cerr << "Hello! I'm Kettle" << std::endl;
  loop.Run();

  return EXIT_SUCCESS;
}
