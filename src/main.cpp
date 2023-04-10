#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

#define FREQ 434.2f     //RFM frequency
#define RFM_CS 4        //RFM chipselect pin
#define RFM_G0 3        //RFM interrupt pin
#define RFM_RST 2       //RFM reset pin

RH_RF95 rf96(RFM_CS, RFM_G0);

void setup() {
  Serial.begin(9600);
  
  Serial.println("\nStarting Initialization");
  
  //Reset the RFM
  pinMode(RFM_RST, OUTPUT);
  digitalWrite(RFM_RST, LOW);
  delay(100);
  digitalWrite(RFM_RST, HIGH);
  delay(100);

  //Initialize RFM
  Serial.println("Initializing RFM. Please wait...");
  if (rf96.init()) {
      Serial.println("RFM initialized!");
    } else {
      Serial.print("\nFailed to initialize RFM. Try again.");
      while (true);
    }

    //Set the frequency that the RFM will use
  if (rf96.setFrequency(FREQ)) {
    Serial.println("\nFrequency is set!(434.2MHz)");
  } else {
    Serial.println("\nFailed to set RFM frequency to 434.2MHz");
    while (true);
  }

}

void loop() {
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf96.recv(buf, &len))
  {
    Serial.println((char*)buf);
    Serial.print("\n");

    Serial.print("RSSI: ");
    Serial.println(rf96.lastRssi(), DEC);
  }
}