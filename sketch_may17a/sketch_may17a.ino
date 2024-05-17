#include <LiquidCrystal.h>
#include "plant_waterer.h"

const int blue_led;
const int red_led;
const int buzzer;
const int pump;

LiquidCrystal lcd(21, 20, 19,18,17,16,15,14, 3,2);
PlantWaterer pw(blue_led, red_led, buzzer, pump, lcd);

void setup() {
  pw.setup();
}

void loop() {
  pw.loop();
}
