#include <Arduino.h>

#include "CytronMD.h"
#include "PS4Controller.h"
#define data_length 4
byte controler_data[data_length];
#define pwm_1 33  // pin指定
#define pwm_2 26  // pin指定
#define pwm_3 14  // pin指定
#define pwm_4 12  // pin指定
#define dir_1 32  // pin指定
#define dir_2 25  // pin指定
#define dir_3 27  // pin指定
#define dir_4 13  // pin指定
CytronMD motor_1 = CytronMD(pwm_1, dir_1, 1);
CytronMD motor_2 = CytronMD(pwm_2, dir_2, 2);
CytronMD motor_3 = CytronMD(pwm_3, dir_3, 3);
CytronMD motor_4 = CytronMD(pwm_4, dir_4, 4);

void show_data(double, double, int, int, int, int);
void motor_control(int, int, int);

void setup() {
  Serial.begin(115200);  // デバック用
  // Serial2.begin(115200, SERIAL_8N1, 16, 17);
  // while (!Serial2)
  //   ;
  // PS4.begin("24:62:AB:FB:15:1A");
}

void loop() {
  motor_control(-16, 0, 0);
  delay(1000);
  motor_control(0, 16, 0);
  delay(1000);
  motor_control(16, 0, 0);
  delay(1000);
  motor_control(0, -16, 0);
  delay(1000);

  // while (Serial2.available()) {
  // String str = Serial2.readStringUntil('\n');
  // Serial2.println("world\n");
  // Serial.printf("%s\n", str);
  // }

  // if (PS4.isConnected()) {
  //   // controler_data[0] = PS4.Circle();
  //   // controler_data[1] = PS4.Cross();
  //   // controler_data[2] = PS4.Triangle();
  //   // controler_data[3] = PS4.Square();
  //   // for (int i = 0; i < 4; i++) {
  //   //   Serial.printf("%d ", controler_data[i]);
  //   // }
  //   // Serial.println();
  //   int val = PS4.L2Value();
  //   PS4.setRumble(val, val);
  //   PS4.setLed(PS4.R2Value(), 100, 100);
  //   PS4.sendToController();
  // }
}

void motor_control(int LY, int LX, int R_X) {
  double rad = atan2(LY, LX);
  double len = sqrt(LX * LX + LY * LY);
  int m1_speed = (-1 * cos(rad + PI / 4) * len);
  int m2_speed = (-1 * sin(rad + PI / 4) * len);
  int m3_speed = (-1 * m2_speed);
  int m4_speed = (-1 * m1_speed);
  show_data(rad, len, m1_speed, m2_speed, m3_speed, m4_speed);

  int motor_speed[4] = {//-128~127の範囲でない可能性あり
                        m1_speed, m2_speed, m3_speed, m4_speed};
  int turn = R_X;
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
    show_data(rad, len, m1_speed, m2_speed, m3_speed, m4_speed);
  }

  motor_1.motor(m1_speed);
  motor_2.motor(m2_speed);
  motor_3.motor(m3_speed);
  motor_4.motor(m4_speed);
}

// デバック用
void show_data(double rad, double len, int m1_speed, int m2_speed, int m3_speed,
               int m4_speed) {
  Serial.print("rad = ");
  Serial.print(rad * 180 / PI);
  Serial.print(",  len = ");
  Serial.println(len);

  Serial.print("m1=");
  Serial.print(m1_speed);
  Serial.print(", ");
  Serial.print("m2=");
  Serial.print(m2_speed);
  Serial.print(", ");
  Serial.print("m3=");
  Serial.print(m3_speed);
  Serial.print(", ");
  Serial.print("m4=");
  Serial.println(m4_speed);
  Serial.println();
}