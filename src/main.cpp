#include <Arduino.h>

#include <SPI.h>
#include <RH_RF95.h>
#include <SD.h>

#define Freq 434.2F

RH_RF95 rf96;
Sd2Card scard;
SdVolume sdsize;
SdFile root;

const int RFMpin = 4;
const int chipselect = 8;
void setup() {

pinMode(4, OUTPUT);
//!pinMode(51, );
digitalWrite(4, HIGH);


  Serial.begin(9600);

  Serial.print("\nStarting Initialization");

  //Wait for serial port to connect
  while (!Serial)
  {
    delay(100);
  }

  //Initialize RFM
  Serial.print("\nInitializing RFM. Please wait...");
  if (!rf96.init())
    {
    Serial.println("\nFailed to initialize RFM. Try again.");
    while (true);
    } else{
      Serial.println("\nRFM initialized!");
    }

 rf96.setModeRx();

    //Set the frequency that the RFM will use
    rf96.setFrequency(434.2);
    if (!rf96.setFrequency(Freq))
    {
      Serial.println("\nFailed to set RFM frequency to 434.2MHz");
      while (true);
    } else{
      Serial.println("\nFrequency is set!(434.2MHz)");
    }

    //Set encryption
    //uint8_t key[] = {};   //! Not working with this LoRa RFM only RFM 69
    //rf96.setEncryptionKey(key);
    

  
  //Initialize SD reader
  Serial.print("\nInitializing SD card. Please wait...");
  if (!scard.init(SPI_HALF_SPEED, chipselect))
  {
    Serial.println("\nFailed to initialize SD card. Try again.");
    while (true);

  } else{
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
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf96.waitAvailableTimeout(500))
  {
    if (rf96.recv(buf, &len))
    {
      Serial.print("\nRecieved data");
      Serial.println((char*)buf);
    } else{
      Serial.println("\nNo data recieved");
    }
    
  } else{
    Serial.println("fail. Is CanSat online?");
  }

//datafile = SD.open(const String = Data.txt, uint8_t mode = )


  delay(400);
  
}