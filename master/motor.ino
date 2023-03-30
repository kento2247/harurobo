// void set_motor_dir() {
//   byte *rev_data = data_receive();
//   if (rev_data != NULL) {
//     if (rev_data[0] == 255) {
//       byte val = rev_data[data_len - 1];
//       for (int i = 0; i < 4; i++) {
//         motor_direction[3 - i] = (val) % 2;
//         val /= 2;
//       }
//     }
//   }
// }


void motor_control(int LY, int LX, int R_X) {
  double rad = atan2(LY, LX);
  double len = sqrt(LX * LX + LY * LY);  // 0~127
  if (len > 127) len = 127;
  int m1_speed = (cos(rad + PI / 4) * len);
  int m2_speed = (sin(rad + PI / 4) * len);
  int m3_speed =  m2_speed;
  int m4_speed =  m1_speed;

  int motor_speed[4] = { m1_speed, m2_speed, m3_speed, m4_speed };
  int turn = R_X;  // 方向転換用
  int turn_step = 0;
  if (turn != 0) {
    if (turn > 0)
      turn_step = 1;
    else
      turn_step = -1;
    bool escape_flag = 0;
    while (1) {
      if (turn == 0) break;
      for (byte i = 0; i < 4; i++) {
        if (abs(motor_speed[i]) >= 127) {
          escape_flag = 1;
          break;
        }
        if(i==0||i==2)motor_speed[i] += turn_step;
        else motor_speed[i] -= turn_step;
      }
      if (escape_flag) break;
      m1_speed = motor_speed[0];
      m2_speed = motor_speed[1];
      m3_speed = motor_speed[2];
      m4_speed = motor_speed[3];
      turn -= turn_step;
    }
  }

  motor_1.motor((motor_direction[0] * 2 - 1) *m1_speed/2);
  motor_2.motor((motor_direction[1] * 2 - 1) *m2_speed/2);
  motor_3.motor((motor_direction[2] * 2 - 1) *m3_speed/2);
  motor_4.motor((motor_direction[3] * 2 - 1) *m4_speed/2);

  Serial.printf("%d:%d:%d  >  %d:%d:%d:%d\n",LY,LX,R_X,m1_speed,m2_speed,m3_speed,m4_speed);
}
