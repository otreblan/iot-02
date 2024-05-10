#include <hcsr04.h>

const int red_led = 20;
const int green_led = 22;
const int yellow_led = 24;



HCSR04 hcsr04(51, 53, 20, 4000);

void setup() {
  Serial.begin(9600);

  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
}

void loop() {
  int distance = hcsr04.distanceInMillimeters();
  Serial.println(distance);
  
  digitalWrite(yellow_led, (distance < 30*10) ? HIGH : LOW);
}
