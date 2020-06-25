#define lighttimeout 10 // menit
#define mediumtimeout 16 // menit
#define darktimeout 30 // menit

unsigned long timestart = 0;
int temp = 230; // derajat celcius
float airTemp1 = 0, airTemp2 = 0, tempAct = 0;
unsigned long timeout = 0;
int8_t slide = 0;
float output;

unsigned long delayLCDRun = millis();
void showtimeout(unsigned long timeleft){
  if(millis() - delayLCDRun < 500) return;
  delayLCDRun = millis();
  
  lcd.setCursor(0,0);
  switch(slide){
    case 0:
    lcdLine0("RUNNING...");
    lcd.print(String(readTemp()) + "C");
    lcd.setCursor(0,1);
    lcd.print("timeleft:    min");
    lcd.setCursor(10,1);
    lcd.print((long) timeleft / 60000);
    break;
    case 1:
    lcd.print("Setpoint Suhu   ");
    lcd.setCursor(0,1);
    lcd.print("        Celcius ");
    lcd.setCursor(0,1);
    lcd.print(temp);
    break;
    case 2:
    lcd.print("Setpoint waktu  ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print((long) timeout / 60000);
    break;
  }
}

void run(){
  timestart = millis();
  timeout = 0;
  slide = 0;
  
  switch(mode.getNum()){
    case 0:
    timeout = 60000 * lighttimeout;
    break;
    case 1:
    timeout = 60000 * mediumtimeout;
    break;
    case 2:
    timeout = 60000 * darktimeout;
    break;
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Jangan masukkan ");
  lcd.setCursor(0,1);
  lcd.print("Kopi, suhu:     ");
  while(tempAct < 200.0){
    if(millis() - delayLCDRun >= 500){
      delayLCDRun = millis();
      lcd.setCursor(12,1);
      lcd.print(tempAct);
    }

    tempAct = readTemp();
    // getFuzzy output
    output = Fuzzy(200 - tempAct);
    output = constrain(output, 0, 90);
    servo.write(output);

    Serial.print(200);
    Serial.print('\t');
    Serial.print(tempAct);
    Serial.print('\t');
    Serial.print(output);
    Serial.println();
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Masukkan Kopi  ");
  lcd.setCursor(0,1);
  lcd.print("Klik untuk Mulai");
  delayButton = millis();
  while(!isBtnSel() || (unsigned long) millis() - delayButton < 1500);
  resetButton;
  
  while((unsigned long) millis() - timestart < timeout){  
    // show information to user
    showtimeout((unsigned long) timeout - (millis()-timestart));
    
    // get instruction from user
    bool buttonReady = ((unsigned long) millis() - delayButton > 500);
    if(isBtnUp() && buttonReady){
      resetButton;
      if(--slide < 0) slide = 2;
    }
    if(isBtnDw() && buttonReady){
      resetButton;
      if(++slide > 2) slide = 0;
    }
    if(isBtnSel() && buttonReady){
      resetButton;
      break;
    }
    if(isBtnRt() && buttonReady){
      resetButton;
      if(slide == 1) temp++; // menambah suhu 1 derajat
      if(slide == 2) timeout+=60000; // menambah waktu 1 menit
    }
    if(isBtnLf() && buttonReady){
      resetButton;
      if(slide == 1) temp--; // mengurangi suhu 1 derajat
      if(slide == 2) timeout-=60000; // mengurangi waktu 1 menit
    }
    
    // getFuzzy output
    output = Fuzzy(temp - readTemp());
    output = constrain(output, 0, 90);
    servo.write(output);
  }

  servo.write(defaultservoangle);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SELESAI");
  taskNow = menu;
}

float readTemp(){
    // getReal Data
    airTemp1 = TCA.readCelsius();
    airTemp2 = TCB.readCelsius();
    
    airTemp1 = -0.0004 * airTemp1 * airTemp1 + 1.4276 * airTemp1 - 27.407; // konversi nilai suhu
    airTemp2 = -0.0004 * airTemp2 * airTemp2 + 1.4276 * airTemp2 - 27.407; // konversi nilai suhu

    return (airTemp1+airTemp2)/2.0;
}
