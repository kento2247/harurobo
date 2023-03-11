#include <Arduino.h>

#include "CytronMD.h"
#include "PS4Controller.h"
HardwareSerial Serial_hd(2);

#define MAC_string "24:62:AB:FB:15:1A"
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
#define servo_1 18
#define servo_2 19
#define bulve_1 23
#define bulve_2 2
#define buzzer_pin 5
#define switch_pin 22

#define data_len 16

void motor_control(int, int, int);
bool data_send(byte *);
byte *data_receive();
bool motor_direction[4] = {
  0, 0, 0, 0
};  // 足回り用モーターの回転方向, 0=reverse, 1=straight


void send_demo() {
  byte serial_data[data_len];
  for (int i = 0; i < data_len; i++) {
    serial_data[i] = i + 10;
  }
  bool result = data_send(serial_data);
  if (result) Serial.println("Send success!!!");
  else Serial.println("failed");
  delay(100);
}
void receive_demo() {
  byte *rev_data = data_receive();
  if (rev_data == NULL) {
    // Serial.println("communication error");
  } else {
    Serial.println("success");
    digitalWrite(buzzer_pin, rev_data[15]);
  }
  delay(100);
}

void setup() {
  Serial.begin(115200);  // デバック用
  Serial_hd.begin(115200);
  PS4.begin(MAC_string);
  pinMode(buzzer_pin, OUTPUT);
}

void loop() {
  receive_demo();
  return;


  while (Serial_hd.available()) {
    String str = Serial_hd.readStringUntil('\n');
    Serial_hd.println("world\n");
    Serial.printf("received : %s\n", str);
  }

  if (PS4.isConnected()) {
    // controler_data[0] = PS4.Circle();
    // controler_data[1] = PS4.Cross();
    // controler_data[2] = PS4.Triangle();
    // controler_data[3] = PS4.Square();
    // for (int i = 0; i < 4; i++) {
    //   Serial.printf("%d ", controler_data[i]);
    // }
    // Serial.println();

    // motor_control(PS4.LStickY(), PS4.LStickX(), PS4.RStickX());

    int val = PS4.L2Value();
    PS4.setRumble(val, val);
    PS4.setLed(PS4.R2Value(), 100, 100);
    PS4.sendToController();
  }
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

bool data_send(byte *data) {
  int state = 1;
  byte timeout_count = 10;
  while (state != 3) {
    while (Serial_hd.available()) Serial_hd.read();
    // Serial.printf("sending data(%d) = ", state);
    // for (int i = 0; i < data_len; i++) {
    //   Serial.printf("%d : ", data[i]);
    // }
    // Serial.println();
    Serial_hd.write(state);
    Serial_hd.write(data, data_len);
    // Serial.println("response waiting");
    byte wait_timeout_count = 30;
    while (!Serial_hd.available()) {
      // Serial.print(".");
      delay(100);
      wait_timeout_count--;
      if (wait_timeout_count == 0) {
        // Serial.println("send failed(non-response)");
        return 0;
      }
    }
    // Serial.println();
    state = 3;  //ok
    timeout_count--;
    // Serial.print("response : ");
    for (int i = 0; i < data_len; i++) {
      byte res = Serial_hd.read();
      // Serial.printf("%d : ", res);
      if (data[i] != res) {
        // Serial.println("data is not matched");
        state = 2;  //re
        while (Serial_hd.available()) Serial_hd.read();
        break;
      }
    }
    Serial_hd.write(state);
    if (state == 3) return 1;
    if (timeout_count == 0) return 0;
    // Serial.println("send failed");
  }
}

byte *data_receive() {
  if (Serial_hd.available() == 0) return NULL;
  int state = 1;
  byte timeout_count = 10;
  static byte data[data_len];

  while (state != 3) {
    state = Serial_hd.read();
    Serial.printf("received(%d) : ", state);
    for (int i = 0; i < data_len; i++) {
      data[i] = Serial_hd.read();
      Serial.printf("%d : ", data[i]);
      Serial_hd.write(data[i]);
    };
    Serial.println();
    while (!Serial_hd.available())
      ;
    state = Serial_hd.read();
    timeout_count--;
    if (state == 3) return data;
    if (timeout_count == 0) return NULL;
  }
}