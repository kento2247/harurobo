// bool data_send(byte *data) {
//   int state = 1;
//   byte resend_count = 5;  //5回までは再送可能
//   while (state != 3) {
//     while (Serial_hd.available()) Serial_hd.read();  //いらないデータを破棄
//     Serial_hd.write(state);
//     Serial_hd.write(data, data_len);
//     byte wait_timeout_count = 255;
//     while (!Serial_hd.available()) {
//       wait_timeout_count--;
//       if (wait_timeout_count == 0) return 0;
//     }
//     state = 3;  //ok
//     resend_count--;
//     for (int i = 0; i < data_len; i++) {
//       byte res = Serial_hd.read();
//       if (data[i] != res) {
//         state = 2;  //re
//         while (Serial_hd.available()) Serial_hd.read();
//         break;
//       }
//     }
//     Serial_hd.write(state);
//     if (state == 3) return 1;
//     if (resend_count == 0) return 0;
//   }
// }

// byte *data_receive() {
//   if (Serial_hd.available() == 0) return NULL;
//   int state = 1;
//   byte resend_count = 5;
//   static byte data[data_len];

//   while (state != 3) {
//     state = Serial_hd.read();
//     for (int i = 0; i < data_len; i++) {
//       data[i] = Serial_hd.read();
//       Serial_hd.write(data[i]);
//     };
//     byte timeout_count = 100;
//     while (!Serial_hd.available()) {
//       delay(10);
//       timeout_count--;
//       if (timeout_count == 0) return NULL;
//     }
//     state = Serial_hd.read();
//     resend_count--;
//     if (state == 3) {
//       Serial.print("rev data=");
//       for (byte i = 0; i < data_len; i++) {
//         Serial.printf("%d:", data[i]);
//       }
//       Serial.println();
//       return data;
//     }
//     if (timeout_count == 0) return NULL;
//   }
// }

void data_send(bool *data) {
  byte base_num = 1;
  byte send_data = 0;
  for (byte i = 0; i < 8; i++) {
    send_data += base_num * data[i];
    base_num *= 2;
  }
  Serial_hd.write(send_data);
}