#define lighttimeout 10 // menit
#define mediumtimeout 16 // menit
#define darktimeout 30 // menit

unsigned long timestart = 0;
int temp = 230; // derajat celcius
unsigned long timeout = 0;
int8_t slide = 0;
int output;

unsigned long delayLCDRun = millis();
void showtimeleft(unsigned long timeleft){
  if(millis() - delayLCDRun < 500) return;
  delayLCDRun = millis();
  timeout = timeleft / 60000;
  
  lcd.setCursor(0,0);
  switch(slide){
    case 0:
    lcdLine0("RUNNING..");
    lcd.print(tempAct);
    derajat();
    lcd.setCursor(0,1);
    lcd.print("timeleft:    min");
    lcd.setCursor(10,1);
    lcd.print((long) timeleft / 60000);
    break;
    case 1:
    lcd.print("Setpoint Suhu   ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print(temp);
    derajat();
    break;
    case 2:
    lcd.print("Setpoint waktu  ");
    lcd.setCursor(0,1);
    lcd.print("         Menit  ");
    lcd.setCursor(0,1);
    lcd.print((long) timeout / 60000);
    break;
  }
}

void run(){
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

//  Serial.println(tempAct);
//  return;

  servo.write(0);
  delay(500);
  relayOn();
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
      derajat();
    }
    
    // getFuzzy output
    output = Fuzzy(200 - tempAct);
    output = constrain(output, 0, 90);
    servo.write(startupangle-output);

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
  
  timestart = millis();
  while((unsigned long) millis() - timestart < timeout){  
    // show information to user
    showtimeleft((unsigned long) timeout - (millis()-timestart));
    
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
    output = Fuzzy(temp - tempAct);
    output = constrain(output, 0, 90);
    servo.write(startupangle-output);
    
    Serial.print(230);
    Serial.print('\t');
    Serial.print(tempAct);
    Serial.print('\t');
    Serial.print(output);
    Serial.println();
  }

  relayOff();
  servo.write(defaultangle);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SELESAI");
  delay(2000);
  taskNow = menu;
}

float readTemp(){
  // getReal Data
  airTemp1 = TCA.readCelsius();
  airTemp2 = TCB.readCelsius();
  if(isnan(airTemp1) || isnan(airTemp2)) return 0;
  airTemp1 = -0.0004 * airTemp1 * airTemp1 + 1.4276 * airTemp1 - 27.407; // konversi nilai suhu
  airTemp2 = -0.0004 * airTemp2 * airTemp2 + 1.4276 * airTemp2 - 27.407; // konversi nilai suhu
  tempAct = (airTemp1+airTemp2)/2.0;
  return (airTemp1+airTemp2)/2.0;
}

void derajat(){
#if ARDUINO >= 100
  lcd.write((byte)0);
#else
  lcd.print(0, BYTE);
#endif
  lcd.print("C ");
}

void lcdLine0(String abc){
    lcd.setCursor(0,0);
    lcd.print(abc);
}

void lcdLine1(String abc){
    lcd.setCursor(0,1);
    lcd.print(abc);
}
