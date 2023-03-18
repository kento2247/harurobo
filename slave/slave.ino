#include <Arduino.h>

#include "CytronMD.h"

HardwareSerial Serial_hd(2);
#define pwm_1 2                                // pin指定
#define pwm_2 5                                // pin指定
#define pwm_3 19                               // pin指定
#define pwm_4 22                               // pin指定
#define dir_1 15                               // pin指定
#define dir_2 4                                // pin指定
#define dir_3 18                               // pin指定
#define dir_4 21                               // pin指定
CytronMD motor_1 = CytronMD(pwm_1, dir_1, 1);  //lack
CytronMD motor_2 = CytronMD(pwm_2, dir_2, 2);  //ere
CytronMD motor_3 = CytronMD(pwm_3, dir_3, 3);  //手前射出
CytronMD motor_4 = CytronMD(pwm_4, dir_4, 4);  //奥射出
#define bulve_1 12
#define bulve_2 13

#define data_len 8
int state_pin[8] = { 25, 33, 32, 35, 34, 26, 27, 14 };
bool motor_direction[4] = {  //switch4~7と連動
  0, 0, 0, 0
};  // モーターの回転方向, 0=reverse, 1=straight
bool switch_state[8];

void shot(bool);
//void send_switch_state();
void set_motor_dir();
// bool data_send(byte *);
bool *data_receive();


void setup() {
  Serial.begin(115200);  // デバック用
  Serial_hd.begin(115200);
  for (int i = 0; i < 8; i++) {
    if (i == 3 || i == 4)pinMode(state_pin[i], INPUT);
    else pinMode(state_pin[i], INPUT_PULLUP);
  }
  pinMode(bulve_1, OUTPUT);
  pinMode(bulve_2, OUTPUT);
  while (Serial_hd.available())Serial_hd.read();
}


bool old_bulve_2 = 0;

void loop() {
  //   send_switc/h_state();
  delay(10);
  for (int i = 0; i < 8; i++) {
    switch_state[i] = digitalRead(state_pin[i]);
  }
  bool *rev_data = data_receive();
  if (rev_data == NULL) return;
  if (rev_data[0] == 0) { //emergency stop
    motor_1.motor(0);
    motor_2.motor(0);
    motor_3.motor(0);
    motor_4.motor(0);
    Serial.println("emergency");
    while (data_receive() == NULL)
      ;
  }
  if (rev_data[1]) motor_1.motor(32 * (2 * switch_state[0] - 1)); //lack
  else if (rev_data[2]) motor_1.motor(-32 * (2 * switch_state[0] - 1));
  else motor_1.motor(0);

  if (rev_data[3]) motor_2.motor(32 * (2 * switch_state[1] - 1)); //ere
  else if (rev_data[4]) motor_2.motor(-32 * (2 * switch_state[1] - 1));
  else motor_2.motor(0);

  if (rev_data[5]) digitalWrite(bulve_1, !switch_state[2]);  //arm deploy
  else digitalWrite(bulve_1, switch_state[2] );

  static bool is_bulve_open = 0;
  bool bulve_2_state = rev_data[6];
  if (old_bulve_2 == 0 && bulve_2_state == 1) {
    is_bulve_open = !is_bulve_open;
  }
  digitalWrite(bulve_2, is_bulve_open);
  old_bulve_2 = bulve_2_state;

  bool shot_state = rev_data[7];
  shot(shot_state);
}

// void send_switch_state() {
//   Serial.print("state=");
//   for (int i = 0; i < 8; i++) {
//     switch_state[i] = digitalRead(state_pin[i]);
//     Serial.printf("%d:", switch_state[i]);
//   }
//   for (int i = 4; i < 8; i++) {
//     motor_direction[i] = switch_state[i];
//   }

//   Serial.println("");
//   byte switch_val;
//   byte num_base = 1;
//   for (byte i = 0; i < 8; i++) {
//     switch_val += num_base * switch_state[i];
//     num_base *= 2;
//   }
//   byte send_data[data_len];
//   send_data[0] = 255;                    //motor_dir
//   send_data[data_len - 1] = switch_val;  //motor_dir
//   if (Serial_hd.available()) {
//     bool result = data_send(send_data);
//     if (result) Serial.println("Send success!!!");
//     else Serial.println("failed");
//   }
//   else Serial.println("failed");
// }
