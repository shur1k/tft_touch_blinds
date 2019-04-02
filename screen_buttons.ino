void load_screen() {
  if (weather < 299) drawRaw("/thunderstorm.raw", 0, 0, 320, 240);
  else if (weather < 399) drawRaw("/drizzle.raw", 0, 0, 320, 240);
  else if (weather < 599) drawRaw("/rain.raw", 0, 0, 320, 240);
  else if (weather < 699) drawRaw("/snow.raw", 0, 0, 320, 240);
  else if (weather < 799) drawRaw("/atmo.raw", 0, 0, 320, 240);
  else if (weather == 800) drawRaw("/sun2.raw", 0, 0, 320, 240);
  else if (weather < 803) drawRaw("/cloudsl.raw", 0, 0, 320, 240);
  else drawRaw("/clouds.raw", 0, 0, 320, 240);
  tft.setFont();
  bLDDN.initButton(&tft, 40, 20, 70, 40, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "Down", 2);
  bLDDN.drawButton();
  bLD40.initButton(&tft, 110, 20, 35, 35, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "40", 2);
  bLD40.drawButton();
  bLD60.initButton(&tft, 155, 20, 35, 35, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "60", 2);
  bLD60.drawButton();
  bLD80.initButton(&tft, 200, 20, 35, 35, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "80", 2);
  bLD80.drawButton();  
  bLDUP.initButton(&tft, 270, 20, 70, 40, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "UP", 2);
  bLDUP.drawButton();
  bRDDN.initButton(&tft, 40, 80, 70, 40, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "Down", 2);
  bRDDN.drawButton();
  bRD40.initButton(&tft, 110, 80, 35, 35, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "40", 2);
  bRD40.drawButton();
  bRD60.initButton(&tft, 155, 80, 35, 35, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "60", 2);
  bRD60.drawButton();
  bRD80.initButton(&tft, 200, 80, 35, 35, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "80", 2);
  bRD80.drawButton();    
  bRDUP.initButton(&tft, 270, 80, 70, 40, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "UP", 2);
  bRDUP.drawButton();
  bBDDN.initButton(&tft, 40, 140, 70, 40, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "Down", 2);
  bBDDN.drawButton();
  bBD40.initButton(&tft, 110, 140, 35, 35, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "40", 2);
  bBD40.drawButton();
  bBD60.initButton(&tft, 155, 140, 35, 35, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "60", 2);
  bBD60.drawButton();
  bBD80.initButton(&tft, 200, 140, 35, 35, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "80", 2);
  bBD80.drawButton();    
  bBDUP.initButton(&tft, 270, 140, 70, 40, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "UP", 2);
  bBDUP.drawButton();
  tft.fillRect(0, 45, lcur_pos*3.2, 10, ILI9341_YELLOW);
  tft.fillRect(lcur_pos*3.2, 45, 320, 10, ILI9341_BLACK);
  tft.fillRect(0, 105, rcur_pos*3.2, 10, ILI9341_YELLOW);
  tft.fillRect(rcur_pos*3.2, 105, 320, 10, ILI9341_BLACK);
  tft.setFont(&FreeMonoBoldOblique9pt7b);
  tft.setCursor(0, 190);
  tft.setTextColor(ILI9341_YELLOW);
  //tft.setTextSize(12);
  tft.print("Temp    :"); tft.print(t,1); tft.println("C");
  tft.print("Humidity:"); tft.print(h,1); tft.println("%");
}

boolean getTouchPoint(word *pwX, word *pwY)
{
  TS_Point pt;
  pt = ts.getPoint();
  g_last_touch_raw_x = pt.x;
  g_last_touch_raw_y = pt.y;
  // Scale from ~0->4000 to tft.width using the calibration #'s
#ifdef SCREEN_ORIENTATION_1
  *pwX = map(pt.y, TS_MINX, TS_MAXX, tft.width(), 0);
  *pwY = map(pt.x, TS_MINY, TS_MAXY, 0, tft.height());
#else
  *pwX = map(pt.x, TS_MINX, TS_MAXX, tft.width(), 0);
  *pwY = map(pt.y, TS_MAXY, TS_MINY, 0, tft.height());
#endif
  return true;
}

void check_buttons(uint16_t x, uint16_t y){
    tft.setFont();
    bLDDN.press(bLDDN.contains(x, y)); // tell the button it is pressed
    if (bLDDN.justReleased()) { bLDDN.drawButton(); bLDDNf = true;}
    if (bLDDN.justPressed())  { bLDDN.drawButton(true); }

    bLDUP.press(bLDUP.contains(x, y)); // tell the button it is pressed
    if (bLDUP.justReleased()) { bLDUP.drawButton();}
    if (bLDUP.justPressed())  { bLDUP.drawButton(true); bLDUPf = true;}

    bRDDN.press(bRDDN.contains(x, y)); // tell the button it is pressed
    if (bRDDN.justReleased()) { bRDDN.drawButton(); bRDDNf = true;}
    if (bRDDN.justPressed())  { bRDDN.drawButton(true); }

    bRDUP.press(bRDUP.contains(x, y)); // tell the button it is pressed
    if (bRDUP.justReleased()) { bRDUP.drawButton(); bRDUPf = true;}
    if (bRDUP.justPressed())  { bRDUP.drawButton(true); }
    
    bBDDN.press(bBDDN.contains(x, y)); // tell the button it is pressed
    if (bBDDN.justReleased()) { bBDDN.drawButton(); bBDDNf = true;}
    if (bBDDN.justPressed())  { bBDDN.drawButton(true); }

    bBDUP.press(bBDUP.contains(x, y)); // tell the button it is pressed
    if (bBDUP.justReleased()) { bBDUP.drawButton(); bBDUPf = true;}
    if (bBDUP.justPressed())  { bBDUP.drawButton(true); }

    bLD40.press(bLD40.contains(x, y)); // tell the button it is pressed
    if (bLD40.justReleased()) { bLD40.drawButton(); bLD40f = true;}
    if (bLD40.justPressed())  { bLD40.drawButton(true); }

    bLD60.press(bLD60.contains(x, y)); // tell the button it is pressed
    if (bLD60.justReleased()) { bLD60.drawButton(); bLD60f = true;}
    if (bLD60.justPressed())  { bLD60.drawButton(true); }

    bLD80.press(bLD80.contains(x, y)); // tell the button it is pressed
    if (bLD80.justReleased()) { bLD80.drawButton(); bLD80f = true;}
    if (bLD80.justPressed())  { bLD80.drawButton(true); }

    bRD40.press(bRD40.contains(x, y)); // tell the button it is pressed
    if (bRD40.justReleased()) { bRD40.drawButton(); bRD40f = true;}
    if (bRD40.justPressed())  { bRD40.drawButton(true); }

    bRD60.press(bRD60.contains(x, y)); // tell the button it is pressed
    if (bRD60.justReleased()) { bRD60.drawButton(); bRD60f = true;}
    if (bRD60.justPressed())  { bRD60.drawButton(true); }

    bRD80.press(bRD80.contains(x, y)); // tell the button it is pressed
    if (bRD80.justReleased()) { bRD80.drawButton(); bRD80f = true;}
    if (bRD80.justPressed())  { bRD80.drawButton(true); }

    bBD40.press(bBD40.contains(x, y)); // tell the button it is pressed
    if (bBD40.justReleased()) { bBD40.drawButton(); bBD40f = true;}
    if (bBD40.justPressed())  { bBD40.drawButton(true); }

    bBD60.press(bBD60.contains(x, y)); // tell the button it is pressed
    if (bBD60.justReleased()) { bBD60.drawButton(); bBD60f = true;}
    if (bBD60.justPressed())  { bBD60.drawButton(true); }

    bBD80.press(bBD80.contains(x, y)); // tell the button it is pressed
    if (bBD80.justReleased()) { bBD80.drawButton(); bBD80f = true;}
    if (bBD80.justPressed())  { bBD80.drawButton(true); }	
}