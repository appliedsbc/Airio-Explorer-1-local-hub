
//=================================================================================================================
void ConnectToWiFi() {
  //Connect to WiFi
  File dataFile = SD.open("Login.txt", FILE_READ);
  if (dataFile) {
    String Login;
    while (dataFile.available() != 0) {
      Login += char(dataFile.read());
    }
    dataFile.close();
    ssid = Login.substring((Login.indexOf(":") + 1), Login.indexOf("\n"));
    ssid.trim();
    pass = Login.substring((Login.lastIndexOf(":") + 1), Login.lastIndexOf("\n"));
    pass.trim();
  }

  dataFile = SD.open("/Config/timezone.txt", FILE_READ);
  if (dataFile) {
    String Zone;
    while (dataFile.available() != 0) {
      Zone += char(dataFile.read());
    }
    dataFile.close();
    timeZone = (Zone.toInt()) * 3600;
  }
  
  //WIFI INITIALIZE
  while (status != WL_CONNECTED) {
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid.c_str(), pass.c_str());
    delay(1000);
    if (status != 3) {
      Serial.println("Cannot connect to WiFi. Check SD card for Login.txt");
      beep("error");
    }
  }
  server.begin();                           // start the web server on port 80
  Serial.println("WiFi connected.");
  delay(100);

  syncWifiTime(); //sync the time with network time
   
  beep("connected2");

  SD.remove("ip.txt");
  dataFile = SD.open("/Config/ip.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(WiFi.localIP());
  }
  dataFile.close();


  printWifiStatus();                        // you're connected now, so print out the status
}

//=================================================================================================================
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
//=================================================================================================================
void syncWifiTime() {
  //SYNC CLOCK WITH NETWORK TIME
  while (WiFi.getTime() == 0) {
    delay(1000);
    Serial.println("Synchronizing clock to network time...");
  }
  //setTime(WiFi.getTime() + timeZone); //sync the internal clock to wifitime
  setTime(WiFi.getTime());
  
//  bool is_dl = 0;
//  if (month(now()) > 3 && month(now()) < 11) is_dl = 1; //daylight savings
//  if (month(now()) == 3 && day(now()) > 14) is_dl = 1;
//  if (month(now()) == 11 && day(now()) < 6) is_dl = 1;
//  if (is_dl == 1) setTime(now() + 3600);
  //Serial.println(now());
}
