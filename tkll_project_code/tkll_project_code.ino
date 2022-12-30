#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>
#include <EEPROM.h>

//defination
const int ROW=4;
const int COL=4;


char keys[ROW][COL] = {
  {'1','2','3','A'},

  {'4','5','6','B'},

  {'7','8','9','C'},

  {'*','0','#','D'}
};
byte rowPins[ROWS] = { 0, 1, 2, 3 };// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte colPins[COLS] = { 4, 5, 6, 7 }; // Connect keypad COL0, COL1 and COL2 to these Arduino pins.
// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, 16, 2);
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROW, COL);
String num1, num2;
long result;
bool currentState = false;
bool finalState = false;
char op;
int temp = 0;
int ptr = 0;
int flag = 0;

//save integer from 0 to 16 bit
void write2Bytes(int address, long number) {
  byte num_4 = (number >> 24) & 0xFF;
  byte num_3 = (number >> 16) & 0xFF;
  byte num_2 = (number >> 8) & 0xFF;
  byte num_1 = number & 0xFF;
  EEPROM.write(address, num_4);
  EEPROM.write(address + 1, num_3);
  EEPROM.write(address + 2, num_2);
  EEPROM.write(address + 3, num_1);
}
long read2Bytes(int address) {
  long num_4 = EEPROM.read(address);
  long num_3 = EEPROM.read(address + 1);
  long num_2 = EEPROM.read(address + 2);
  long num_1 = EEPROM.read(address + 3);
  return ((num_1 << 0) & 0xFF) + ((num_2 << 8) & 0xFFFF) + ((num_3 << 16) & 0xFFFFFF) + ((num_4 << 24) & 0xFFFFFFFF);
}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 0);
  char key = myKeypad.getKey();

  if(key == '=' && num1.length()>1){
    lcd.setCursor(16-num1.length(),1);
    lcd.print(num1);
  }
  if (key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0') {
    if (currentState != true) {
      lcd.setCursor(0, 0);
      num1 = num1 + key;
      int numLen = num1.length();
      lcd.print(num1);
    }
    else {
      num2 = num2 + key;
      int numLen = num2.length();
      lcd.setCursor(num1.length() + 1, 0);
      lcd.print(num2);
      finalState = true;
    }
  }

  if (currentState == false && (key == '+' || key == '*')) {
    if (currentState == false) {
      currentState = true;
      op = key;
      lcd.setCursor(num1.length(), 0);
      lcd.print(op);
    }
  }
  if (finalState == true) {
    if (op == '+') result = num1.toInt() + num2.toInt();
    if (op == '*') result = num1.toInt() * num2.toInt();
    if (key == '=') {
      finalState = false;
      long sav = result;
      if (result == 0) {
        lcd.setCursor(15, 1);
        lcd.print(0);
      } else {
        int cnt = 0;
        while (sav != 0) {
          sav = sav / 10;
          cnt++;
        }
        lcd.setCursor(16 - cnt, 1);
        lcd.print(result);
        lcd.noAutoscroll();
      }
    }
  }
  if (key == '/') {
    if (temp >= 254) temp = 0;
    write2Bytes(temp, result);
    temp += 4;
    ptr = temp - 4;
  }

  if (key == '-') {

    lcd.setCursor(0, 0);
    long value = read2Bytes(ptr);
    if (ptr < 0) {
      value = 0;
    }
    ptr -= 4;
    if (currentState != true) {
      num1 = "";
      num1 = num1 + value;
      lcd.clear();
      int numLen = num1.length();
      lcd.setCursor(0, 0);
      lcd.print(num1);  // first it is value
    } else {
      num2 = "";
      num2 = num2 + value;
      int numLen = num2.length();
      lcd.setCursor(num1.length() + 1, 0);
      finalState = true;
      lcd.print(value);
    }
    if (finalState == true) {
      if (op == '+') result = num1.toInt() + num2.toInt();
      if (op == '*') result = num1.toInt() * num2.toInt();
      if (key == '=') {
        finalState = false;
        long sav = result;
        if (result == 0) {
          lcd.setCursor(15, 1);
          lcd.print(0);
        } else {
          int cnt = 0;
          while (sav != 0) {
            sav = sav / 10;
            cnt++;
          }
          lcd.setCursor(16 - cnt, 1);
          lcd.print(result);
          lcd.noAutoscroll();
        }
      }
    }
  }

  if (key == 'C') {
    lcd.clear();
    currentState = false;
    num1 = "";
    num2 = "";
    result = 0;
    op = ' ';
  }
}
