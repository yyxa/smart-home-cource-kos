#include <chrono>

using namespace std::chrono;

constexpr uint8_t MAX_TEMPERATURE = 100;
constexpr uint8_t MIN_TEMPERATURE = 20;
constexpr int HEAT_RATE = 1;          // Скорость нагрева: 1°C в секунду
constexpr int COOL_RATE = 1;          // Скорость остывания: 1°C за 5 секунд
constexpr int COOL_INTERVAL = 5;      // Интервал остывания: каждые 5 секунд

void UpdateTemperature(
  std::chrono::steady_clock::time_point& now, 
  uint8_t& current_temperature, 
  std::chrono::steady_clock::time_point& last_time, 
  std::chrono::steady_clock::time_point& off_time, 
  uint8_t& status_code);
