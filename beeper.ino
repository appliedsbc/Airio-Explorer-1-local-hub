//=================================================================================================================
void beep (String beep) {

  //beep=""; //UNCOMMENT FOR SILENT MODE
  
  //add custom melodies

  // connected (SD Detected)
  if (beep == "connected"){
    tone(8, 3000, 100);
    delay(150);
    tone(8, 4000, 50);
    delay(200);
  }

  // connected 2 (WiFi Connection Success)
  if (beep == "connected2"){
    tone(8, 4100, 130);
    delay(310);
    tone(8, 4100, 150);
    delay(160);
  }

  // double chirp
  if (beep == "double-chirp"){
    tone(8, 4100, 75);
    delay(100);
    tone(8, 4100, 100);
    delay(150);
  }

  // disconnected
  if (beep == "disconnected"){
    tone(8, 4000, 100);
    delay(150);
    tone(8, 3000, 50);
    delay(200);
  }

  // long tone
  if (beep == "long"){
    tone(8, 3500, 200);
    delay(200);
  }

  // quick chirp
  if (beep == "chirp"){
    tone(8, 4200, 30);
    delay(200);
  }

  // error
  if (beep == "error"){
    tone(8, 2000, 150);
    delay(200);
    tone(8, 1700, 300);
    delay(310);
  }
  
}
