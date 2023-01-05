
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

// Define the Keymap
char keys[ROWS][COLS] = {

  { '1', '2', '3', 'A' },

  { '4', '5', '6', 'B' },

  { '7', '8', '9', 'C' },

  { '*', '0', '#', 'D' }

};

byte rowPins[ROWS] = { 0, 1, 2, 3 };
byte colPins[COLS] = { 4, 5, 6, 7 };

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal_I2C lcd(0x27, 16, 2);

long Num1, Num2, Number, Ans = 0, PreAns = 0, state = 0;
char key, action;
boolean result = false;

void setup() {
  lcd.init();
  lcd.print("Calculator Ready");
  lcd.setCursor(0, 1);
  lcd.print("A=+ B=- C=* D=/");

  delay(1000);
  lcd.clear();
}

void loop() {

  key = kpd.getKey();

  if (key != NO_KEY)
    DetectButtons();

  if (result == true) {
    calculateResult();
  }
  displayResult();
  answer();
}

void DetectButtons() {
  state = 3;
  lcd.clear();
  if (key == '*') {
    Serial.println("Button Cancel");
    Number = Num1 = Num2 = 0;
    result = false;
  }

  if (key == '1') {
    Serial.println("Button 1");
    if (Number == 0)
      Number = 1;
    else
      Number = (Number * 10) + 1; 
  }

  if (key == '4') 
  {
    Serial.println("Button 4");
    if (Number == 0)
      Number = 4;
    else
      Number = (Number * 10) + 4;  
  }

  if (key == '7')  
  {
    Serial.println("Button 7");
    if (Number == 0)
      Number = 7;
    else
      Number = (Number * 10) + 7; 
  }


  if (key == '0') {
    Serial.println("Button 0"); 
    if (Number == 0)
      Number = 0;
    else
      Number = (Number * 10) + 0;  
  }

  if (key == '2')  
  {
    Serial.println("Button 2");
    if (Number == 0)
      Number = 2;
    else
      Number = (Number * 10) + 2;  
  }

  if (key == '5') {
    Serial.println("Button 5");
    if (Number == 0)
      Number = 5;
    else
      Number = (Number * 10) + 5;  
  }

  if (key == '8') {
    Serial.println("Button 8");
    if (Number == 0)
      Number = 8;
    else
      Number = (Number * 10) + 8; 
  }


  if (key == '#') {
    Serial.println("Button Equal");
    Num2 = Number;
    result = true;
    state = 0;
  }

  if (key == 'C') {
    Serial.println("Ans");
    state = 1;
  }

  if (key == 'D') {
    Serial.println("PreAns");
    state = 2;
  }

  if (key == '3') {
    Serial.println("Button 3");
    if (Number == 0)
      Number = 3;
    else
      Number = (Number * 10) + 3;  
  }

  if (key == '6') {
    Serial.println("Button 6");
    if (Number == 0)
      Number = 6;
    else
      Number = (Number * 10) + 6;  
  }

  if (key == '9') {
    Serial.println("Button 9");
    if (Number == 0)
      Number = 9;
    else
      Number = (Number * 10) + 9; 
  }

  if (key == 'A' || key == 'B')  
  {
    Num1 = Number;
    Number = 0;
    if (key == 'A') {
      Serial.println("Addition");
      action = '+';
    }
    if (key == 'B') {
      Serial.println("Subtraction");
      action = '-';
    }
    delay(100);
  }
}

void calculateResult() {
  if (action == '+') {
    Number = Num1 + Num2;
    PreAns = Ans;
    Ans = Number;
  }
  if (action == '-') {
    Number = Num1 - Num2;
    PreAns = Ans;
    Ans = Number;
  }
  result = false;
}

void displayResult() {
  if (state != 1 && state != 2) {
    lcd.setCursor(0, 0);  
    lcd.print(Num1);
    lcd.print(action);
    lcd.print(Num2);

    if (result == true) {
      if (state == 0) {
        lcd.print(" =");
        lcd.print(Number); 
      }
    }

    lcd.setCursor(0, 1);  
    lcd.print(Number);    
  }
}
void answer() {
  if (state == 1) {

    lcd.setCursor(0, 0);
    lcd.print("Ans");
    lcd.setCursor(0, 1);
    
    lcd.print(Ans);
    
  }  
  if (state == 2) {
    lcd.setCursor(0, 0);
    lcd.print("Ans");
    lcd.setCursor(0, 1);
    
    lcd.print(PreAns);
  }  
}
