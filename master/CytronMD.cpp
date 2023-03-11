#include "CytronMD.h"

#include "Arduino.h"

CytronMD::CytronMD(byte Pwm, byte Dir, byte Ch) {
  pwm = Pwm;
  dir = Dir;
  ch = Ch;
  pinMode(dir, OUTPUT);
  ledcSetup(ch, 10000, 7);  // 10kHz、7bit
  ledcAttachPin(pwm, ch);
}

void CytronMD::motor(int speed) {
  //-127〜127で速度と方向を指定
  if (speed < 0) {
    digitalWrite(dir, 0);
  } else {
    digitalWrite(dir, 1);
  }
  ledcWrite(ch, abs(speed));
}
