// print Wifi status
void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

// to request data from OWM
void makehttpRequest() {
  // close any connection before send a new request to allow client make connection to server
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 80)) {
    //Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /data/2.5/forecast?q=" + nameOfCity + "&APPID=" + apiKey + "&mode=json&units=metric&cnt=2 HTTP/1.1");
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
    
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        //Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    
    String payload = "";
	char c;
	bool startJSON = false;
    while (client.available()) {
      c = client.read();
	  if (c == '{') startJSON = true;
	  if (startJSON) payload += c;
    }
	
	//Serial.println("Payload:");
	//Serial.println(payload);
	
	DynamicJsonBuffer jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject((char*)payload.c_str()); 
	
	if (!json.success()) {
		//Serial.println("parse failed");
		return;
	}
	
    JsonArray& list = json["list"];
    JsonObject& nowT = list[0];
    JsonObject& later = list[1];
		
	t = nowT["main"]["temp"];
	h = nowT["main"]["humidity"];
	weather = nowT["weather"][0]["id"];

	float tempLater = later["main"]["temp"];
	float humidityLater = later["main"]["humidity"];
	String weatherLater = later["weather"][0]["id"];

	//Serial.println("Parse JSON");
	//Serial.print("TempNow: "); Serial.println(String(t));
	//Serial.print("HumidityNow: "); Serial.println(String(h));
	//Serial.print("WeatherNow: "); Serial.println(String(weather));
	//Serial.println();
  }
  else {
    // if no connction was made:
    //Serial.println("connection failed");
    return;
  }
}
