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
