/**
 * Program Coffee maker, based on DFROBOT MICROCONTROLLER
 * feature: adaptive pd controller and adaptive pid controller
 * 
 * Created by:  H. Taufik
 */
 
#include <Servo.h> 
#include <Wire.h> 
//#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include "max6675.h"

#define analog_button // pilihan antara analog_button dan digital_button

// start of pins declaration
#define serPin 10    // Servo
#define relay1 11    // Relay
#define relay2 12
#define btnRT 0     // Button
#define btnUP 1 
#define btnDW 2
#define btnLF 3
#define btnSEL 4
#define btnNONE 5
#define TCASO 13    // Thermocouple A
#define TCACS 14    
#define TCACLK 15
#define TCBSO 16    // Thermocouple B
#define TCBCS 17
#define TCBCLK 18
#define rs 8        // LCD
#define en 9   
#define d4 4    
#define d5 5    
#define d6 6    
#define d7 7    
// end of pins declaration

//// Set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// initialize an instance of class Servo
Servo servo;
#define defaultservoangle 0

// initialize an instance of class MAX6675
MAX6675 TCA(TCACLK, TCACS, TCASO);
MAX6675 TCB(TCBCLK, TCBCS, TCBSO);

class modes {
  #define modelength 3
  public:
  uint8_t num = 0;
  String names[modelength] = {"light ", "medium", "dark  "};

  modes(){
    num = 0;
  }

  uint8_t getNum(){
    return num;
  }

  String getName(){
    return names[num];
  }

  void shiftRight(){
    if(num == (modelength-1)) num = 0;
    else num++;
  }

  void shiftLeft(){
    if(num == 0) num = modelength-1;
    else num--;
  }
};
modes mode;

enum step {
  menu,
  setting,
  running
};

void setup()
{  
  servo.attach(serPin);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  #if defined(digital_button)
  pinMode(btnSEL, INPUT_PULLUP);
  pinMode(btnLF, INPUT_PULLUP);
  pinMode(btnUP, INPUT_PULLUP);
  pinMode(btnRT, INPUT_PULLUP);
  pinMode(btnDW, INPUT_PULLUP);
  #endif
  
  lcd.begin(16,2);  // lcd non i2c begin function
//  lcd.begin();    // lcd i2c begin function
//  lcd.backlight(); // lcd i2c function
  lcd.setCursor(0,0);
  lcd.print("COFFEE MAKER");
  lcd.setCursor(0,1);
  lcd.print("MASBUKHIN");
  delay(3000);
  lcd.clear();
}

step taskNow = menu;
void loop()
{
  // standby mode choose menu
  while(taskNow == menu){
    // show information to user
    lcd.setCursor(0,0);
    lcd.print("   PILIH MENU   ");
    lcd.setCursor(0,1);
    lcd.print(mode.getName());
    lcd.setCursor(15,1);
    lcd.print("<>");
    
    // get instruction from user
    if(isBtnRt()) mode.shiftRight();
    if(isBtnLf()) mode.shiftLeft();
    if(isBtnSel()){
      taskNow = running;
      digitalWrite(relay1, 0);
      digitalWrite(relay2, 0);
      lcd.clear();
    }
    
    // set default configuration
    digitalWrite(relay1, 1);
    digitalWrite(relay2, 1);
    servo.write(defaultservoangle);
  }

  // running mode
  while(taskNow == running){
    run();
//    Fuzzy(12);
  }
}
