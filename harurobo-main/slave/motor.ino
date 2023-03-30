void shot(bool is_on) {
  if (is_on) {
    motor_4.motor(127 * (2 * switch_state[3] - 1));
    delay(1000);
    motor_3.motor(127 * (2 * switch_state[2] - 1));
    delay(1000);
  } else {
    motor_3.motor(0);
    motor_4.motor(0);
  }
}
