void sendDataToSD(double data1, double data2){
  // make a string that start with a timestamp for assembling the data to log:
  String dataString("{");

  dataString += String("\"data1\":");
  dataString += String(data1);
  dataString += ",";
  dataString += String("\"data2\":");
  dataString += String(data2);
  dataString += "},";


  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  // The FileSystem card is mounted at the following "/mnt/FileSystema1"
  File dataFile = FileSystem.open("/mnt/sda1/dialog1.txt", FILE_APPEND);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    SerialUSB.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    SerialUSB.println("error opening datalog.txt");
  }

  delay(1000);
}

