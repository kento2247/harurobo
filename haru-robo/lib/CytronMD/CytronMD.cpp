#include "Arduino.h"
#include "CytronMD.h"

CytronMD::CytronMD(byte Pwm, byte Dir) {
  pwm = Pwm;
  dir = Dir;
  pinMode(dir, OUTPUT);
  ledcSetup(0, 10000, 7);  // 10kHz、7bit
  ledcAttachPin(pwm, 0);
}

void CytronMD::motor(char speed) {
  //-128〜127で速度と方向を指定
  if (speed < 0) {
    digitalWrite(dir, 0);
  } else {
    digitalWrite(dir, 1);
  }
  ledcWrite(0, abs(speed));
}
