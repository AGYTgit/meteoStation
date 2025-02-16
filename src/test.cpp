float currentTempTotal = 0;
float currentHumidTotal = 0;
int numReadings = 0;

float avgTemp[3];
float avgHumid[3];

int previousHour;
int previousDay;


void addReading(float temp, float humid) {
  currentTempTotal += temp;
  currentHumidTotal += humid;
  numReadings++;
}

void resetTotals() {
    currentTempTotal = 0;
    currentHumidTotal = 0;
    numReadings = 0;
}

void calculateAndStoreAverage(int hour) {
    if (hour == 8) {
        avgTemp[0] = currentTempTotal / numReadings;
        avgHumid[0] = currentHumidTotal / numReadings;
        resetTotals();        
    }

    if (hour == 16) {
        avgTemp[1] = currentTempTotal / numReadings;
        avgHumid[1] = currentHumidTotal / numReadings;
        resetTotals();        
    }

    if (hour == 0) {
        avgTemp[2] = currentTempTotal / numReadings;
        avgHumid[2] = currentHumidTotal / numReadings;
        resetTotals();        
    }
}

int getCurrentHourFromRTC() {
  return 0; // Placeholder
}

void setup() {

}

void loop() {
    RtcDateTime dt = Rtc.GetDateTime();
    int hour = dt.Hour();
  
    if (hour != previousHour) {
        addReading(float temp, float humid);
        previousHour = dt.Hour();

        if (hour == 8 && hour == 16 && hour == 0) {
            calculateAndStoreAverage(hour);
        }
        if (dt.Day() != previousDay) {
            previousDay = dt.Day();
      }
    }
  
    // ... (Display logic)
  }