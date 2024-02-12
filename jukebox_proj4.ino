
#include <SoftwareSerial.h>
#include <DFPlayerMini_Fast.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define PAUSE_PIN 3
#define RESUME_PIN 4
#define VOLUME_PIN A3

SoftwareSerial mySerial(6, 7); // RX, TX
DFPlayerMini_Fast myMP3;
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
int volume = 15; // initial volume setting

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  mySerial.begin(9600);
  myMP3.begin(mySerial);
  pinMode(PAUSE_PIN, INPUT_PULLUP);
  pinMode(RESUME_PIN, INPUT_PULLUP);
  analogWrite(VOLUME_PIN, volume); // set initial volume level

  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  Serial.println("Waiting for an RFID tag...");
}

void loop() {
  if (digitalRead(PAUSE_PIN) == LOW) { // pause button pressed
    myMP3.pause();
    while (digitalRead(PAUSE_PIN) == LOW) {} // wait for button release
  }

 
  volume = map(analogRead(VOLUME_PIN), 0, 1023, 0, 30); // read volume setting from potentiometer
  myMP3.volume(volume); // set volume to the read setting

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String cardID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      cardID += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println("RFID tag detected: " + cardID);
    if (cardID == "f32c4c2e") {
      Serial.println("Playing udja");
      myMP3.play(1);
      delay(2000); // Wait for the MP3 to start playing
    }
    if (cardID == "291287b9") {
      Serial.println("Playing song1");
      myMP3.play(2);
      delay(2000); // Wait for the MP3 to start playing
    }
    if (cardID == "8ad83b9") {
      Serial.println("Playing song2");
      myMP3.play(1);
      delay(2000); // Wait for the MP3 to start playing
    }
    if (cardID == "changexyz") {
      Serial.println("Playing song3");
      myMP3.play(2);
      delay(2000); // Wait for the MP3 to start playing
    }
    if (cardID == "changeabc") {
      Serial.println("Playing song4");
      myMP3.play(2);
      delay(2000); // Wait for the MP3 to start playing
    }
    else {
      Serial.println("Unauthorized RFID tag detected.");
    }
  }
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
