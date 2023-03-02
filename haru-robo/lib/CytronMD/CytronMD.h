class CytronMD {
 public:
  CytronMD(byte Pwm, byte Dir);
  void motor(char speed);

 protected:
  byte pwm;
  byte dir;
};
