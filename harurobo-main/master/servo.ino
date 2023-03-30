#define minUs 500
#define maxUs 2400
#define servo_hz 50


void servo_attach() {
  servo1.setPeriodHertz(servo_hz);  // Standard 50hz servo
  servo1.attach(servo_1, minUs, maxUs);
  servo2.setPeriodHertz(servo_hz);  // Standard 50hz servo
  servo2.attach(servo_2, minUs, maxUs);
}

void servo_detach() {
  servo1.detach();
  servo2.detach();
}

void servo_open() {
  servo_attach();
  servo1.write(135);
  servo2.write(0);
  delay(740);  // waits 20ms for the servo to reach the position
  servo_detach();
}
void servo_close() {
  servo_attach();
  servo1.write(0);
  servo2.write(135);
  delay(740);  // waits 20ms for the servo to reach the position
  servo_detach();
}
