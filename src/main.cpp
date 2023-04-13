#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <SD.h>

#define FREQ 434.2f //RFM frequency
#define RFM_CS 4    //RFM chipselect pin
#define RFM_G0 3    //RFM interrupt pin
#define RFM_RST 2   //RFM reset pin
#define SD_CS 49    //SD chipselect pin
#define RFID_CS 7   //RFID chipselect pin
#define RFID_RST 6  //RFID reset pin
#define BUZPIN 48   //Buzzer enable pin
#define DATFILE "datatext.txt"

RH_RF95 rf96(RFM_CS, RFM_G0);
Sd2Card scard;
File coldata;

void setup()
{
  while (!Serial)
  {
    Serial.begin(9600);
  }

  Serial.println("\nStarting Initialization");
  tone(BUZPIN, 450);
  delay(500);
  noTone;
  
  //Reset the RFM
  pinMode(RFID_RST, OUTPUT);
  digitalWrite(RFID_RST, LOW);
  delay(100);
  digitalWrite(RFM_RST, HIGH);
  delay(100);

  //Initialize RFM
  Serial.println("Initializing RFM. Please wait...");
  if (rf96.init())
  {
    Serial.println("RFM initialized!");
  } else
  {
    Serial.println("Failed to initialize RFM. Try again.");
    while (true);
  }

    //Set the frequency that the RFM will use
  if (rf96.setFrequency(FREQ))
  {
    Serial.println("\nFrequency is set!(434.2MHz)");
  } else
  {
    Serial.println("\nFailed to set RFM frequency to 434.2MHz");
    while (true);
  }

  //Initialize SD reader
  Serial.println("Initializing SD card. Please wait...");

  if (scard.init(SPI_FULL_SPEED, SD_CS))
  {
    Serial.println("SD card initialized!");
  } else
  {
    Serial.println("Failed to initialize SD card. Try again.");
    while (true);
  }

  if (SD.exists(DATFILE))
  {
    SD.remove(DATFILE);
    coldata = SD.open(DATFILE, FILE_WRITE);
  }
}

void loop()
{
}