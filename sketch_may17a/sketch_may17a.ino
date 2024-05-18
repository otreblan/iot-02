#include <LiquidCrystal.h>
#include "plant_waterer.h"

const int blue_led = 31;
const int red_led = 33;
const int buzzer = 35;
const int pump = 37;
const int sensor = 7;

LiquidCrystal lcd(24, 22, 38, 40, 42, 44, 46, 48, 50, 52);
PlantWaterer pw(blue_led, red_led, buzzer, pump, sensor, lcd);

void setup() {
  pw.setup();
}

void loop() {
  pw.loop();
}
