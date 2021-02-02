/*************************************************************
  This is an examples for the SW01
  Ambient, UVA and UVB light sensor
  
  You can buy one on our store!
  -----> https://xinabox.cc/SL01/
  
  This example request a UV meausrement to be 
  made by the SL01 and display the data over the Serial
  bus.
  
  The sensor communicates over the I2C Bus.
  
*************************************************************/
#include <xCore.h>
#include <xSL01.h>
#include <xSW01.h>
#include <xSI01.h>
#include <xSN01.h>

const int DELAY_TIME = 1000;

xSL01 SL01;
xSW01 SW01;
xSI01 SI01;
xSN01 SN01;

#define PRINT_SPEED 250
static unsigned long lastPrint = 0;

long tick_Print = 0;

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  
  // Set the I2C Pins for CW01
  #ifdef ESP8266
    Wire.pins(2, 14);
    Wire.setClockStretchLimit(15000);
  #endif
  
  // Start the I2C Comunication
  Wire.begin();
  
  // Start the  SL01 Sensor
  SL01.begin();

  // Start the  SW01 Sensor
  SW01.begin();

  // Start the SN01 Sensor
  SN01.begin();

  // Delayy for Sensor to normalise
  delay(DELAY_TIME);
  
  Serial.println("================================");
  Serial.println(" XINABOX SL01 LUX Measurements  ");
  Serial.println("================================"); 
}

void loop() {
  // Create a variable to store the incoming measurements
  // from SL01 sensor
  float uv;
  uv = 0;

  // Create a variable to store the data read from SN01
  String time;
  long latitude = 0;
  long longitude = 0;
  String date;

  // Poll Sensor for collect data
  SL01.poll();
  // Read and calculate data from SW01 sensor
  SW01.poll();
  //read data from SI01
  SI01.poll();
  // Poll the sensor to read all available data
  SN01.poll();

  // Request SL01 to return calculated UVB intensity
  uv = SL01.getUVA();
  // Display Data on the Serial monitor
  Serial.print("UVA Intersity: ");
  Serial.print(uv);
  Serial.println(" uW/m^2");
  
  // Request SL01 to return calculated UVB intensity
  uv = SL01.getUVB();
  // Display Data on the Serial monitor
  Serial.print("UVB Intensity: ");
  Serial.print(uv);
  Serial.println(" uW/m^2");

  // Request SL01 to return calculated UVB intensity
  uv = SL01.getUVIndex();
  // Display Data on the Serial monitor
  Serial.print("UVB Index: ");
  Serial.println(uv);

  Serial.println();

  delay(DELAY_TIME);

  // Create a variable to store the data read from SW01
  float alt;
  alt = 0;
  
  // Request SW01 to get the altitude measurement and store in
  // the altitude variable  
  alt = SW01.getAltitude(101325);
  
  // Display the recoreded data over the Serial Monitor 
  Serial.print("Altitude: ");
  Serial.print(alt);
  Serial.println(" m");

  if ( (lastPrint + PRINT_SPEED) < millis()) {
    printGyro();  // Print "G: gx, gy, gz"
    printAccel(); // Print "A: ax, ay, az"
    printMag();   // Print "M: mx, my, mz"
    printAttitude(); // Print Roll, Pitch and G-Force
    Serial.println();
    lastPrint = millis(); // Update lastPrint time
  }

     // Use a timer to print data once a second
    if((millis() - tick_Print) > 1000){

      // Get the date from GPS
    date = SN01.getDate();
    
    // Get the time from the GPS 
    time = SN01.getTime();

    // Get the latitude from GPS
    latitude = SN01.getLatitude();

    // Get the longitude from GPS
    longitude = SN01.getLongitude();
    
    // Display the recorded data over the serial monitor
    Serial.print("GPS Time: ");
    Serial.println(time);
    Serial.print("GPS Date: ");
    Serial.println(date);
    Serial.print("GPS Latitude: ");
    Serial.println(latitude);
    Serial.print("GPS longitude: ");
    Serial.println(longitude);

    Serial.println();
    
    tick_Print = millis();
  }
  
  // Small delay between sensor reads
  delay(DELAY_TIME);
}

void printGyro(void) {
  Serial.print("G: ");
  Serial.print(SI01.getGX(), 2);
  Serial.print(", ");
  Serial.print(SI01.getGY(), 2);
  Serial.print(", ");
  Serial.println(SI01.getGZ(), 2);

}

void printAccel(void) {
  Serial.print("A: ");
  Serial.print(SI01.getAX(), 2);
  Serial.print(", ");
  Serial.print(SI01.getAY(), 2);
  Serial.print(", ");
  Serial.println(SI01.getAZ(), 2);
}

void printMag(void) {
  Serial.print("M: ");
  Serial.print(SI01.getMX(), 2);
  Serial.print(", ");
  Serial.print(SI01.getMY(), 2);
  Serial.print(", ");
  Serial.println(SI01.getMZ(), 2);

}

void printAttitude(void) {
  Serial.print("Roll: ");
  Serial.println(SI01.getRoll(), 2);
  Serial.print("Pitch :");
  Serial.println(SI01.getPitch(), 2);
  Serial.print("GForce :");
  Serial.println(SI01.getGForce(), 2);
} 
