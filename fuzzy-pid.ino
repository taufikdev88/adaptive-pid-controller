/**
 * Matrix Adaptive PID atau Adaptive PD bisa dilihat di folder **
 * FILE: "data\Matrix Min Max.xlsx"
 * 
 * fuzzy-pid
 * Edited by:   H. Taufik
 */ 
#define adaptive_pid // gunakan antara adaptive_pd dan adaptive_pid
#define mode4 // jika menggunakan adaptive_pid, wajib menggunakan salah satu mode1 / mode2 / mode3 / mode4

float _and(float a1, float a2) // fungsi min
{
  return (a1 < a2 ? a1 : a2);
}

float _or(float a1, float a2) // fungsi max  
{
  return (a1 > a2 ? a1 : a2);
}

float MF(float i, float a, float b, float c) // fungsi fuzzifikasi (convert crisp value to fuzzy value)
{
  if (i < a || i > c) return 0;
  else if (i < b) return (i - a) / (b - a);
  else return (c - i) / (c - b);
}

float Fuzzy(float e) // fungsi Fuzzy 
{
  static float LastError = 0;
  float de;                                             // delta error
  float ie;                                             // integral error
  float oNH, oNB, oNM, oNS, oZ, oPS, oPM, oPB, oPH;     // output Value, sebelum di defuzzifikasi
  float Out;                                            // Crisp value, output setelah di defuzzifikasi

  #define pePB 900                                        // point Error Positive Big
  #define pePM 50                                         // point Error Positive Medium
  #define pePS 10                                         // point Error Positive Small
  #define peZ  0                                          // point Error Zero
  #define peNS -10                                        // point Error Negative Small
  #define peNM -50                                        // point Error Negative Medium
  #define peNB -900                                       // point Error Negative Big
  
  #define pdPB 1                                          // point Delta Error Positive Big
  #define pdPM 0.5                                        // point Delta Error Positive Medium
  #define pdPS 0.25                                       // point Delta Error Positive Small
  #define pdZ  0                                          // point Delta Error Zero
  #define pdNS -0.25                                      // point Delta Error Negative Small
  #define pdNM -0.5                                       // point Delta Error Negative Medium
  #define pdNB -1                                         // point Delta Error Negative Big
  
  #define piPB 0.1                                        // point Integral Error Positive Big
  #define piPM 0.05                                       // point Integral Error Positive Medium
  #define piPS 0.025                                      // point Integral Error Positive Small
  #define piZ  0                                          // point Integral Error Zero
  #define piNS -0.025                                     // point Integral Error Negative Small
  #define piNM -0.05                                      // point Integral Error Negative Medium
  #define piNB -0.1                                       // point Integral Error Negative Big
  
  #define poPH 20                                        // point of Output Positive Huge tone
  #define poPB 15                                        // point of Output Positive Big tone
  #define poPM 10                                        // point of Output Positive Medium tone
  #define poPS 5                                         // point of Output Positive Small tone
  #define poZ 0                                           // point of Output Zero tone
  #define poNS -5                                        // point of Output Negative Small tone
  #define poNM -10                                       // point of Output Negative Medium tone
  #define poNB -15                                       // point of Output Negative Big tone
  #define poNH -20                                       // point of Output Negative Huge tone

                                                         // fungsi PID 
  de = e - LastError;                                    
  LastError = e;
  ie += e;
  
  float eP[7], dP[7], iP[7]; // penyimpanan ePB sampai eNB diubah ke bentuk array, agar perhitungan lebih gampang
  float temp[7][7][7];       // array penyimpanan sementara hasil fungsi MIN/OR
                                              
                                                          // calculate fuzzy value using fuzzyfication based on membership function
                                                          // calculate membership function for error signal, delta error and integral error
  eP[0] = MF(e, pePM, pePB, 1e38);                        // Fungsi keanggotaan Positive Big untuk Error
  eP[1] = MF(e, pePS, pePM, pePB);                        // Fungsi keanggotaan Positive Medium untuk Error
  eP[2] = MF(e, peZ, pePS, pePM);                         // Fungsi keanggotaan Positive Small untuk Error
  eP[3] = MF(e, peNS, peZ, pePS);                          // Fungsi keanggotaan Zero untuk Error
  eP[4] = MF(e, peNM, peNS, peZ);                         // Fungsi keanggotaan Negative Small untuk Error
  eP[5] = MF(e, peNB, peNM, peNS);                        // Fungsi keanggotaan Negative Medium untuk Error
  eP[6] = MF(e, -1e38, peNB, peNM);                       // Fungsi keanggotaan Negative Big untuk Error

  dP[0] = MF(de, pdPM, pdPB, 1e38);                       // Fungsi keanggotaan Positive Big untuk Delta Error
  dP[1] = MF(de, pdPS, pdPM, pdPB);                       // Fungsi keanggotaan Positive Medium untuk Delta Error
  dP[2] = MF(de, pdZ, pdPS, pdPM);                        // Fungsi keanggotaan Positive Small untuk Delta Error
  dP[3] = MF(de, pdNS, pdZ, pdPS);                         // Fungsi keanggotaan Zero untuk Delta Error
  dP[4] = MF(de, pdNM, pdNS, pdZ);                        // Fungsi keanggotaan Negative Small untuk Delta Error
  dP[5] = MF(de, pdNB, pdNM, pdNS);                       // Fungsi keanggotaan Negative Medium untuk Delta Error
  dP[6] = MF(de, -1e38, pdNB, pdNM);                      // Fungsi keanggotaan Negative Big untuk Delta Error

  iP[0] = MF(ie, piPM, piPB, 1e38);                       // Fungsi keanggotaan Positive Big untuk Delta Error
  iP[1] = MF(ie, piPS, piPM, piPB);                       // Fungsi keanggotaan Positive Medium untuk Delta Error
  iP[2] = MF(ie, piZ, piPS, piPM);                        // Fungsi keanggotaan Positive Small untuk Delta Error
  iP[3] = MF(ie, piNS, piZ, piPS);                         // Fungsi keanggotaan Zero untuk Delta Error
  iP[4] = MF(ie, piNM, piNS, piZ);                        // Fungsi keanggotaan Negative Small untuk Delta Error
  iP[5] = MF(ie, piNB, piNM, piNS);                       // Fungsi keanggotaan Negative Medium untuk Delta Error
  iP[6] = MF(ie, -1e38, piNB, piNM);                      // Fungsi keanggotaan Negative Big untuk Delta Error

                                                        // do inference engine using IF-THEN rule base
  // or = fungsi min, and = fungsi max
  // ini adalah fungsi jika yg dipilih adaptive_pd
  #if defined(adaptive_pd)
  oPH = _or(_and(eP[0], dP[2]), _or(_and(eP[1], dP[1]), _or(_and(eP[2], dP[0]), _or(_and(eP[0], dP[1]), _or(_and(eP[1], dP[0]), _and(eP[0], dP[0])))))); 
  oPB = _or(_and(eP[0], dP[3]), _or(_and(eP[1], dP[2]), _or(_and(eP[2], dP[1]), _and(eP[3], dP[0])))); 
  oPM = _or(_and(eP[0], dP[4]), _or(_and(eP[1], dP[3]), _or(_and(eP[2], dP[2]), _or(_and(eP[3], dP[1]), _and(eP[4], dP[0]))))); 
  oPS = _or(_and(eP[0], dP[5]), _or(_and(eP[1], dP[4]), _or(_and(eP[2], dP[3]), _or(_and(eP[3], dP[2]), _or(_and(eP[4], dP[1]), _and(eP[5], dP[0])))))); 
  oZ  = _or(_and(eP[0], dP[6]), _or(_and(eP[1], dP[5]), _or(_and(eP[2], dP[4]), _or(_and(eP[3], dP[3]), _or(_and(eP[4], dP[2]), _or(_and(eP[5], dP[1]), _and(eP[6], dP[0]))))))); 
  oNS = _or(_and(eP[1], dP[6]), _or(_and(eP[2], dP[5]), _or(_and(eP[3], dP[4]), _or(_and(eP[4], dP[3]), _or(_and(eP[5], dP[2]), _and(eP[6], dP[1])))))); 
  oNM = _or(_and(eP[2], dP[6]), _or(_and(eP[3], dP[5]), _or(_and(eP[4], dP[4]), _or(_and(eP[5], dP[3]), _and(eP[6], dP[0]))))); 
  oNB = _or(_and(eP[3], dP[6]), _or(_and(eP[4], dP[5]), _or(_and(eP[5], dP[4]), (_and(eP[6], dP[3]))))); 
  oNH = _or(_and(eP[4], dP[6]), _or(_and(eP[5], dP[5]), _or(_and(eP[6], dP[4]), _or(_and(eP[5], dP[6]), _or(_and(eP[6], dP[5]), _and(eP[6], dP[6]))))));
  #endif
  // ini adalah fungsi jika yang dipilih adaptive_pid
  #if defined(adaptive_pid)
  // mencari nilai max/_and dari e, de, dan ie lalu di simpan ke temp
  for(int i=0; i<7; i++){
      for(int j=0; j<7; j++){
          for(int k=0; k<7; k++){
              temp[i][j][k] = max(eP[i], max(dP[j], iP[k]));
          }
      }
  }
  
  // mencari nilai min/_or berdasarkan mode yang dipilih
  for(int i=0; i<7; i++){
      for(int j=0; j<7; j++){
          for(int k=0; k<7; k++){
              #if defined(mode1)
              if(i+j <= 2) oPH = min(oPH, temp[i][j][k]);
              if(i+j == 3 || i+j == 4) oPB = min(oPB, temp[i][j][k]);
              if(i+j == 5 || i+j == 6) oPM = min(oPM, temp[i][j][k]);
              if(i+j == 7 || i+j == 8) oPS = min(oPS, temp[i][j][k]);
              if(i+j == 9) oZ = min(oZ, temp[i][j][k]);
              if(i+j == 10 || i+j == 11) oNS = min(oNS, temp[i][j][k]);
              if(i+j == 12 || i+j == 13) oNM = min(oNM, temp[i][j][k]);
              if(i+j == 14 || i+j == 15) oNB = min(oNB, temp[i][j][k]);
              if(i+j >= 13) oNH = min(oNH, temp[i][j][k]);
              #endif
              #if defined(mode2)
              if(i+j <= 2) oPH = min(oPH, temp[i][j][k]);
              if(i+j >=3 && i+j <= 5) oPB = min(oPB, temp[i][j][k]);
              if(i+j == 6 || i+j == 7) oPM = min(oPM, temp[i][j][k]);
              if(i+j == 8) oPS = min(oPS, temp[i][j][k]);
              if(i+j == 9) oZ = min(oZ, temp[i][j][k]);
              if(i+j == 10) oNS = min(oNS, temp[i][j][k]);
              if(i+j == 11 || i+j == 12) oNM = min(oNM, temp[i][j][k]);
              if(i+j >= 13 && i+j <= 15) oNB = min(oNB, temp[i][j][k]);
              if(i+j >= 16) oNH = min(oNH, temp[i][j][k]);
              #endif
              #if defined(mode3)
              if(i+j <= 3) oPH = min(oPH, temp[i][j][k]);
              if(i+j == 4 || i+j == 5) oPB = min(oPB, temp[i][j][k]);
              if(i+j == 6 || i+j == 7) oPM = min(oPM, temp[i][j][k]);
              if(i+j == 8) oPS = min(oPS, temp[i][j][k]);
              if(i+j == 9) oZ = min(oZ, temp[i][j][k]);
              if(i+j == 10) oNS = min(oNS, temp[i][j][k]);
              if(i+j == 11 || i+j == 12) oNM = min(oNM, temp[i][j][k]);
              if(i+j == 13 || i+j == 14) oNB = min(oNB, temp[i][j][k]);
              if(i+j >= 15) oNH = min(oNH, temp[i][j][k]);
              #endif
              #if defined(mode4)
              if(i+j <= 5) oPH = min(oPH, temp[i][j][k]);
              if(i+j == 6) oPB = min(oPB, temp[i][j][k]);
              if(i+j == 7) oPM = min(oPM, temp[i][j][k]);
              if(i+j == 8) oPS = min(oPS, temp[i][j][k]);
              if(i+j == 9) oZ = min(oZ, temp[i][j][k]);
              if(i+j == 10) oNS = min(oNS, temp[i][j][k]);
              if(i+j == 11) oNM = min(oNM, temp[i][j][k]);
              if(i+j == 12) oNB = min(oNB, temp[i][j][k]);
              if(i+j >= 13) oNH = min(oNH, temp[i][j][k]);
              #endif
          }
      }
  }
  #endif
                                                        // calculate crisp value using defuzzyfication
  Out = (oNH * poNH + oNB * poNB + oNM * poNM + oNS * poNS + oZ * poZ + oPS * poPS + oPM * poPM + oPB * poPB + oPH * poPH) / (oNH + oNB + oNM + oNS + oZ + oPS + oPM + oPB + oPH);
  return Out;
}
