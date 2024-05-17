#include <LiquidCrystal.h>

const int blue_led;
const int red_led;
const int buzzer;
const int pump;

enum class state_t {
  normal,
  watering,
  idle,
  over_hydrated
};

float current_humidity;

const float low_humidity_threshold = 0.2;
const float high_humidity_threshold = 0.8;
state_t current_state = state_t::normal;

LiquidCrystal lcd(21, 20, 19,18,17,16,15,14, 3,2);

state_t check_humidity(float humidity){
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

state_t get_next_state(){
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

void reset_pins(){
  digitalWrite(blue_led, LOW);
  digitalWrite(red_led, LOW);
  digitalWrite(pump, LOW);
  noTone(buzzer);
  lcd.clear();
}

void print_humidity(){
  lcd.print((int)(current_humidity*100));
  lcd.print("%");
} 

void state_handler(){
  reset_pins();
  
  switch(current_state){
    case state_t::watering:
      digitalWrite(blue_led, HIGH);
      digitalWrite(pump, HIGH);
      lcd.print("Regando...");
      break;

    case state_t::over_hydrated:
      digitalWrite(red_led, HIGH);
      tone(buzzer, 1000);
    case state_t::normal:
      print_humidity();
      break;

    case state_t::idle:
      lcd.print("idle");
      break;
  }
}


void setup() {
  pinMode(blue_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  lcd.begin(16,2);
}

void loop() {
  state_handler();
  current_state = get_next_state();
}
