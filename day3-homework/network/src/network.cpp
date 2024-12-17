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

  auto proxy = app.MakeProxy<ServiceChecker>(kosipc::ConnectStaticChannel("checker_connection", "server.service"));

  uint8_t result = 0, temperature = 0;

  // 1. Выключаем чайник
  proxy->Off(result);
  std::cerr << "Command: Off, Result: " << static_cast<int>(result) << std::endl;

  // 2. Проверяем температуру
  proxy->Temperature(temperature);
  std::cerr << "Command: Temperature, Current Temperature: " << static_cast<int>(temperature) << std::endl;

  // 3. Включаем чайник
  proxy->On(result);
  std::cerr << "Command: On, Result: " << static_cast<int>(result) << std::endl;

  // 4. Цикл проверки температуры до середины
  int elapsed_time = 0;
  while (true) {
    proxy->Temperature(temperature);
    std::cerr << "Temperature: " << static_cast<int>(temperature) << std::endl;

    if (temperature >= 30) { // На середине выключаем
      proxy->Off(result);
      std::cerr << "Command: Off, Result: " << static_cast<int>(result) << std::endl;
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    elapsed_time++;
  }

  // 5. Ждём, пока чайник остынет
  std::cerr << "Waiting for cooling..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(20));
  proxy->Temperature(temperature);
  std::cerr << "Temperature after cooling: " << static_cast<int>(temperature) << std::endl;

  // 6. Снова включаем чайник
  proxy->On(result);
  std::cerr << "Command: On, Result: " << static_cast<int>(result) << std::endl;

  // 7. Цикл проверки температуры до 100
  while (true) {
    proxy->Temperature(temperature);
    std::cerr << "Temperature: " << static_cast<int>(temperature) << std::endl;

    if (temperature >= 100) { // Нагрелось до 100
      std::cerr << "Temperature reached 100 degrees!" << std::endl;
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  // 8. Ждём, пока чайник остынет
  std::cerr << "Waiting for cooling..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(20));
  proxy->Temperature(temperature);
  std::cerr << "Temperature after cooling: " << static_cast<int>(temperature) << std::endl;
}

int main(
  [[maybe_unused]] int argc, 
  [[maybe_unused]] const char *argv[]) 
{
  kosipc::Application app = kosipc::MakeApplicationAutodetect();

  Communication(app);

  return EXIT_SUCCESS;
}