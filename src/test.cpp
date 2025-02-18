
#include <RtcDS1307.h>
RtcDS1307<TwoWire> Rtc(Wire);

#include <DHT22.h>
#define pinDATA 13
DHT22 dht22(pinDATA);


int currentHour = 0;
float totalDailyTemp[3];
float totalDailyHumid[3];

int eepromStartAddress;

int currentDayIndex = 0;
float tempAverages[7][3] = {
    {20.5, 25.0, 18.2}, {21.0, 26.5, 19.0}, {22.2, 27.8, 20.1},
    {23.1, 28.5, 21.0}, {24.0, 29.2, 21.8}, {24.8, 29.8, 22.5},
    {25.5, 30.5, 23.2}
}; // remove data after first use
float humidAverages[7][3] = {
    {60.2, 55.8, 65.1}, {62.5, 58.0, 68.0}, {65.0, 60.5, 70.2},
    {68.2, 63.1, 72.5}, {70.8, 65.5, 75.0}, {73.5, 68.0, 77.2},
    {76.0, 70.2, 79.5}
}; // remove data after first use


void storeDataToEeprom() {
    int address = eepromStartAddress;

    EEPROM.put(address, currentDayIndex);
    address += sizeof(int);

    address += currentDayIndex * 3 * sizeof(float);

    for (int i = 0; i < 3; i++) {
        EEPROM.put(address, totalDailyTemp[i] / 8);
        address += sizeof(float);
    }

    address += 19 * sizeof(float);

    for (int i = 0; i < 3; i++) {
        EEPROM.put(address, totalDailyTemp[i] / 8);
        address += sizeof(float);
    }
}

void readDataFromEeprom() {
    int address = eepromStartAddress;

    EEPROM.get(address, currentDayIndex);
    address += sizeof(int);

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            EEPROM.get(address, tempAverages[i][j]);
            address += sizeof(float);
        }
    }

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            EEPROM.get(address, humidAverages[i][j]);
            address += sizeof(float);
        }
    }
}

void addUpDataToDailyTotal(int p, float t, float h) {
    totalDailyTemp[p] += t;
    totalDailyHumid[p] += h;
}


void setup() {
    Serial.begin(115200);

    Serial.println("Hi");

    Wire.begin(8,9);
    Rtc.Begin();

    if (!Rtc.IsDateTimeValid()) {
        RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
        Rtc.SetDateTime(compiled);
    }

    if (!Rtc.GetIsRunning()) {
        Rtc.SetIsRunning(true);
    }

    // readDataFromEeprom(); // for the first use the data have to be stored first, then uncomment this
}

void loop() {
    RtcDateTime dt = Rtc.GetDateTime();
    int hour = dt.Hour();

    float t = dht22.getTemperature();
    float h = dht22.getHumidity();
    
  
    if (currentHour != hour) {
        currentHour = dt.Hour();

        int p;
        if (currentHour <= 8 && currentHour != 0) { p = 0; }
        else if (currentHour <= 16) { p = 1; }
        else if (currentHour <= 23 && currentHour == 0) { p = 2; }
        addUpDataToDailyTotal(p, t, h);

        if (hour == 0) {
            storeDataToEeprom();
            readDataFromEeprom();

            currentDayIndex = (currentDayIndex++) % 7;
        }
    }

    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println(currentDayIndex);
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 3; j++) {
        Serial.print(tempAverages[i][j]);
        Serial.print(" ");
        Serial.print(tempAverages[i][j]);
        Serial.print(" ");
        Serial.println(tempAverages[i][j]);
      }
    }
    Serial.println();
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 3; j++) {
        Serial.print(humidAverages[i][j]);
        Serial.print(" ");
        Serial.print(humidAverages[i][j]);
        Serial.print(" ");
        Serial.println(humidAverages[i][j]);
      }
    }
  
    delay(1000);
  }