#include <Arduino.h>

#include "CytronMD.h"
#include "PS4Controller.h"
#include <ESP32Servo.h>
HardwareSerial Serial_hd(2);
Servo servo1;
Servo servo2;

#define MAC_string "24:62:AB:FB:15:1A"
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
#define servo_1 18
#define servo_2 19
#define bulve_1 23
#define bulve_2 2
#define buzzer_pin 5
#define switch_pin 22

#define data_len 8  //hdserialのパケット長

bool motor_direction[4] = {
  1, 1, 1, 1
};  // 足回り用モーターの回転方向, 0=reverse, 1=straight

void emergency_stop();
void set_motor_dir();
void motor_control(int, int, int);
bool data_send(byte *);
byte *data_receive();
void send_demo();
void receive_demo();
void servo_atatach();
void servo_detach();
void servo_open();
void servo_close();



void setup() {
  Serial.begin(115200);  // デバック用
  Serial_hd.begin(115200);
  PS4.begin(MAC_string);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(bulve_1, OUTPUT);
  pinMode(bulve_2, OUTPUT);
}

bool arm_state = 0;
bool shot_state = 0;
bool servo_state = 0;
bool old_servo_state = 0;
bool larm_state = 0;
bool rarm_state = 0;

bool old_up = 0;
bool old_circle = 0;
bool old_cross = 0;
bool old_r = 0;
bool old_l = 0;
void loop() {
  while (Serial_hd.available())
    Serial_hd.read();
  while (PS4.isConnected()) {
    delay(10);
    if (PS4.Square()) {  //緊急停止
      Serial.println("emergency stop");
      emergency_stop();
      Serial.println("recovered");
      return;
    } else {
      set_motor_dir();
      motor_control(PS4.LStickY(), PS4.LStickX(), PS4.RStickX());
      if (old_circle != PS4.Circle()) shot_state = 1 - shot_state;
      if (old_up != PS4.Up()) arm_state = 1 - arm_state;
      if (old_cross != PS4.Cross()) servo_state = 1 - servo_state;
      if (old_l != PS4.Left()) larm_state = 1 - larm_state;
      if (old_r != PS4.Right()) rarm_state = 1 - rarm_state;
      old_circle = PS4.Circle();
      old_up = PS4.Up();
      old_cross = PS4.Cross();
      old_l = PS4.Left();
      old_r = PS4.Right();

      byte send_data[8] = {
        254, PS4.L1(), (bool)PS4.L2(), PS4.R1(), (bool)PS4.R2(), arm_state, PS4.Triangle(), shot_state
      };

      Serial.print("sending data  ");
      for (byte i = 0; i < 8; i++) {
        Serial.printf("%d:", send_data[i]);
      }
      data_send(send_data);
      Serial.println();

      if (old_servo_state != servo_state) {
        if (servo_state) servo_open();
        else servo_close();
      }
      old_servo_state = servo_state;
      if (larm_state) digitalWrite(bulve_1, 1);
      else digitalWrite(bulve_1, 0);
      if (rarm_state) digitalWrite(bulve_2, 1);
      else digitalWrite(bulve_2, 0);
    }
  }
  delay(10);
}


void emergency_stop() {
  byte send_data[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  data_send(send_data);
  digitalWrite(buzzer_pin, 1);  //ブザーオン
  motor_control(0, 0, 0);
  while (PS4.Square()) delay(10);
  while (!PS4.Square()) delay(10);
  while (PS4.Square()) delay(10);
  digitalWrite(buzzer_pin, 0);  //ブザーオフ
}