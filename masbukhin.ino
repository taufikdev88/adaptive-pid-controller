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
#define relay 11    // Relay
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

unsigned long delayUpdateLCD = millis();
void lcdLine0(String abc){
  if((unsigned long) millis() - delayUpdateLCD >= 400){
    delayUpdateLCD = millis();
    lcd.setCursor(0,0);
    lcd.print(abc);
  }
}
void lcdLine1(String abc){
    lcd.setCursor(0,1);
    lcd.print(abc);
}

unsigned long delayButton = millis();
#define resetButton delayButton=millis()

void setup()
{  
  Serial.begin(9600);
  servo.attach(serPin);
  
  pinMode(relay, OUTPUT);
  
  #if defined(digital_button)
  pinMode(btnSEL, INPUT_PULLUP);
  pinMode(btnLF, INPUT_PULLUP);
  pinMode(btnUP, INPUT_PULLUP);
  pinMode(btnRT, INPUT_PULLUP);
  pinMode(btnDW, INPUT_PULLUP);
  #endif
  
  lcd.begin(16,2);  // lcd non i2c begin 
//  lcd.begin();    // lcd i2c begin function
//  lcd.backlight(); // lcd i2c function
  lcd.setCursor(0,0);
  lcd.print("COFFEE MAKER");
  lcd.setCursor(0,1);
  lcd.print("MASBUKHIN");
  delay(1000);
  lcd.clear();
  
  digitalWrite(relay, 0);
  servo.write(defaultservoangle);
}

step taskNow = menu;
void loop()
{
//  for(int i=0; i<=90; i++){
//    servo.write(i);
//    Serial.println((String) "servo angle: " + i);
//    delay(1000);
//  }
//  return;
  
  servo.write(defaultservoangle);
  lcdLine1(mode.getName());
  // standby mode choose menu
  while(taskNow == menu){
    // show information to user
    lcdLine0("   PILIH MENU   ");
    // get instruction from user
    bool buttonReady = ((unsigned long) millis() - delayButton > 500);
    if(isBtnRt() && buttonReady){ mode.shiftRight(); resetButton; lcdLine1(mode.getName()); }
    if(isBtnLf() && buttonReady){ mode.shiftLeft(); resetButton; lcdLine1(mode.getName()); }
    if(isBtnSel() && buttonReady){
      taskNow = running;
      lcd.clear();
      resetButton;
    }
  }

  // running mode
  while(taskNow == running){
    run();
  }
}
