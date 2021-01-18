#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <xOD01.h>

const char* ssid = "######";               // your network SSID (name)
const char* pass = "######";             // your network password
char servername[]= "celestrak.com";           // Celestrak Server
xOD01 OD01;

WiFiClient client;

void setup() {
  Serial.begin(115200);
 
    // Starts the I2C communication
  #ifdef ESP8266
  Wire.pins(2, 14);
  #endif
  Wire.begin(); 
 
  OD01.begin();
  OD01.println("Attempting to connect to WiFi");
  WiFi.begin(ssid, pass);
  while ( WiFi.status() != WL_CONNECTED) {
    delay(1000);
    OD01.println("...");
  }

    OD01.println("Connected to wifi");
    OD01.println("\nStarting connection with server...");

    makeRequest();
}

void makeRequest(){
    // if you get a connection, report back via serial:
    if (client.connect(servername, 80)) {
    OD01.println("connected to server");
    Serial.println();
    Serial.print("TLE for: ");
    // Make HTTP request:
    client.println("GET /NORAD/elements/iridium-33-debris.txt HTTP/1.0");     // rest of url for your chosen txt file, i.e extension following celestrak.com , Replace everything EXCEPT: GET HTTP/1.0
    client.println();                                                       
    }
    
   // if there are incoming bytes available
   // from the server, read them and print them:
  char c;
  int lineCounter=0; 
 while (!client.available()){
  // while loop runs while waiting for server availability
 }

// Skip HTTP headers
 char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders))
  {
    Serial.println(F("Invalid response"));
    return;
  }

 while (client.available()) {
    c = client.read();
    Serial.print(c);
    
    if (c == '\n'){
      lineCounter = lineCounter+1;
    }

    if (lineCounter==3){
      client.stop();
      break;
    }
  }

  // if the server becomes disconnected, stop the client:
  if (!client.connected()) {
    //Serial.println();
    OD01.println("disconnecting from server");
    client.stop();
  }
}

void loop() {
}
