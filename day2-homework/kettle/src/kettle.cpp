#include <iostream>
#include <chrono>
#include <algorithm>

#include <kosipc/make_application.h>
#include <kosipc/serve_static_channel.h>

#include <highlevel/Kettle.edl.cpp.h>

using namespace kosipc::stdcpp;
using namespace highlevel;
using namespace std::chrono;

uint8_t status_code = 0;
uint8_t current_temperature = 20;
steady_clock::time_point last_time;    // Время последней операции
steady_clock::time_point off_time;    // Время выключения чайника

constexpr uint8_t MAX_TEMPERATURE = 100;
constexpr uint8_t MIN_TEMPERATURE = 20;
constexpr int HEAT_RATE = 1;          // Скорость нагрева: 1°C в секунду
constexpr int COOL_RATE = 1;          // Скорость остывания: 1°C за 5 секунд
constexpr int COOL_INTERVAL = 5;      // Интервал остывания: каждые 5 секунд

class MyServiceKettle : public ServiceKettle
{
public:
    void On(uint8_t &result)
    {   
        std::cerr << "Kettle: On()" << std::endl;

        if (status_code == 0) {
            auto now = steady_clock::now();
            if (status_code == 0) {
                UpdateTemperature(now);
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
            UpdateTemperature(now);
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
        UpdateTemperature(now);
        temperature = current_temperature;
        std::cerr << "Kettle: Get_temperature() = " << static_cast<int>(current_temperature) << std::endl;
    }

private:
    void UpdateTemperature(steady_clock::time_point now)
    {
        if (status_code == 1) {
            auto elapsed_seconds = static_cast<int>(duration_cast<seconds>(now - last_time).count());
            int new_temperature = static_cast<int>(current_temperature) + elapsed_seconds * HEAT_RATE;

            if (new_temperature >= MAX_TEMPERATURE) {
                current_temperature = MAX_TEMPERATURE;
                status_code = 0; // Автоматически выключаем чайник
                off_time = now;
                std::cerr << "Kettle: Temperature reached 100°C. Auto Off." << std::endl;
            } else {
                current_temperature = static_cast<uint8_t>(new_temperature);
            }

            last_time = now;
        } else if (status_code == 0 && off_time != steady_clock::time_point{}) {
            auto elapsed_seconds = static_cast<int>(duration_cast<seconds>(now - off_time).count());
            int cooled_temperature = elapsed_seconds / COOL_INTERVAL * COOL_RATE;
            int new_temperature = static_cast<int>(current_temperature) - cooled_temperature;
            current_temperature = static_cast<uint8_t>(std::max(new_temperature, static_cast<int>(MIN_TEMPERATURE)));
        }
    }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] const char *argv[])
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
