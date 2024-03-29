#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <SD.h>
#include <MFRC522.h>
#include <megalovania.h>
#include <rickroll.h>

#define FREQ 434.2f //RFM frequency
#define RFM_CS 4    //RFM chipselect pin
#define RFM_G0 3    //RFM interrupt pin
#define RFM_RST 2   //RFM reset pin
#define SD_CS 49    //SD chipselect pin
#define RFID_CS 7   //RFID chipselect pin
#define RFID_RST 6  //RFID reset pin
#define BUZPIN 48   //Buzzer enable pin
#define DATFILE "datatext.txt"
#define POI_TXT "poi.txt"

RH_RF95 rf96(RFM_CS, RFM_G0);
MFRC522 rfid(RFID_CS, RFID_RST);
File coldata;

bool gui = true;
bool targetslct = false;
bool musicunlock = true;
String poi;
char* packet;

void setup() {
  Serial.begin(9600);

  Serial.println("\nStarting Initialization");
  tone(BUZPIN, 450);
  delay(500);
  noTone(BUZPIN);
  
  //Reset the RFM
  pinMode(RFID_RST, OUTPUT);
  digitalWrite(RFID_RST, LOW);
  delay(100);
  digitalWrite(RFM_RST, HIGH);
  delay(100);

  //Initialize RFM
  Serial.println("Initializing RFM. Please wait...");
  if (rf96.init()) {
    Serial.println("RFM initialized!");
  } else {
    Serial.println("Failed to initialize RFM. Try again.");
    while (true);
  }

    //Set the frequency that the RFM will use
  if (rf96.setFrequency(FREQ)) {
    Serial.println("\nFrequency is set!(434.2MHz)");
  } else {
    Serial.println("\nFailed to set RFM frequency to 434.2MHz");
    while (true);
  }

  //Initialize SD reader
  Serial.println("Initializing SD card. Please wait...");

  if (SD.begin(SD_CS)) {
    Serial.println("SD card initialized!");
  } else {
    Serial.println("Failed to initialize SD card. Try again.");
    //while (true);
  }

  //Initialize the RFID
  Serial.println("Initializing the RFID. Please wait...");

  rfid.PCD_Init();    
  Serial.println("RFID initialization ended.");

  tone(BUZPIN, 900);
  delay(600);
  noTone(BUZPIN);
}

void loop() {
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf96.recv(buf, &len)) {
    packet = (char*)buf;
    if (packet[0] == 'P' && packet[1] == 'R' && packet[2] == 'b' && packet[3] == '_') {
      if (gui) {
        Serial.println(packet);
        Serial.println("RSSI: ");
        Serial.print(rf96.lastRssi(), DEC);
      }

      if (packet[4] == 'D') { 
        coldata = SD.open(DATFILE, FILE_WRITE);
        coldata.write((char*)buf);
        coldata.close();
      }
      
      if (packet[4] == 'P') {
        coldata = SD.open(POI_TXT, FILE_WRITE);
        coldata.println(packet);
        coldata.close();
      }
    }
    
  }

  while (!Serial.available() == 0) {
    if (Serial.readString() == "poi") {
      gui = false;
      coldata = SD.open(POI_TXT);
      Serial.write(coldata.read());
      coldata.close();
    }

    if (Serial.readString() == "resume") {
      gui = true;
    }

    if (Serial.readString() == "target")
    {
      targetslct = true;
    }
    
  }

  while (targetslct)
  {
    Serial.println("Select where to land:");
    while (!Serial.available() == 0)
    {
      poi = Serial.readString();
      uint8_t poi_def[] = "PRb_PoI", poi;
      rf96.send(poi_def, sizeof(poi_def));
    }
  }
  

  if (rfid.PICC_IsNewCardPresent() & musicunlock) {
    rickroll(BUZPIN);
  }
}