#include <Arduino.h>

#include "BluetoothSerial.h"
#include "CytronMD.h"

HardwareSerial Serial_hd(2);
#define pwm_1 2   // pin指定
#define pwm_2 5   // pin指定
#define pwm_3 19  // pin指定
#define pwm_4 22  // pin指定
#define dir_1 15  // pin指定
#define dir_2 4   // pin指定
#define dir_3 18  // pin指定
#define dir_4 21  // pin指定
CytronMD motor_1 = CytronMD(pwm_1, dir_1, 1);
CytronMD motor_2 = CytronMD(pwm_2, dir_2, 2);
CytronMD motor_3 = CytronMD(pwm_3, dir_3, 3);
CytronMD motor_4 = CytronMD(pwm_4, dir_4, 4);
#define bulve_1 12
#define bulve_2 13

int state_pin[8] = { 25, 33, 32, 35, 34, 26, 27, 14 };

void motor_control(int, int, int);
void get_switch_state();
bool motor_direction[4] = {
  0, 0, 0, 0
};  // 足回り用モーターの回転方向, 0=reverse, 1=straight
bool switch_state[8];

void setup() {
  Serial.begin(115200);  // デバック用
  Serial_hd.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(state_pin[i], INPUT_PULLUP);
  }
}

void loop() {
  if (Serial_hd.available()) {
    String str = Serial_hd.readStringUntil('\n');
    Serial.printf("%s\n", str);
    Serial_hd.println("hello\n");
  } else {
    Serial.printf("connection status=%d\n", Serial_hd.available());
    get_switch_state();
  }
  delay(500);
}

void get_switch_state() {
  Serial.print("state=");
  for (int i = 0; i < 8; i++) {
    switch_state[i] = digitalRead(state_pin[i]);
    Serial.printf("%d:", switch_state[i]);
  }
  Serial.println("");
}

void motor_control(int LY, int LX, int R_X) {
  double rad = atan2(LY, LX);
  double len = sqrt(LX * LX + LY * LY);  // 0~127
  if (len > 127) len = 127;
  int m1_speed = ((motor_direction[0] * 2 - 1) * cos(rad + PI / 4) * len);
  int m2_speed = ((motor_direction[1] * 2 - 1) * sin(rad + PI / 4) * len);
  int m3_speed = ((motor_direction[2] * 2 - 1) * m2_speed);
  int m4_speed = ((motor_direction[3] * 2 - 1) * m1_speed);

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
        motor_speed[i] -= turn_step;
      }
      if (escape_flag) break;
      m1_speed = motor_speed[0];
      m2_speed = motor_speed[1];
      m3_speed = motor_speed[2];
      m4_speed = motor_speed[3];
      turn -= turn_step;
    }
  }

  motor_1.motor(m1_speed);
  motor_2.motor(m2_speed);
  motor_3.motor(m3_speed);
  motor_4.motor(m4_speed);
}