#define lighttime 10 // menit
#define mediumtime 16 // menit
#define darktime 30 // menit

unsigned long int timer = 0;
int temp = 230; // derajat celcius
float airTemp = 0, coffeeTemp = 0;
int time = 0;
float output;
uint8_t slide = 0;

void showTime(unsigned long timeleft){
  lcd.setCursor(0,0);
  switch(slide){
    case 0:
    if(timeleft % 1000 == 0){
      lcd.print("RUNNING...");
      lcd.setCursor(10,0);
      lcd.print(String(airTemp) + "C");
      lcd.setCursor(0,1);
      lcd.print("timelf:         ");
      lcd.setCursor(7,1);
      lcd.print(String(timeleft));
    }
    break;
    case 1:
    lcd.print("Setpoint Suhu   ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print(String(temp));
    break;
    case 2:
    lcd.print("Setpoint waktu  ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print(String(time));
    break;
  }
}

void run(){
  timer = millis();
  switch(mode.getNum()){
    case 0:
    time = lighttime*60000;
    break;
    case 1:
    time = mediumtime*60000;
    break;
    case 2:
    time = darktime*60000;
    break;
  }
  
  while(millis() - timer <= time){
    // show information to user
    showTime(millis()-timer);
    
    // get instruction from user
    if(isBtnUp()){
      if(slide == 0) slide = 2;
      else slide--;
    }
    if(isBtnDw()){
      if(slide == 2) slide = 0;
      else slide++;
    }
    if(isBtnSel()){
      taskNow = menu;
      break;
    }
    if(isBtnRt()){
      if(slide == 1) temp++; // menambah suhu 1 derajat
      if(slide == 2) time+=60000; // menambah waktu 1 menit
    }
    if(isBtnLf()){
      if(slide == 1) temp--; // mengurangi suhu 1 derajat
      if(slide == 2) time-=60000; // mengurangi waktu 1 menit
    }
    
    // getReal Data
    airTemp = TCA.readCelsius();
    coffeeTemp = TCB.readCelsius();
    
    airTemp = -0.0004 * airTemp * airTemp + 1.4276 * airTemp - 27.407; // konversi nilai suhu
    coffeeTemp = -0.0004 * coffeeTemp * coffeeTemp + 1.4276 * coffeeTemp - 27.407; // konversi nilai suhu

    // getFuzzy output
    output = Fuzzy(temp - airTemp);
    output = constrain(output, 0, 90);

    servo.write(output);
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SELESAI");
  taskNow = menu;
}
