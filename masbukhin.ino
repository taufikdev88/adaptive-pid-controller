
/**
 * Program Coffee maker, based on DFROBOT MICROCONTROLLER
 * feature: adaptive pd controller and adaptive pid controller
 * 
 * Created by:  H. Taufik
 */
#include <TimerOne.h>
#include <Servo.h> 
#include <Wire.h> 
#include <LiquidCrystal.h>
#include "max6675.h"


#define analog_button // pilihan antara analog_button dan digital_button

// start of pins declaration
#define serPin 46    // Servo
#define relay 44    // Relay

// jika analog, maka ini digunakan sebagai identifikasi button saja bukan sebagai pinout
#define btnRT 0     // Button
#define btnUP 1 
#define btnDW 2
#define btnLF 3
#define btnSEL 4
#define btnNONE 5

#define TCASO 32    // Thermocouple A
#define TCACS 34    
#define TCACLK 36
#define TCBSO 38    // Thermocouple B
#define TCBCS 40
#define TCBCLK 42

#define lcdbl 10
#define rs 8        // LCD
#define en 9   
#define d4 4    
#define d5 5    
#define d6 6    
#define d7 7    
// end of pins declaration

//// Set the LCD address to 0x27 for a 16 chars and 2 line 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// make a cute degree symbol
uint8_t degree[8]  = {140,146,146,140,128,128,128,128};

// initialize an instance of class Servo
Servo servo;
#define defaultangle 50 // default valve menutup
#define startupangle 40 // default valve minimal agar gas masuk untuk penyalaan awal

// initialize an instance of class MAX6675
MAX6675 TCA(TCACLK, TCACS, TCASO);
MAX6675 TCB(TCBCLK, TCBCS, TCBSO);
float airTemp1 = 0.0, airTemp2 = 0.0, tempAct = 0.0;

#define relayOn() digitalWrite(relay,1)
#define relayOff() digitalWrite(relay,0)

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
unsigned long delayButton = millis();
#define resetButton delayButton=millis()

void setup()
{  
  pinMode(relay, OUTPUT);
  relayOff();
  servo.attach(serPin);
  servo.write(defaultangle);
  
  Serial.begin(9600);

  pinMode(lcdbl, OUTPUT);
  digitalWrite(lcdbl, 1);
  #if defined(digital_button)
  pinMode(btnSEL, INPUT_PULLUP);
  pinMode(btnLF, INPUT_PULLUP);
  pinMode(btnUP, INPUT_PULLUP);
  pinMode(btnRT, INPUT_PULLUP);
  pinMode(btnDW, INPUT_PULLUP);
  #endif
  
  lcd.begin(16,2);  // lcd non i2c begin
  lcd.createChar(0, degree);
  lcd.setCursor(0,0);
  lcd.print("COFFEE MAKER");
  lcd.setCursor(0,1);
  lcd.print("MASBUKHIN");
  delay(1000);
  lcd.clear();

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(readTemp);
}

step taskNow = menu;
void loop()
{ 
  // program start here
  relayOff();
  servo.write(defaultangle);
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
