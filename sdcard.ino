//===================================================================================================================
void refreshFiles()
{
  fileList = "";
  File root = SD.open("/Data");
  root.rewindDirectory();
  listDirectory(root);
  root.close();
}

//===================================================================================================================
void listDirectory(File& dir)
{
  while (true) {
    File entry = dir.openNextFile();
    if (! entry) {
      break;
    }
    fileList += entry.name();
    fileList += "\n";
    entry.close();
  }
}
//===================================================================================================================
void logSD()
{
    //organize the data in CSV format
    dataString += String(time_read);
    dataString += ", ";
    dataString += CARBONDIOXIDE;
    dataString += ", ";
    dataString += OXYGEN;
    dataString += ", ";
    dataString += HUMIDITY;
    dataString += ", ";
    dataString += TEMPERATURE;
    dataString += ", ";
    dataString += PRESSURE;


    //write data so SD card
    char file_name[] = "/Data/xx-xx-xxxx.csv";
    sprintf(file_name, "/Data/%02d-%02d-%02d.csv", month(now()), day(now()), (year(now()) - 2000));
    present_file = file_name;

    File dataFile = SD.open(file_name, FILE_WRITE);
    delay(10);
    if (dataFile) {
      dataFile.println(dataString);
      delay(100);
      dataFile.close();
      Serial.println(dataString);
      
      //beep("chirp");
      digitalWrite(16, HIGH);
      light("G");
      delay(500);
      digitalWrite(16, LOW);
    }
    dataString = "";
}

//===================================================================================================================
//ISR on SD card change event
void insertSD()
{
  Serial.println("SD Inserted.");
  delay(2000);
  Serial.println("resetting...");
  delay(1000);
  NVIC_SystemReset();
}
