#include <chrono>
#include <thread>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <kosipc/connect_static_channel.h>
#include <kosipc/make_application.h>

#include <kettle/include/logic.h>

TEST(KettleTemperatureTest, HeatingToMaxTemperature) 
{
  uint8_t current_temperature = 50;
  uint8_t status_code = 1;
  auto now = std::chrono::steady_clock::now();
  auto last_time = now;
  auto off_time = std::chrono::steady_clock::time_point{};

  now += std::chrono::seconds(50);
  UpdateTemperature(now, current_temperature, last_time, off_time, status_code);

  EXPECT_EQ(static_cast<int>(current_temperature), 100);
}

TEST(KettleTemperatureTest, AutoOffOnMaxTemperature) 
{
  uint8_t current_temperature = 99;
  uint8_t status_code = 1;
  auto now = std::chrono::steady_clock::now();
  auto last_time = now;
  auto off_time = std::chrono::steady_clock::time_point{};

  now += std::chrono::seconds(1);
  UpdateTemperature(now, current_temperature, last_time, off_time, status_code);

  EXPECT_EQ(static_cast<int>(current_temperature), 100);
  EXPECT_EQ(static_cast<int>(status_code), 0);
}

TEST(KettleTemperatureTest, CoolingAfterOff) 
{
  uint8_t current_temperature = 80;
  uint8_t status_code = 0;
  auto now = std::chrono::steady_clock::now();
  auto last_time = now;
  auto off_time = now;

  now += std::chrono::seconds(10);
  UpdateTemperature(now, current_temperature, last_time, off_time, status_code);

  EXPECT_EQ(static_cast<int>(current_temperature), 78);
}

TEST(KettleTemperatureTest, TemperatureNotBelowMin) 
{
  uint8_t current_temperature = 30;
  uint8_t status_code = 0;
  auto now = std::chrono::steady_clock::now();
  auto last_time = now;
  auto off_time = now;

  now += std::chrono::seconds(300);
  UpdateTemperature(now, current_temperature, last_time, off_time, status_code);

  EXPECT_EQ(static_cast<int>(current_temperature), 20);
}

TEST(KettleTemperatureTest, HeatingIncreaseRate) 
{
  uint8_t current_temperature = 20;
  uint8_t status_code = 1;
  auto now = std::chrono::steady_clock::now();
  auto last_time = now;
  auto off_time = std::chrono::steady_clock::time_point{};

  now += std::chrono::seconds(5);
  UpdateTemperature(now, current_temperature, last_time, off_time, status_code);

  EXPECT_EQ(static_cast<int>(current_temperature), 25);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}