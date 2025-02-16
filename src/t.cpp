// #include <Arduino.h>
// #include <SPI.h>
// #include <MFRC522.h>
// #include <EEPROM.h>
// #include <WiFi.h>
// #include <WebServer.h>
// #include <sstream>

// #define RFID_RST_PIN 2
// #define RFID_SS_PIN 5
// #define BUTTON_PIN 4

// #define LED_PIN 12
// #define RELAY_PIN 14

// MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);

// const int MAX_UID_COUNT = 10;
// const int EEPROM_ADDR = 0;
// const int UID_LENGTH = 17;
// char eepromBuffer[UID_LENGTH];

// String uidArr[MAX_UID_COUNT];
// int uidCount = 0;

// const String EMPTY_UID = "00:00:00:00";

// const unsigned long relayDuration = 3000;

// WebServer server(80);

// String webpage = "";

// void initializeEEPROM() {
//   for (int i = 0; i < MAX_UID_COUNT; i++) {
//     int address = EEPROM_ADDR + i * UID_LENGTH;
//     EMPTY_UID.toCharArray(eepromBuffer, UID_LENGTH);
//     for (int j = 0; j < UID_LENGTH; j++) {
//       EEPROM.write(address + j, eepromBuffer[j]);
//     }
//   }
//   EEPROM.commit();
//   Serial.println("EEPROM initialized");
// }

// void loadUIDsFromEEPROM() {
//   uidCount = 0;
//   for (int i = 0; i < MAX_UID_COUNT; i++) {
//     int address = EEPROM_ADDR + i * UID_LENGTH;
//     for (int j = 0; j < UID_LENGTH; j++) {
//       eepromBuffer[j] = EEPROM.read(address + j);
//     }
//     String uid = String(eepromBuffer);
//     uid.trim();

//     if (uid != EMPTY_UID && uid.length() > 0) {
//       uidArr[uidCount] = uid;
//       uidCount++;
//     }
//   }
//   Serial.println("UIDs loaded from EEPROM:");
//   for (int i = 0; i < uidCount; i++) {
//     Serial.print("UID ");
//     Serial.print(i + 1);
//     Serial.print(": ");
//     Serial.println(uidArr[i]);
//   }
//   Serial.println();
// }

// void saveUIDsToEEPROM() {
//   for (int i = 0; i < uidCount; i++) {
//     int address = EEPROM_ADDR + i * UID_LENGTH;
//     uidArr[i].toCharArray(eepromBuffer, UID_LENGTH);
//     for (int j = 0; j < UID_LENGTH; j++) {
//       EEPROM.write(address + j, eepromBuffer[j]);
//     }
//   }
  
//   for (int i = uidCount; i < MAX_UID_COUNT; i++) {
//     int address = EEPROM_ADDR + i * UID_LENGTH;
//     EMPTY_UID.toCharArray(eepromBuffer, UID_LENGTH);
//     for (int j = 0; j < UID_LENGTH; j++) {
//         EEPROM.write(address + j, eepromBuffer[j]);
//     }
//   }
//   EEPROM.commit();
//   Serial.println("UIDs saved to EEPROM");
// }

// void addNewUID(String newUID) {
//     if (newUID == EMPTY_UID) {
//         Serial.println("Invalid UID. UID can't be 00:00:00:00");
//         return;
//     }

//   for (int i = 0; i < uidCount; i++) {
//     if (uidArr[i] == newUID) {
//       Serial.println("This card is already saved");
//       return;
//     }
//   }

//   if (uidCount < MAX_UID_COUNT) {
//     uidArr[uidCount] = newUID;
//     uidCount++;
//     saveUIDsToEEPROM();
//     Serial.print("Card successfully added: ");
//     Serial.println(newUID);
//   } else {
//     Serial.println("Card list already full!");
//   }
// }

// void updateWebpage() {
//   webpage = R"=====(
// <!DOCTYPE html>
// <html>
// <head>
//   <title>ESP32 RFID Control</title>
//   <meta charset="UTF-8">
//   <meta name="viewport" content="width=device-width, initial-scale=1.0">
//   <style>
//     body { font-family: sans-serif; }
//     table { width: 100%; border-collapse: collapse; }
//     th, td { padding: 8px; border: 1px solid #ddd; text-align: left; }
//     th { background-color: #f2f2f2; }
//   </style>
// </head>
// <body>
//   <h1>Allowed RFID List</h1>
//   <table>
//     <tr><th>UID</th><th>Action</th></tr>
// )=====";

//   for (int i = 0; i < uidCount; i++) {
//     webpage += "<tr><td>" + uidArr[i] + "</td><td><button onclick=\"deleteRFID('" + uidArr[i] + "')\">Delete</button></td></tr>";
//   }

//   webpage += R"=====(
//   </table>
//   <script>
//     function deleteRFID(uid) {
//       window.location.href = "/delete?uid=" + uid;
//     }
//   </script>
// </body>
// </html>
// )=====";
// }

// void handleRoot() {
//   updateWebpage();
//   server.send(200, "text/html", webpage);
// }

// void handleDelete() {
//   String uidToDelete = server.arg("uid");
//   if (uidToDelete.length() > 0) {
//     for (int i = 0; i < uidCount; i++) {
//       if (uidArr[i] == uidToDelete) {
//         for (int j = i; j < uidCount - 1; j++) {
//           uidArr[j] = uidArr[j + 1];
//         }
//         uidCount--;

//         for (int k = uidCount; k < MAX_UID_COUNT; k++) {
//           uidArr[k] = EMPTY_UID;
//         }

//         saveUIDsToEEPROM();
//         Serial.print("RFID deleted: ");
//         Serial.println(uidToDelete);

//         updateWebpage();
//         server.sendHeader("Location", "/");
//         server.send(302, "text/plain", "");
//         return;
//       }
//     }
//     Serial.println("RFID not found for deletion");
//   } else {
//     Serial.println("No UID specified for deletion");
//   }

//     updateWebpage();
//     server.sendHeader("Location", "/");
//     server.send(302, "text/plain", "");
// }

// void setup() {
//   Serial.begin(115200);
//   SPI.begin();
//   rfid.PCD_Init();
//   EEPROM.begin(512);

//   pinMode(BUTTON_PIN, INPUT_PULLUP);
//   pinMode(RELAY_PIN, OUTPUT);
//   pinMode(LED_PIN, OUTPUT);

//   digitalWrite(RELAY_PIN, LOW);
//   digitalWrite(LED_PIN, LOW);

//   Serial.println("\n");

//   loadUIDsFromEEPROM();

//   WiFi.softAP("EspAP", "12345678");
//   IPAddress IP = WiFi.softAPIP();
//   Serial.print("AP IP address: ");
//   Serial.println(IP);

//   server.on("/", handleRoot);
//   server.on("/delete", handleDelete);
//   server.begin();
//   delay(1000);
//   Serial.println("HTTP server started");
//   Serial.println();

//   Serial.println("Place RFID card/chip to unlock");
//   Serial.println("Press the button and place a new card/chip to add it to the allowed list");
//   Serial.println();
// }

// void loop() {
//   server.handleClient();

//   if (!rfid.PICC_IsNewCardPresent()) {
//     return;
//   }

//   if (!rfid.PICC_ReadCardSerial()) {
//     return;
//   }

//   String uid = "";
//   for (byte i = 0; i < rfid.uid.size; i++) {
//     uid += String(rfid.uid.uidByte[i], HEX);
//     if (i < rfid.uid.size - 1) {
//       uid += ":";
//     }
//   }

//   bool isValid = false;
//   for (int i = 0; i < uidCount; i++) {
//     if (uidArr[i] == uid) {
//       isValid = true;
//       break;
//     }
//   }

//   if (isValid) {
//     Serial.print("Unlocked! UID: ");
//     Serial.println(uid);

//     digitalWrite(RELAY_PIN, HIGH);
//     Serial.println("Relay On");

//     delay(relayDuration);

//     digitalWrite(RELAY_PIN, LOW);
//     Serial.println("Relay off");
//   } else {
//     Serial.print("Invalid Card! UID: ");
//     Serial.println(uid);

//     for (int i = 0; i < 3; i++) {
//       digitalWrite(LED_PIN, HIGH);
//       delay(200);
//       digitalWrite(LED_PIN, LOW);
//       delay(200);
//     }
//   }

//   if (digitalRead(BUTTON_PIN) == LOW) {
//     addNewUID(uid);
//   }

//   rfid.PICC_HaltA();
//   delay(1000);
// }