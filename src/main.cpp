#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <RtcDS1307.h>
RtcDS1307<TwoWire> Rtc(Wire);

#include <DHT22.h>
#define dhtData 13
DHT22 dht22(dhtData);

#include <WiFi.h>
#include <WebServer.h>
const char* ssid = "EspAP";
const char* password = "12345678";
WebServer server(80);
String webpage = "";

#define NUM_DAYS 7
#define NUM_PERIODS 3
#define EEPROM_SIZE 512

const char* dayNames[NUM_DAYS] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

float tempAverages[NUM_DAYS][NUM_PERIODS];
float humidAverages[NUM_DAYS][NUM_PERIODS];
float totalDailyTemp[NUM_PERIODS];
float totalDailyHumid[NUM_PERIODS];
int sampleCounts[NUM_PERIODS];

int currentDayIndex = 0;
int currentHour = -1;
int eepromStartAddress = 0;

int hour = 0;
int minute = 0;
int second = 0;

void updateWebpage() {
    webpage = R"=====(<!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>ESP32 Sensor Data</title>
            <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@400;700&display=swap" rel="stylesheet">
            <style>
                body {
                    font-family: 'Poppins', sans-serif;
                    background: linear-gradient(135deg, #1e3c72, #2a5298);
                    color: #fff;
                    margin: 0;
                    padding: 0;
                    display: flex;
                    flex-direction: column;
                    align-items: center;
                    justify-content: center;
                    height: 100vh;
                }
                .tab-buttons {
                    display: flex;
                    justify-content: center;
                    margin-bottom: 10px;
                    background-color: transparent;
                    padding: 10px;
                }
                .tab-button {
                    background-color: rgba(51, 102, 153, 0.8);
                    color: white;
                    border: none;
                    padding: 10px 20px;
                    margin: 0 5px;
                    border-radius: 5px;
                    cursor: pointer;
                    font-family: 'Poppins', sans-serif;
                    transition: background-color 0.3s ease;
                }
                .tab-button:hover {
                    background-color: rgba(51, 102, 153, 1);
                }
                .container {
                    background-color: rgba(255, 255, 255, 0.2);
                    padding: 20px;
                    border-radius: 10px;
                    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
                    text-align: center;
                    margin-top: 20px;
                }
                h1 {
                    color: #66b3ff;
                    margin-bottom: 20px;
                }
                p {
                    font-size: 1.2em;
                    margin-bottom: 10px;
                }
                .data-value {
                    font-weight: bold;
                }
                .tab-content {
                    display: none;
                }
                .active {
                    display: block;
                }
                #average table {
                    width: 100%;
                    border-collapse: collapse;
                    margin-top: 20px;
                }
                #average th,
                #average td {
                    border: 1px solid #ddd;
                    padding: 8px;
                    text-align: left;
                }
                #average th {
                    background-color: rgba(51, 102, 153, 0.7);
                    color: white;
                }
                #average tr:nth-child(even) {
                    background-color: rgba(255, 255, 255, 0.1);
                }
                .day-details {
                    display: none;
                }
                .day-details.active {
                    display: table-row-group;
                }
                .day-details tr {
                    background-color: rgba(255, 255, 255, 0.15);
                    color: #fff;
                }
                .day-details.active tr:hover {
                    background-color: rgba(255, 255, 255, 0.25);
                }
                #averageTableBody > tr {
                    transition: opacity 0.3s ease;
                }
                .day-details.active ~ tr {
                    opacity: 0.7;
                }
            </style>
        </head>
        <body>
            <div class="tab-buttons">
                <button class="tab-button active" onclick="openTab('current')">Current Data</button>
                <button class="tab-button" onclick="openTab('average')">Averages</button>
            </div>
            <div class="container">
                <h1>ESP32 Sensor Data</h1>
                <div id="current" class="tab-content active">
                    <p>Temperature: <span class="data-value" id="currentTemp"></span></p>
                    <p>Humidity: <span class="data-value" id="currentHum"></span></p>
                    <p>Time: <span class="data-value" id="currentTime"></span></p>
                </div>
                <div id="average" class="tab-content">
                    <table>
                        <thead>
                            <tr>
                                <th>Day of the Week</th>
                                <th>Temperature (°C)</th>
                                <th>Humidity (%)</th>
                            </tr>
                        </thead>
                        <tbody id="averageTableBody">
                            <!-- Data will be inserted here by updateWebpage function -->
                        </tbody>
                    </table>
                </div>
            </div>
            <script>
                function openTab(tabName) {
                    let i;
                    let tabContent = document.getElementsByClassName("tab-content");
                    let tabButtons = document.getElementsByClassName("tab-button");
                    for (i = 0; i < tabContent.length; i++) {
                        tabContent[i].style.display = "none";
                    }
                    for (i = 0; i < tabButtons.length; i++) {
                        tabButtons[i].className = tabButtons[i].className.replace(" active", "");
                    }
                    document.getElementById(tabName).style.display = "block";
                    event.currentTarget.className += " active";
                }
                function toggleDayDetails(dayId) {
                    const dayDetails = document.getElementById(dayId);
                    const allDayDetails = document.querySelectorAll('.day-details.active');
                    allDayDetails.forEach(openDetails => {
                        if (openDetails.id !== dayId) {
                            openDetails.classList.remove('active');
                        }
                    });
                    dayDetails.classList.toggle('active');
                }
                function fetchCurrentData() {
                    fetch('/currentData')
                        .then(response => response.json())
                        .then(data => {
                            document.getElementById('currentTemp').innerText = data.temperature + '°C';
                            document.getElementById('currentHum').innerText = data.humidity + '%';
                            document.getElementById('currentTime').innerText = data.time;
                        });
                }
                setInterval(fetchCurrentData, 1000);
            </script>
        </body>
    </html>)=====";

    String averageTableBody = "";
    for (int i = 0; i < NUM_DAYS; i++) {
        int dayIndex = (currentDayIndex - i + NUM_DAYS) % NUM_DAYS;
        averageTableBody += "<tr onclick=\"toggleDayDetails('day" + String(dayIndex) + "')\">";
        averageTableBody += "<td>" + String(dayNames[dayIndex]) + "</td>";
        averageTableBody += "<td>" + String(tempAverages[dayIndex][0]) + "</td>";
        averageTableBody += "<td>" + String(humidAverages[dayIndex][0]) + "</td>";
        averageTableBody += "</tr>";
        averageTableBody += "<tbody class=\"day-details\" id=\"day" + String(dayIndex) + "\">";
        for (int j = 0; j < NUM_PERIODS; j++) {
            averageTableBody += "<tr>";
            averageTableBody += "<td>Period " + String(j + 1) + "</td>";
            averageTableBody += "<td>" + String(tempAverages[dayIndex][j]) + "</td>";
            averageTableBody += "<td>" + String(humidAverages[dayIndex][j]) + "</td>";
            averageTableBody += "</tr>";
        }
        averageTableBody += "</tbody>";
    }
    webpage.replace("<!-- Data will be inserted here by updateWebpage function -->", averageTableBody);
}

void handleRoot() {
    updateWebpage();
    server.send(200, "text/html", webpage);
}

void handleCurrentData() {
    RtcDateTime now = Rtc.GetDateTime();
    float t = dht22.getTemperature();
    float h = dht22.getHumidity();
    char timeString[9];
    snprintf(timeString, sizeof(timeString), "%02u:%02u:%02u", now.Hour(), now.Minute(), now.Second());

    String json = "{";
    json += "\"temperature\":" + String(t) + ",";
    json += "\"humidity\":" + String(h) + ",";
    json += "\"time\":\"" + String(timeString) + "\"";
    json += "}";

    server.send(200, "application/json", json);
}

void readDataFromEeprom() {
    int address = eepromStartAddress;

    EEPROM.get(address, currentDayIndex);
    address += sizeof(int);

    for (int i = 0; i < NUM_DAYS; i++) {
        for (int j = 0; j < NUM_PERIODS; j++) {
            EEPROM.get(address, tempAverages[i][j]);
            address += sizeof(float);
            EEPROM.get(address, humidAverages[i][j]);
            address += sizeof(float);
        }
    }
}

void storeDataToEeprom() {
    int address = eepromStartAddress;

    EEPROM.put(address, currentDayIndex);
    address += sizeof(int);

    for (int i = 0; i < NUM_DAYS; i++) {
        for (int j = 0; j < NUM_PERIODS; j++) {
            EEPROM.put(address, tempAverages[i][j]);
            address += sizeof(float);
            EEPROM.put(address, humidAverages[i][j]);
            address += sizeof(float);
        }
    }

    EEPROM.commit();
    readDataFromEeprom();
}

void addData(float t, float h, int hour) {
    int period;
    if (hour >= 0 && hour < 8) {
        period = 0;
    } else if (hour >= 8 && hour < 16) {
        period = 1;
    } else {
        period = 2;
    }

    totalDailyTemp[period] += t;
    totalDailyHumid[period] += h;
    sampleCounts[period]++;
}

void calculateAverages(int period) {
    if (sampleCounts[period] > 0) {
        tempAverages[currentDayIndex][period] = totalDailyTemp[period] / sampleCounts[period];
        humidAverages[currentDayIndex][period] = totalDailyHumid[period] / sampleCounts[period];
    } else {
        tempAverages[currentDayIndex][period] = 0;
        humidAverages[currentDayIndex][period] = 0;
    }
}

void resetDailyTotals() {
    for (int i = 0; i < NUM_PERIODS; i++) {
        totalDailyTemp[i] = 0;
        totalDailyHumid[i] = 0;
        sampleCounts[i] = 0;
    }
}

void setup() {
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);
    Wire.begin();
    Rtc.Begin();
    lcd.init();
    lcd.backlight();

    if (!Rtc.IsDateTimeValid()) {
        RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
        Rtc.SetDateTime(compiled);
    }

    if (!Rtc.GetIsRunning()) {
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    currentDayIndex = now.DayOfWeek();

    readDataFromEeprom();
    resetDailyTotals();

    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();

    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.on("/", handleRoot);
    server.on("/currentData", handleCurrentData);
    server.begin();

    Serial.println("HTTP server started");
    Serial.println();
}

void loop() {
    server.handleClient();

    RtcDateTime now = Rtc.GetDateTime();
    hour = now.Hour();
    minute = now.Minute();
    second = now.Second();

    // if (Serial.available()) { // manual input for skipping hours
    //     hour = Serial.parseInt();
    //     Serial.println(hour);
    //     while (Serial.available() > 0) {
    //         Serial.read();
    //     }
    // }

    if (currentHour != hour) {
        currentHour = hour;

        float t = dht22.getTemperature();
        float h = dht22.getHumidity();

        addData(t, h, hour);

        if (hour == 8 || hour == 16 || hour == 0) {
            int period = (hour == 0) ? 2 : (hour / 8) - 1;
            calculateAverages(period);

            if (hour == 0) {
                storeDataToEeprom();
                currentDayIndex = (currentDayIndex + 1) % NUM_DAYS;
                resetDailyTotals();
            }
        }
    }

    char timeString[17];
    snprintf(timeString, sizeof(timeString), "Time: %02u:%02u:%02u", hour, minute, second);
    lcd.setCursor(0, 0);
    lcd.print(timeString);

    float t = dht22.getTemperature();
    float h = dht22.getHumidity();
    char dataString[17];
    snprintf(dataString, sizeof(dataString), "T:%.1fC H:%.1f%%", t, h);
    lcd.setCursor(0, 1);
    lcd.print(dataString);

    delay(1000);
}