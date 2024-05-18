#pragma once

#include <LiquidCrystal.h>

class PlantWaterer{
private:
  enum class state_t {
    normal,
    watering,
    idle,
    over_hydrated
  };

  const int blue_led;
  const int red_led;
  const int buzzer;
  const int pump;
  const int sensor;

  static const float low_humidity_threshold = 0.2;
  static const float high_humidity_threshold = 0.8;

  LiquidCrystal& lcd;
  float current_humidity;
  state_t current_state = state_t::normal;

  state_t check_humidity(float humidity);
  state_t get_next_state();
  void reset_pins();
  void print_humidity();
  void state_handler();
  
public:
  PlantWaterer(int blue_led, int red_led, int buzzer, int pump, int sensor, LiquidCrystal& lcd);
  void setup();
  void loop();
};
