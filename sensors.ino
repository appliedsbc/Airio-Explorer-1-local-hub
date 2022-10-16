//These functions handle communication with the sensors to poll measurements and store them in variables.
//====SENSORS======================================================================================================

void LOX_02(){
//LOX-02
  OXYGEN = "";
  while(Serial1.available()){
    char t = Serial1.read();
  }
  Serial1.print("% \r\n");
  while(!Serial1.available());
  char buffer[16];
  int size = Serial1.readBytesUntil('\r', buffer, 16);
  for (int i = 3 ; i < size ; i++) {
    OXYGEN += buffer[i];
  }
  
  PRESSURE = "";
  while(Serial1.available()){
    char t = Serial1.read();
  }
  Serial1.print("P \r\n");
  while(!Serial1.available());
  //char buffer[16];
  size = Serial1.readBytesUntil('\r', buffer, 16);
  for (int i = 3 ; i < size ; i++) {
    PRESSURE += buffer[i];
  }
}

void HIH8000(){
  //HIH8000 TEMP AND HUMIDITY
  int WireBytes[4];
  Wire.beginTransmission(0x27);
  Wire.write(0x27);
  Wire.endTransmission(); //begins measurement
  delay(100); //wait for conversion
  Wire.requestFrom(0x27, 4); //receive 4 bytes
  WireBytes[0] = Wire.read();
  WireBytes[1] = Wire.read();
  WireBytes[2] = Wire.read();
  WireBytes[3] = Wire.read();
  float RH = (256 * (63 & WireBytes[0])) + WireBytes[1];
  RH = (RH / 16383) * 100;
  float TEMP = (63 * WireBytes[2]) + (63 & (WireBytes[3] >> 2));
  TEMP = ((TEMP / 16383) * 165) - 40;
  HUMIDITY = RH;
  TEMPERATURE = TEMP;
}

void T67XX(){
  //T67XX CARBON DIOXIDE
  int WireBytes[4];
  Wire.beginTransmission(0x15);
  Wire.write(0x04);
  Wire.write(0x13);
  Wire.write(0x8B);
  Wire.write(0x00);
  Wire.write(0x01);
  Wire.endTransmission(); //begins measurement
  delay(30);
  Wire.requestFrom(0x15, 4); //receive 4 bytes
  WireBytes[0] = Wire.read();
  WireBytes[1] = Wire.read();
  WireBytes[2] = Wire.read();
  WireBytes[3] = Wire.read();
  delay(1000);
  int CO2 = (WireBytes[2] *256) + WireBytes[3];
  CARBONDIOXIDE = CO2;
}
