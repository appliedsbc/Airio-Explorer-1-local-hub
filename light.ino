//=================================================================================================================
void light (String light) {
  int ledPin;
  //light=""; //UNCOMMENT FOR SILENT MODE

  //add custom melodies

  // connected (SD Detected)
  if (light == "R") {
    ledPin = 6;
  }

  if (light == "G") {
    ledPin = 5;
  }

  if (light == "B") {
    ledPin = 3;
  }

  for (int fadeValue = 0 ; fadeValue <= 30; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);
    delay(60);
  }
  while(1);
  for (int fadeValue = 30 ; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);
    delay(30);
  }

}
