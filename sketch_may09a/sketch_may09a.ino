#include <LiquidCrystal.h>

#include <hcsr04.h>
#include <Keypad.h>

const int red_led = 25;
const int green_led = 27;
const int yellow_led = 29;

const int back_led = 39;

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

int password_buffer = 0;
int digits = 0;
int correct_password = 1234;

void newDigit(int digit){
  password_buffer = password_buffer*10+digit;
  Serial.println(digit);

  if(digits++ == 0)
    lcd.clear();

  lcd.print("*");
}

void check_password(int password){
  if(password == correct_password){
    digitalWrite(red_led, LOW);
    digitalWrite(green_led, HIGH);

    lcd.clear();
    lcd.print("Correcto");
  }
  else {
    lcd.clear();
    lcd.print("Incorrecto");
    digitalWrite(green_led, LOW);
    digitalWrite(red_led, HIGH);
  }
}

void enter(){
  Serial.print("Enter ");
  Serial.println(password_buffer);

  check_password(password_buffer);

  password_buffer = 0;
  digits = 0;
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
}

void area_detection(int distance_mm){
  int hl = (distance_mm < 30*10) ? HIGH : LOW;

  if(hl == LOW){
    lcd.clear();
    digitalWrite(green_led, LOW);
    digitalWrite(red_led, LOW);
  }
  else {
    kpd.getKey();
  }
  
  digitalWrite(yellow_led, hl);
  digitalWrite(back_led, hl);
}

void loop() {
  int distance = hcsr04.distanceInMillimeters();

  //Serial.println(distance);
  
  area_detection(distance);
}
