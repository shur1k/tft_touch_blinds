/*====================================================================================
  This sketch contains support functions for the ESP6266 SPIFFS filinf system

  Created by Bodmer 9th Jan 2017
  ==================================================================================*/
 

//====================================================================================
//          This function prints a SPIFFS directory list (root directory)
//====================================================================================

void listFiles(void) {
  Serial.println();
  Serial.println("SPIFFS files found:");

  Dir dir = SPIFFS.openDir("/"); // Root directory
  String  line = "=====================================";

  Serial.println(line);
  Serial.println("  File name               Size");
  Serial.println(line);

  while (dir.next()) {
    String fileName = dir.fileName();
    Serial.print(fileName);
    int spaces = 25 - fileName.length(); // Tabulate nicely
    while (spaces--) Serial.print(" ");
    File f = dir.openFile("r");
    Serial.print(f.size()); Serial.println(" bytes");
  }

  Serial.println(line);
  Serial.println();
  delay(1000);
}
//====================================================================================