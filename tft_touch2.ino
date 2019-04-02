#define BTN_CHK 50      //The time for checking if the button was pressed (millis)
#define LDPOW   1       //The POWER PIN for the Windpw
#define LDUD    3       //The UP/DOWN selection PIN for the Window
#define LDTIME  29500   //31 seconds for full open(close);
#define RDPOW   0       //The POWER PIN for the Windpw
#define RDUD    16      //The UP/DOWN selection PIN for the Window
#define RDTIME  26500   //28 seconds for full open(close);
#define TIMEINT 900000L //15 minutes (900 sec) timer interwall for temperature capture
//#define BLYNK_PRINT Serial

#include <Blinds.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include <FS.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h>

#define CS_PIN  4
#define TIRQ_PIN  5
XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling
#define TS_MINX 350
#define TS_MINY 430
#define TS_MAXX 3850
#define TS_MAXY 3850
#define SCREEN_ORIENTATION_1

#define TFT_DC 2
#define TFT_CS 15
#define UTFT_FORMAT

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_GFX_Button bLDDN, bLD40, bLD60, bLD80, bLDUP, bRDDN, bRD40, bRD60, bRD80, bRDUP, bBDDN, bBD40, bBD60, bBD80, bBDUP;
bool bLDDNf = false, bLDUPf = false, bLD40f = false, bLD60f = false, bLD80f = false; 
bool bRDDNf = false, bRDUPf = false, bRD40f = false, bRD60f = false, bRD80f = false;
bool bBDDNf = false, bBDUPf = false, bBD40f = false, bBD60f = false, bBD80f = false;

const char *ld = "ld";
const char *rd = "rd";
Blinds ldoor(LDPOW, LDUD, LDTIME, ld, true), rdoor(RDPOW, RDUD, RDTIME, rd, true);
BlynkTimer timer;

uint16_t g_last_touch_raw_x;
uint16_t g_last_touch_raw_y;

char auth[] = "xxxx";

char ssid[] = "SSID";
char pass[] = "PSWD";

WiFiClient client;

const char server[] = "api.openweathermap.org";
String nameOfCity   = "Wroclaw,pl"; 
String apiKey       = "apikey"; 

int   status = WL_IDLE_STATUS;
float h, t, ph, pt;
int   weather, pweather;
byte  lpos, lcur_pos, rpos, rcur_pos;

unsigned long time_check; 

BLYNK_WRITE(V1) {
  ldoor.setPosition(param.asInt()); 
}

BLYNK_WRITE(V2) {
  rdoor.setPosition(param.asInt()); 
}

BLYNK_WRITE(V3) {
  rdoor.setPosition(param.asInt());
  ldoor.setPosition(param.asInt()); 
}

void myTimerEvent()
{
  bool change = false;
  makehttpRequest();
  if (ph!=h) change = true;
  if (pt!=t) change = true;
  if (pweather!=weather) change = true;
  if (change) {
    Blynk.virtualWrite(V5, t);
    Blynk.virtualWrite(V6, h);
    load_screen();
  }
  ph = h;
  pt = t;
  pweather = weather;   
}

void setup() {
  //Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(TIMEINT, myTimerEvent);
  lpos = ldoor.getPosition();
  lcur_pos = lpos;
  Blynk.virtualWrite(V1, lcur_pos);
  rpos = rdoor.getPosition();
  rcur_pos = rpos;
  Blynk.virtualWrite(V2, rcur_pos);
  delay(100);
  ts.begin();
  ts.setRotation(0);
  //while (!Serial && (millis() <= 1000));
  delay(400); //init
  tft.begin();
  tft.setRotation(3);
  delay(400);
  //Serial.print("tftx ="); Serial.print(tft.width()); Serial.print(" tfty ="); Serial.println(tft.height());
  if (!SPIFFS.begin()) {
    //Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  //Serial.println("\r\nInitialisation done.");
  //listFiles(); // Lists the files so you can see what is in the SPIFFS
  myTimerEvent();
  //printWiFiStatus();
  time_check - millis();
}

void loop() {
  Blynk.run();
  timer.run();
  ldoor.run();
  rdoor.run();
  uint16_t x, y;
  if (millis() - time_check > BTN_CHK) {
    if (ts.tirqTouched()) {
      if (ts.touched()) {
        getTouchPoint(&x, &y);
        //Serial.print("x ="); Serial.print(x); Serial.print(" y ="); Serial.println(y);
        x = 320 - x;
      }
      check_buttons(x, y);
    }
    time_check = millis();
    set_blinds();
  }  
}
