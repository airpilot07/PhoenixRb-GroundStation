#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <SD.h>

#define FREQ 434.2f     //RFM frequency
#define RFM_CS 4   //RFM chipselect pin
#define RFM_G0 3     //RFM interrupt pin
#define BUZPIN 5        //Buzzer enable pin
#define DATFILE "datatext.txt"

RH_RF95 rf96(RFM_CS, RFM_G0);
Sd2Card scard;
File coldata;

void setup() {
  while (!Serial)
  {
    Serial.begin(9600);
  }
  
  Serial.println("\nStarting Initialization");
  tone(BUZPIN, 4500, 5000);
  
  //Reset the RFM
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  delay(100);
  digitalWrite(2, HIGH);
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

  rf96.setModeRx();  //set the RFM to recieve only mode

  //Initialize SD reader
  Serial.print("\nInitializing SD card. Please wait...");
  if (scard.init(100, 7)) {
    Serial.println("SD card initialized!");
  } else {
    Serial.println("Failed to initialize SD card. Try again.");
    while (true);
  }

  if (SD.exists(DATFILE)) {
    SD.remove(DATFILE);
  }
  coldata = SD.open(DATFILE, FILE_WRITE);
}

void loop() {
  uint8_t buf[225];
  uint8_t len;
  if (rf96.recv(buf, &len)) {
    for (int i = 0; i < len; i++)
    {
      Serial.print((char)buf[i]);
    }
    Serial.print("\n");
    
    if (coldata) {
      coldata.println((char)buf);
    } else {
      Serial.println("data.txt file wasn' t able to open");
    }
    
  }
  
}