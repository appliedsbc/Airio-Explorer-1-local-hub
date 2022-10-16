
//=================================================================================================================
void UserClick() {

  beep("long");
  Serial.println("new client");           // print a message out the serial port
  String currentLine = "";                // make a String to hold incoming data from the client
  bool autorefresh = 0;

  while (client.connected()) {            // loop while the client's connected
    if (client.available()) {             // if there's bytes to read from the client,
      char c = client.read();             // read a byte, then
      Serial.write(c);                    // print it out the serial monitor
      if (c == '\n') {                    // if the byte is a newline character

        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {

        loadDashboard(present_file);

          // break out of the while loop:
          break;
        } else {    // if you got a newline, then clear currentLine:
          currentLine = "";
        }
        
      } else if (c != '\r') {  // if you got anything else but a carriage return character,
        currentLine += c;      // add it to the end of the currentLine
      }



      //GENERATE CSV DOWLODAD LINKS
      if (currentLine.endsWith("GET /browse")) {
        browseCSV();
        light("B");
      }
      if (currentLine.endsWith("X")) {
light("B");
      }

      //DOWNLOAD CSV
      if (currentLine.endsWith(".CSV")) {
        String filename = "/Data/" + currentLine.substring(currentLine.lastIndexOf(".CSV") - 8, currentLine.lastIndexOf(".CSV") + 4);
        //downloadCSV(filename);
        present_file = filename;
        //loadDashboard(present_file);
      }

    }

  }
  client.stop();
  delay(100);
  Serial.println("client disconnected");
  client.stop();
}
//=================================================================================================================
void loadDashboard (String filename) {
  //LOAD THE DASHBOARD HTML PAGE
  String hypertext = "";
  File dataFile = SD.open("/HTML/dash.txt", FILE_READ);
  if (dataFile) {
    while (dataFile.available() != 0) {
      hypertext += char(dataFile.read());
    }
    dataFile.close();
  }
  String date_format = filename;
  date_format.remove(0, 6);
  date_format.remove(8, 10);
  date_format.replace("-", "/");
  hypertext.replace("<!CHART TITLE>", date_format);
  //hypertext.replace("<!CHART UNITS>", "PARTS PER MILLION");

  //SEND THE PAGE
  int data_index = hypertext.indexOf("<!CHART DATA>");
  client.print(hypertext.substring(0, data_index));
  //filename = "/Data/" + filename + ".CSV";
  Serial.println(filename);
  googleData(filename);

  //INSERT A LIST OF AVAILABLE RECORDS FROM SD
  int browse_index = hypertext.indexOf("<!BROWSE RECORDS>");
  client.print(hypertext.substring((data_index + 13), browse_index));
  browseCSV();

  //CLOSE HTML
  client.print(hypertext.substring((browse_index + 17), hypertext.length())); //prints bottom section of dash.html

}
//=================================================================================================================
void googleData (String filename) {
  //SENDS A .CSV FILE IN "GOOGLE" FORMAT

  String plotData;
  Serial.println("googledata");
Serial.println(filename);
  File dataFile = SD.open(filename, FILE_READ);
  if (dataFile) {
    int sizee = 45; //buffer should be slightly larger than 1 row of characters
    char buf[sizee];
    int end_of_row;

    //int maxi = 0;
    while (dataFile.available() > 0 || end_of_row > 0) {
      //Serial.println(plotData.length());
      //if (plotData.length() > maxi) maxi = plotData.length();
      //Serial.println(maxi);
      if (dataFile.available() >= sizee && plotData.length() < (2 * sizee)) {
        dataFile.read(buf, sizee);
        for (int i = 0; i <= (sizee - 1); i++) {
          plotData += buf[i];
        }
      }
      else if (dataFile.available()) {
        plotData += char(dataFile.read());
      }

      end_of_row = plotData.indexOf("\r\n");
      if (end_of_row > 0) {
        client.print("[");
        client.print(plotData.substring(0, end_of_row));
        client.print("],\r\n");
        plotData.remove(0, (end_of_row + 2));
      }
    }
    dataFile.close();
    digitalWrite(3, LOW);


  }
}
//=================================================================================================================
void downloadCSV(String filename) {

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/csv");
  client.println("");

  int sizee = 32;
  char buf[sizee];
  File dataFile = SD.open(filename);
  if (dataFile) {
    while (dataFile.available() >= sizee) {
      dataFile.read(buf, sizee);
      client.write(buf, sizee);
    }
    while (dataFile.available()) {
      client.write(dataFile.read());
    }
  }
  dataFile.close();
  client.stop();

}
//=================================================================================================================
void browseCSV() {

//  client.println("HTTP/1.1 200 OK");
//  client.println("Content-type:text/html");
//  client.println("");
//  client.println("<html>");
//  client.println("<head>");
//  //if (autorefresh == 1) client.println("<meta http-equiv=\"refresh\" content=\"30\">");
//  client.println("</head>");



  refreshFiles();
  while (fileList.length() > 1) {
    String current_file = fileList.substring(0, fileList.indexOf("\n"));
    fileList.remove(0, (fileList.indexOf("\n") + 1));
    current_file.trim();
    client.print("<option value=\"http://");
    client.print(ip);
    client.print("/");
    client.print(current_file);
    client.print("\"");
    if ("/Data/" + current_file == present_file) client.print(" selected");
    client.print(">");
    client.print(current_file);
    client.print("</option>");
  }


//  client.println("</html>");
//  client.println();
//  client.stop();
}
//=================================================================================================================
