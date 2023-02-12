#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <SD.h>

#define Freq 434.2f     //RFM frequency
#define rfmSelecPin 4   //RFM chipselect pin
#define rfmIntPin 3     //RFM interrupt pin
#define buzPin 5        //Buzzer enable pin
#define datfile "datatext.txt"

RH_RF95 rf96(rfmSelecPin, rfmIntPin);
Sd2Card scard;
SdVolume sdsize;
File coldata;

void setup() {
  Serial.begin(9600);

  Serial.println("\nStarting Initialization");
  tone(buzPin, 4500, 5000);
  
  //Wait for serial port to connect
  while (!Serial);

  //Reset the RFM
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  delay(100);
  digitalWrite(2, HIGH);
  delay(100);

  //Initialize RFM
  Serial.println("Initializing RFM. Please wait...");
  if (rf96.init())
    {
      Serial.println("RFM initialized!");
    } else{
      Serial.print("\nFailed to initialize RFM. Try again.");
      while (true);
    }

    //Set the frequency that the RFM will use
    if (rf96.setFrequency(Freq))
    {
      Serial.println("\nFrequency is set!(434.2MHz)");
    } else{
      Serial.println("\nFailed to set RFM frequency to 434.2MHz");
      while (true);
    }

    rf96.setModeRx();  //set the RFM to recieve only mode

  //Initialize SD reader
  Serial.print("\nInitializing SD card. Please wait...");
  if (scard.init(100, 7))
  {
    Serial.println("SD card initialized!");
  } else{
    Serial.println("Failed to initialize SD card. Try again.");
    while (true);
  }

  Serial.println("Card type:   ");
  switch (scard.type())
  {
   case SD_CARD_TYPE_SD1:
    Serial.println("SD1");
    break;
  
   case SD_CARD_TYPE_SD2:
    Serial.println("SD2");
    break;

   case SD_CARD_TYPE_SDHC:
    Serial.println("SDHC");
    break;

   default:
    Serial.println("Unknown");
    break;
  }

  if (!sdsize.init(scard))
  {
    Serial.println("Could not find Fat16/Fat32 partition. \nMake sure the SD card is formatted");
    while (true);
  }
 uint32_t volumesize;
 Serial.print("Volume type is:   FAT");
 Serial.println(sdsize.fatType(), DEC);
 volumesize = sdsize.blocksPerCluster();
 volumesize *= sdsize.clusterCount();
 volumesize /= 1024;
 Serial.print("Volume size (Gb):   ");
 Serial.println((float)volumesize / 1024);

if (SD.exists(datfile))
{
  SD.remove(datfile);
}
 coldata = SD.open(datfile, FILE_WRITE);
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
    
    if (coldata)
    {
      coldata.println((char)buf);
    } else{
     Serial.println("data.txt file wasn' t able to open");
    }
    
  }
  
}