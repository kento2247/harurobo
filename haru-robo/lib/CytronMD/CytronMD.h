#include <Arduino.h>

class CytronMD {
 public:
  CytronMD(byte Pwm, byte Dir, byte Ch);
  void motor(int speed);

 protected:
  byte pwm;
  byte dir;
  byte ch;
};
