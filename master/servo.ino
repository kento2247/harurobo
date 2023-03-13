void servo_init() {
  servo1.setPeriodHertz(servo_hz);      // Standard 50hz servo
  servo1.attach(servo_1, minUs, maxUs);
  servo2.setPeriodHertz(servo_hz);      // Standard 50hz servo
  servo2.attach(servo_2, minUs, maxUs);
  servo1.write(0);
  servo2.write(0);
}

void demo() {
  servo1.write(180);
  delay(720);             // waits 20ms for the servo to reach the position
  servo1.write(0);
  delay(720);             // waits 20ms for the servo to reach the position
}
