// #include <Arduino.h>
// #include <Wire.h>
// #include <EEPROM.h>

// #include <RtcDS1307.h>
// RtcDS1307<TwoWire> Rtc(Wire);

// #define NUM_DAYS 7
// #define NUM_PERIODS 3
// #define EEPROM_SIZE 512

// float tempAverages[NUM_DAYS][NUM_PERIODS];
// float humidAverages[NUM_DAYS][NUM_PERIODS];
// float totalDailyTemp[NUM_PERIODS];
// float totalDailyHumid[NUM_PERIODS];
// int sampleCounts[NUM_PERIODS];

// int currentDayIndex = 0;
// int currentHour = 0;
// int eepromStartAddress = 0;

// void storeDataToEeprom() {
//     int address = eepromStartAddress;

//     EEPROM.put(address, currentDayIndex);
//     address += sizeof(int);

//     for (int i = 0; i < NUM_DAYS; i++) {
//         for (int j = 0; j < NUM_PERIODS; j++) {
//             EEPROM.put(address, tempAverages[i][j]);
//             address += sizeof(float);
//             EEPROM.put(address, humidAverages[i][j]);
//             address += sizeof(float);
//         }
//     }

//     EEPROM.commit();
// }

// void readDataFromEeprom() {
//     int address = eepromStartAddress;

//     EEPROM.get(address, currentDayIndex);
//     address += sizeof(int);

//     for (int i = 0; i < NUM_DAYS; i++) {
//         for (int j = 0; j < NUM_PERIODS; j++) {
//             EEPROM.get(address, tempAverages[i][j]);
//             address += sizeof(float);
//             EEPROM.get(address, humidAverages[i][j]);
//             address += sizeof(float);
//         }
//     }
// }

// void addData(float t, float h, int hour) {
//     int period;
//     if (hour >= 0 && hour < 8) {
//         period = 0;
//     } else if (hour >= 8 && hour < 16) {
//         period = 1;
//     } else {
//         period = 2;
//     }

//     totalDailyTemp[period] += t;
//     totalDailyHumid[period] += h;
//     sampleCounts[period]++;
// }

// void calculateAverages(int period) {
//     if (sampleCounts[period] > 0) {
//         tempAverages[currentDayIndex][period] = totalDailyTemp[period] / sampleCounts[period];
//         humidAverages[currentDayIndex][period] = totalDailyHumid[period] / sampleCounts[period];
//     } else {
//         tempAverages[currentDayIndex][period] = 0;
//         humidAverages[currentDayIndex][period] = 0;
//     }
// }

// void resetDailyTotals() {
//     for (int i = 0; i < NUM_PERIODS; i++) {
//         totalDailyTemp[i] = 0;
//         totalDailyHumid[i] = 0;
//         sampleCounts[i] = 0;
//     }
// }

// void setup() {
//     Serial.begin(115200);
//     EEPROM.begin(EEPROM_SIZE);
//     Wire.begin();
//     Rtc.Begin();

//     if (!Rtc.IsDateTimeValid()) {
//         RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
//         Rtc.SetDateTime(compiled);
//     }

//     if (!Rtc.GetIsRunning()) {
//         Rtc.SetIsRunning(true);
//     }

//     readDataFromEeprom();
//     resetDailyTotals();
// }

// void loop() {
//     if (Serial.available() > 0) {
//         String input = Serial.readStringUntil('\n');
//         float t, h;
//         int hour;
//         sscanf(input.c_str(), "%f %f %d", &t, &h, &hour);

//         if (currentHour != hour) {
//             currentHour = hour;

//             addData(t, h, hour);

//             if (hour == 8 || hour == 16 || hour == 0) {
//                 int period = (hour == 0) ? 2 : (hour / 8) - 1;
//                 calculateAverages(period);

//                 if (hour == 0) {
//                     storeDataToEeprom();
//                     currentDayIndex = (currentDayIndex + 1) % NUM_DAYS;
//                     resetDailyTotals();
//                 }
//             }
//         }

//         Serial.println();
//         Serial.print("Day: ");
//         Serial.println(currentDayIndex);
//         Serial.println("Temperature Averages:");
//         for (int i = 0; i < NUM_DAYS; i++) {
//             for (int j = 0; j < NUM_PERIODS; j++) {
//                 Serial.print(tempAverages[i][j]);
//                 Serial.print(" ");
//             }
//             Serial.println();
//         }
//         Serial.println("Humidity Averages:");
//         for (int i = 0; i < NUM_DAYS; i++) {
//             for (int j = 0; j < NUM_PERIODS; j++) {
//                 Serial.print(humidAverages[i][j]);
//                 Serial.print(" ");
//             }
//             Serial.println();
//         }
//     }

//     delay(1000);
// }