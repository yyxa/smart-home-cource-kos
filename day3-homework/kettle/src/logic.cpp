#include <iostream>
#include <algorithm>
#include <chrono>

#include <kettle/include/logic.h>

using namespace std::chrono;

void UpdateTemperature(
  std::chrono::steady_clock::time_point& now, 
  uint8_t& current_temperature, 
  std::chrono::steady_clock::time_point& last_time, 
  std::chrono::steady_clock::time_point& off_time, 
  uint8_t& status_code) 
{
  if (status_code == 1) {
    auto elapsed_seconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - last_time).count());
    int new_temperature = static_cast<int>(current_temperature) + elapsed_seconds * HEAT_RATE;

    if (new_temperature >= MAX_TEMPERATURE) {
      current_temperature = MAX_TEMPERATURE;
      status_code = 0; // Автоматически выключаем чайник
      off_time = now;
      std::cerr << "Kettle: Temperature reached 100°C. Auto Off." << std::endl;
    } 
    else {
      current_temperature = static_cast<uint8_t>(new_temperature);
    }

    last_time = now;
  } 
  else if (status_code == 0 && off_time != std::chrono::steady_clock::time_point{}) {
    auto elapsed_seconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - off_time).count());
    int cooled_temperature = elapsed_seconds / COOL_INTERVAL * COOL_RATE;
    int new_temperature = static_cast<int>(current_temperature) - cooled_temperature;
    current_temperature = static_cast<uint8_t>(std::max(new_temperature, static_cast<int>(MIN_TEMPERATURE)));
  }
}