#include <Servo.h>

#include <LiquidCrystal.h>

#include <hcsr04.h>
#include <Keypad.h>

const int red_led = 25;
const int green_led = 27;
const int yellow_led = 29;

const int back_led = 39;

const int servo_pin = 26;

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {44,42,40,38};
byte colPins[COLS] = {36,34,32,30};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );

HCSR04 hcsr04(51, 53, 20, 4000);

LiquidCrystal lcd(21, 20, 19,18,17,16,15,14, 2,3);

unsigned long last_door_opening = 0;
bool door_open = false;
Servo door_servo;

int password_buffer = 0;
int digits = 0;
int correct_password = 1234;

void open_door(){
  door_servo.write(120);
  last_door_opening = millis();
  door_open = true;
}

void close_door(){
  door_servo.write(0);
  door_open = false;
}

void newDigit(int digit){
  password_buffer = password_buffer*10+digit;
  Serial.println(digit);

  if(digits++ == 0)
    lcd.clear();

  lcd.blink();
  lcd.print("*");
}

void check_password(int password){
  lcd.noBlink();
  if(password == correct_password){
    digitalWrite(red_led, LOW);
    digitalWrite(green_led, HIGH);

    lcd.clear();
    lcd.print("Correcto");
    open_door();
  }
  else {
    lcd.clear();
    lcd.print("Incorrecto");
    digitalWrite(green_led, LOW);
    digitalWrite(red_led, HIGH);
  }
}

void resetDigits(){
  password_buffer = 0;
  digits = 0;
}

void enter(){
  Serial.print("Enter ");
  Serial.println(password_buffer);

  check_password(password_buffer);
  resetDigits();
}

void keypadEvent(KeypadEvent key){
  switch (kpd.getState()){
    case PRESSED:
      switch(key) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          newDigit(key-'0');
          break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case '*':
        case '#':
          enter();
          break;
      }
      break;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(back_led, OUTPUT);

  kpd.addEventListener(keypadEvent);
  interrupts();
  lcd.begin(16,2);
  door_servo.attach(servo_pin);
  close_door();
}

void area_detection(int distance_mm){
  int hl = (distance_mm < 30*10) ? HIGH : LOW;

  if(hl == LOW){
    lcd.clear();
    digitalWrite(green_led, LOW);
    digitalWrite(red_led, LOW);
    resetDigits();
    close_door();
  }
  else {
    kpd.getKey();
  }
  
  digitalWrite(yellow_led, hl);
  digitalWrite(back_led, hl);
}

void loop() {
  int distance = hcsr04.distanceInMillimeters();

  area_detection(distance);
  if(door_open && millis() >= last_door_opening+10*1000){
    close_door();
  }
}
