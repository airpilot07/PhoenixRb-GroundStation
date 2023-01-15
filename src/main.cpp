#include <Arduino.h>

#include <SPI.h>
#include <SD.h>


Sd2Card scard;
SdVolume sdsize;
SdFile root;

const int chipselect = 8;

void setup() {
  Serial.begin(9600);

  //Wait for serial port to connect
  while (!Serial)
  {
    delay(100);
  }
  
  //Initialize SD reader
  Serial.print("\nInitializing SD card. Please wait...");
  if (!scard.init(SPI_HALF_SPEED, chipselect))
  {
    Serial.println("\nFailed to initialize SD card. Try again.");
    while (true);
    
  } else {
    Serial.println("SD card initialized!");
  }

  Serial.println();
  Serial.print("Card type:   ");
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

  Serial.print("Clusters:   ");
  Serial.println(sdsize.clusterCount());
  Serial.print("Blocks x Cluster:   ");
  Serial.println(sdsize.blocksPerCluster());
  Serial.print("Total Blocks:   ");
  Serial.println(sdsize.blocksPerCluster() * sdsize.clusterCount());
  Serial.println();

  uint32_t volumesize;
  Serial.print("Volume type is:   FAT");
  Serial.println(sdsize.fatType(), DEC);
  volumesize = sdsize.blocksPerCluster();
  volumesize *= sdsize.clusterCount();
  volumesize /= 2;
  Serial.print("Volume size (Kb):   ");
  Serial.println(volumesize);
  Serial.print("Volume size (Mb):   ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Gb):   ");
  Serial.println((float)volumesize / 1024.0);
  Serial.println("\nFiles found on the card (name, date and size in bytes):   ");
  root.openRoot(sdsize);
  root.ls(LS_R | LS_DATE | LS_SIZE);
  root.close();
  
}

void loop() {
  // put your main code here, to run repeatedly:
}