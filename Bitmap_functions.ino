/*====================================================================================

   There is no clipping so images that don't fit within the screen boundaries will
   appear corrupted. Crop the image (in say Paint on Windows OS) before converting to
   16 bit, or position image so it does not go off the screen.

  Times to draw a 240 x 320 image, 40MHz SPI, 160MHz CPU
  for UTFT format files:
  
  Buffer size  Draw time
  (bytes)       (ms)
     64         172
    128         141
    240         126
    256         124
    400         120
    512         118 <--- buffer size sweet spot
    640         127
   1024         127

  Rendering time in milliseconds .v. SPI frequency (buffer size = 512) and encoding format
  240 x 320 image, 80MHz CPU clock:
   SPI    "UTFT" format  "Due" format
  80Mhz     228*(150)         230 
  40Mhz       167             249
  20Mhz       199             279

  As above, 160MHz CPU clock:
   SPI    "UTFT" format   "Due" format
  80Mhz     159*(102)         149
  40Mhz       118             165
  20Mhz       150             198

  *Figure includes deliberately added software delays, number in brackets excludes 
   software delays but TFT cannot cope with the peak pixel rate and image is corrupted.
   
  Created by Bodmer 9th Jan 2017
  ==================================================================================*/
#define PIXEL_BUFFER_SIZE 512 // Must be an integer multiple of 4, must be more than 54

//====================================================================================
//   This function renders the raw (16 bit, 565 RGB pixels) image file onto the TFT
//====================================================================================
void drawRaw(const char *filename, int32_t x, int32_t y, int32_t rawWidth, int32_t rawHeight) {

  //Serial.println("=====================================");
  //Serial.print("Drawing file: "); Serial.println(filename);
  //Serial.println("=====================================");

  File     imageFile;
  // Check file exists, open it if it does
  if ((imageFile = SPIFFS.open(filename, "r")) == 0) { // Open for reading
    //Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
    return;
  }

  uint8_t  pixelBuffer[PIXEL_BUFFER_SIZE];   // Pixel buffer
  uint32_t fs = imageFile.size();

#ifdef UTFT_FORMAT
  /*if (fs != (rawWidth * rawHeight * 2)) {
    Serial.println("##### ERROR IN WIDTH OR HEIGHT #####");
    return;
  }*/
#else
  if (fs != (rawWidth * rawHeight * 2 + 54)) {
	Serial.print("File size is: "); Serial.println(fs);
	Serial.print("File size should be: "); Serial.println(rawWidth * rawHeight * 2 + 54);
    Serial.println("##### ERROR IN WIDTH OR HEIGHT #####");
    return;
  }
  imageFile.read(pixelBuffer, 54); // Dump the file header (we could get the Width and Height)
  if (pixelBuffer[0] != 'B' || pixelBuffer[1] != 'M') {
    Serial.println("##### BAD FILE HEADER #####");
    return;
  }
  
#endif

  uint32_t start = 0;
  uint32_t fetchTime = 0;
  uint32_t renderTime = 0;
  uint32_t drawTime = micros(); // Save current time for performance evaluation

  digitalWrite(TFT_CS, LOW);
  // Prepare the TFT screen area to receive the data, the SPI chip select will stay low
  tft.setAddrWindow(x, y, x + rawWidth, y + rawHeight);

  // Work out how many whole pixel buffers but we need to read file based on the file size!
  // See comments below about closing file early!

  uint32_t nr = fs / PIXEL_BUFFER_SIZE;

  while (nr--) {
    // SPIFFS supports the Stream Class member functions
    start = micros();
    imageFile.read(pixelBuffer, PIXEL_BUFFER_SIZE);
    fetchTime += micros() - start;

#ifdef UTFT_FORMAT
  #ifdef ADD_DELAY // Use slower method of sending pixels so TFT can cope with bursts
    start = micros();
    uint32_t nWords = PIXEL_BUFFER_SIZE / 4;
    uint32_t* ptr = (uint32_t*)pixelBuffer;
    while (nWords--) SPI.write32(*ptr++, 0); // Byte order for UTFT = 0
    renderTime += micros() - start;
  #else
    start = micros();
    SPI.writeBytes(pixelBuffer, PIXEL_BUFFER_SIZE);
    renderTime += micros() - start;
  #endif
#else
    start = micros();
    uint32_t nWords = PIXEL_BUFFER_SIZE / 4;
    uint32_t* ptr = (uint32_t*)pixelBuffer;
    while (nWords--) SPI.write32(*ptr++, 1);  // Byte order for UTFT ILI9341_Due file format = 1
    renderTime += micros() - start;
#endif
    //yield(); // Uncomment if a really slow SPI clock is used (to stop watchdog timeout)
  }

  // Work out remining byte count
  nr = fs % PIXEL_BUFFER_SIZE;

  // Send any partial pixel buffer
  if (nr) {
    ////uint8_t* pixelBuffer = (uint8_t*)&SPI1W0; // We will pipe the bytes straight to the SPI buffer
    start = micros();
    imageFile.read(pixelBuffer, nr);   // Get pixels
    fetchTime += micros() - start;

    start = micros();
#ifdef UTFT_FORMAT
    SPI.writeBytes(pixelBuffer, nr); // Send to screen

#else
    uint32_t nPixels = nr / 4;
    uint32_t* ptr = (uint32_t*)pixelBuffer;
    while (nPixels--) {
      SPI.write32(*ptr++, 1);  // Byte order 1 for Due, but is slow @ 163 or 166ms, /4 on buffer
    }
#endif
    renderTime += micros() - start;

    //yield(); // Uncomment if a really slow SPI clock is used (to stop watchdog timeout)
  }

  digitalWrite(TFT_CS, HIGH); // TFT Chip Select was left low

  // Close the file, must have reched end or processor throws an exception!
  imageFile.close();

  drawTime = micros() - drawTime; // Calculate the time it took

  //Serial.print  ("Total render time was    : "); Serial.print(drawTime   / 1000.0, 1); Serial.println(" ms");
  //Serial.print  ("SPIFFS fetch time was    : "); Serial.print(fetchTime  / 1000.0, 2); Serial.println(" ms");
  //Serial.print  ("Render time to TFT was   : "); Serial.print(renderTime / 1000.0, 2); Serial.println(" ms");

  //Serial.print  ("Image transfer rate was  : "); Serial.print( (11 * 8 + rawWidth * rawHeight * 16.0) / drawTime, 2); Serial.println(" Mbps");
  //Serial.println("=====================================");
  
  // SPIFFS bug work-around:
  // In the ESP8266 Arduino Core version 2.2.0 and 2.3.0 the processor will throw an exception
  // if we close the file before the end is reached! This is why the file size is used instead
  // of the "Width x Height x 2" for the number of bytes to read (a user error in Width or
  // Height would otherwise crash the processor!).
}
//====================================================================================