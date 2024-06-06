#include <Arduino.h>
#include "plant_waterer.h"

PlantWaterer::PlantWaterer(int blue_led, int red_led, int buzzer, int pump, int sensor, LiquidCrystal& lcd):
  blue_led(blue_led),
  red_led(red_led),
  buzzer(buzzer),
  pump(pump),
  sensor(sensor),
  lcd(lcd)
{};

PlantWaterer::state_t PlantWaterer::check_humidity(float humidity){
  if(humidity < low_humidity_threshold) {
    return state_t::watering;
  }
  else if(humidity > high_humidity_threshold) {
    return state_t::over_hydrated;
  }
  else {
    return state_t::normal;
  }
}

PlantWaterer::state_t PlantWaterer::get_next_state(){
  switch(current_state) {
    case state_t::idle:
      delay(15*1000);
    case state_t::over_hydrated:
    case state_t::normal:
      return check_humidity(current_humidity);

    case state_t::watering:
      delay(10*1000);
      return state_t::idle;
  }
}

void PlantWaterer::reset_pins(){
  digitalWrite(blue_led, LOW);
  digitalWrite(red_led, LOW);
  digitalWrite(pump, HIGH);
  noTone(buzzer);
  lcd.clear();
}

void PlantWaterer::print_humidity(){
  lcd.print((int)(current_humidity*100));
  lcd.print("%");
}

void PlantWaterer::state_handler(){
  reset_pins();

  switch(current_state){
    case state_t::watering:
      digitalWrite(blue_led, HIGH);
      digitalWrite(pump, LOW);
      lcd.print("Regando...");
      break;

    case state_t::over_hydrated:
      digitalWrite(red_led, HIGH);
      tone(buzzer, 10);
    case state_t::normal:
      print_humidity();
      break;

    case state_t::idle:
      lcd.print("idle");
      break;
  }
}

void PlantWaterer::setup() {
  Serial.begin(9600);
  pinMode(blue_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pump, OUTPUT);
  //pinMode(sensor, INPUT);

  lcd.begin(16,2);

  reset_pins();
}

void PlantWaterer::loop() {
  current_humidity = map(analogRead(sensor), 0, 1023, 100, 0)/100.f;
  Serial.println(current_humidity);

  state_handler();
  current_state = get_next_state();
}
