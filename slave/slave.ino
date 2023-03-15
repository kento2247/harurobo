#include <Arduino.h>

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

#define data_len 8
int state_pin[8] = { 25, 33, 32, 35, 34, 26, 27, 14 };
bool motor_direction[4] = {
  0, 0, 0, 0
};  // 足回り用モーターの回転方向, 0=reverse, 1=straight
bool switch_state[8];

void get_switch_state();
void set_motor_dir();
void motor_control(int, int, int);
bool data_send(byte *);
byte *data_receive();


void setup() {
  Serial.begin(115200);  // デバック用
  Serial_hd.begin(115200);
  for (int i = 0; i < 8; i++) {
    pinMode(state_pin[i], INPUT_PULLUP);
  }
  pinMode(bulve_1, OUTPUT);
  pinMode(bulve_2, OUTPUT);
}

void loop() {
  get_switch_state();
  byte switch_val;
  byte num_base = 1;
  for (byte i = 0; i < 8; i++) {
    switch_val += num_base * switch_state[i];
    num_base *= 2;
  }
  byte send_data[data_len];
  send_data[0] = 255; //motor_dir
  send_data[data_len - 1] = switch_val; //motor_dir
  bool result = data_send(send_data);
  if (result) Serial.println("Send success!!!");
  else Serial.println("failed");

  byte rev_data=data_receive();
  Serial.print()

  motor_1.motor(127 * switch_state[0]);
  motor_2.motor(127 * switch_state[1]);
  motor_3.motor(127 * switch_state[2]);
  motor_4.motor(127 * switch_state[3]);
  digitalWrite(bulve_1, switch_state[4]);
  digitalWrite(bulve_2, switch_state[5]);
  return;


  if (Serial_hd.available()) {
    String str = Serial_hd.readStringUntil('\n');
    Serial.printf("receive : %s\n", str);
    Serial_hd.println("hello\n");
  } else {
    Serial.println("message sent");
    Serial_hd.println("hello\n");
    delay(1000);
    //    Serial.printf("connection status=%d\n", Serial_hd.available());
    //    get_switch_state();
  }
  //  delay(500);
}

void get_switch_state() {
  Serial.print("state=");
  for (int i = 0; i < 8; i++) {
    switch_state[i] = digitalRead(state_pin[i]);
    Serial.printf("%d:", switch_state[i]);
  }
  Serial.println("");
}
