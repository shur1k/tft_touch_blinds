void set_blinds() {
  //-------------- send to Blynk while move --------------------
  if(ldoor.isMoving()) {
    lpos = ldoor.getPosition();
    if(lpos!=lcur_pos) {
      lcur_pos = lpos;
      Blynk.virtualWrite(V1, lcur_pos);
	    Blynk.virtualWrite(V3, (lcur_pos+rcur_pos)/2);
	    tft.fillRect(0, 45, lcur_pos*3.2, 10, ILI9341_YELLOW);
      tft.fillRect(lcur_pos*3.2, 45, 320, 10, ILI9341_BLACK);
    }
  }  

  if(rdoor.isMoving()) {
    rpos = rdoor.getPosition();
    if(rpos!=rcur_pos) {
      rcur_pos = rpos;
      Blynk.virtualWrite(V2, rcur_pos);
	    Blynk.virtualWrite(V3, (lcur_pos+rcur_pos)/2);
      tft.fillRect(0, 105, rcur_pos*3.2, 10, ILI9341_YELLOW);
      tft.fillRect(rcur_pos*3.2, 105, 320, 10, ILI9341_BLACK);	  
    }
  }  
  
  //------------------------- check the buttons -------------------------------
  
  //------------ Left Door ----------
  if(bLDDNf) {
    if(ldoor.isMoving()) {
      bLDDNf = false;
      ldoor.setPosition(ldoor.getPosition());  
    }
    else {
      bLDDNf = false; 
      ldoor.setPosition(0); 
    }
  }
  
  if(bLDUPf) {
    if(ldoor.isMoving()) {
      bLDUPf = false;
      ldoor.setPosition(ldoor.getPosition());  
    }
    else {
      bLDUPf = false; 
      ldoor.setPosition(100); 
    }
  }
  
  if(bLD40f) { bLD40f = false; ldoor.setPosition(40); }
  if(bLD60f) { bLD60f = false; ldoor.setPosition(60); }
  if(bLD80f) { bLD80f = false; ldoor.setPosition(80); }

  
  //------------------ Right door -------------
  if(bRDDNf) {
    if(rdoor.isMoving()) {
      bRDDNf = false;
      rdoor.setPosition(rdoor.getPosition());  
    }
    else {
      bRDDNf = false; 
      rdoor.setPosition(0); 
    }
  }
  
  if(bRDUPf) {
    if(rdoor.isMoving()) {
      bRDUPf = false;
      rdoor.setPosition(rdoor.getPosition());  
    }
    else {
      bRDUPf = false; 
      rdoor.setPosition(100); 
    }
  }

  if(bRD40f) { bRD40f = false; rdoor.setPosition(40); }
  if(bRD60f) { bRD60f = false; rdoor.setPosition(60); }
  if(bRD80f) { bRD80f = false; rdoor.setPosition(80); }

  //------------- Both Doors -----------------------------
  if(bBDDNf) {
    if((rdoor.isMoving())||(ldoor.isMoving())) {
      bBDDNf = false;
      rdoor.setPosition(rdoor.getPosition());
	  ldoor.setPosition(ldoor.getPosition());
    }
    else {
      bBDDNf = false; 
      rdoor.setPosition(0);
	    ldoor.setPosition(0);
    }
  }
  
  if(bBDUPf) {
    if((rdoor.isMoving())||(ldoor.isMoving())) {
      bBDUPf = false;
      rdoor.setPosition(rdoor.getPosition());
	    ldoor.setPosition(ldoor.getPosition());
    }
    else {
      bBDUPf = false; 
      rdoor.setPosition(100);
	    ldoor.setPosition(100);
    }
  }

  if(bBD40f) { bBD40f = false; rdoor.setPosition(40); ldoor.setPosition(40);}
  if(bBD60f) { bBD60f = false; rdoor.setPosition(60); ldoor.setPosition(60);}
  if(bBD80f) { bBD80f = false; rdoor.setPosition(80); ldoor.setPosition(80);}
}
