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

void set_motor_dir();
void motor_control(int, int, int);
bool data_send(byte *);
byte *data_receive();
void send_demo();
void receive_demo();
void servo_atatach();
void servo_detach();
void servo_demo();



void setup() {
  Serial.begin(115200);  // デバック用
  Serial_hd.begin(115200);
  PS4.begin(MAC_string);
  pinMode(buzzer_pin, OUTPUT);
}

void loop() {
  delay(10);
  if (PS4.isConnected()) {
    digitalWrite(buzzer_pin, 0);  //ブザーオフ
    set_motor_dir();
    motor_control(PS4.LStickY(), PS4.LStickX(), PS4.RStickX());
    // Serial.printf("%d:%d:%d:%d\n",PS4.Circle(),PS4.Cross(),PS4.Triangle(),PS/4.Square());
    // int val = PS4.L2Value();
    // PS4.setRumble(val, val);
    // PS4.setLed(PS4.R2Value(), 100, 100);
    // PS4.sendToController();
    if (PS4.Circle()) {
      servo_demo();
    }
    if (PS4.Cross()) {
      PS4.setRumble(255, 255);
      PS4.sendToController();
    }
    else {
      PS4.setRumble(0, 0);
      PS4.sendToController();
    }
    if (PS4.Triangle()) {
    }
    if (PS4.Square()) {
    }
  } else {
    digitalWrite(buzzer_pin, 1);  //ブザーオン
  }
}
