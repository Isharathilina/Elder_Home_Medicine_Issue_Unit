#include <LiquidCrystal.h>
const int RS = D2, EN = D3, d4 = D5, d5 = D6, d6 = D7, d7 = D8;   
LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);

void setup() {
                         // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
                      // Print a message to the LCD.
  lcd.print("hello, world!");
 // lcd.noBlink();
  delay(2000);
  // Turn on the blinking cursor:
 // lcd.blink();
 // delay(3000);
   
  lcd.clear();
  //lcd.setCursor(0, 0);

  lcd.print("hi!");
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(300);
  }
    lcd.clear();

 
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);

  double a = 22.0;
  lcd.print("My "+String(a));
}
