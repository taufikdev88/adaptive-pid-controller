#if defined(digital_button)
bool isBtnSel(){
  if(digitalRead(btnSEL) == 0){
    while(digitalRead(btnSEL) == 0);
    return 1;
  }
  return 0;
}

bool isBtnLf(){
  if(digitalRead(btnLF) == 0){
    while(digitalRead(btnLF) == 0);
    return 1;
  }
  return 0;
}

bool isBtnRt(){
  if(digitalRead(btnRT) == 0){
    while(digitalRead(btnRT) == 0);
    return 1;
  }
  return 0;
}

bool isBtnUp(){
  if(digitalRead(btnUP) == 0){
    while(digitalRead(btnUP) == 0);
    return 1;
  }
  return 0;
}

bool isBtnDw(){
  if(digitalRead(btnDW) == 0){
    while(digitalRead(btnDW) == 0);
    return 1;
  }
  return 0;
}
#endif
#if defined(analog_button)
/**
 * Based on DFROBOT Module
 */
int adcKeyIn = 0;

uint8_t readButton(){
  adcKeyIn = analogRead(0);
//  // 5 key value: 0,144,329,504,741
//  if(adcKeyIn > 1000) return btnNONE;
//  if(adcKeyIn < 50) return btnRT;
//  if(adcKeyIn < 150) return btnUP;
//  if(adcKeyIn < 300) return btnDW;
//  if(adcKeyIn < 500) return btnLF;
//  if(adcKeyIn < 850) return btnSEL;

  // V1.0 Use the following version threshold:
  if(adcKeyIn < 50) return btnRT;
  if(adcKeyIn < 195) return btnUP;
  if(adcKeyIn < 350) return btnDW;
  if(adcKeyIn < 555) return btnLF;
  if(adcKeyIn < 790) return btnSEL;
  return btnNONE;
}

bool isBtnSel(){
  if(readButton() == btnSEL) return 1;
  else return 0;
}

bool isBtnLf(){
  if(readButton() == btnLF) return 1;
  else return 0;
}

bool isBtnRt(){
  if(readButton() == btnRT) return 1;
  else return 0;
}

bool isBtnUp(){
  if(readButton() == btnUP) return 1;
  else return 0;
}

bool isBtnDw(){
  if(readButton() == btnDW) return 1;
  else return 0;
}
#endif
