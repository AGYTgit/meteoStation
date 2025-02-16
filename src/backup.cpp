// #include <Arduino.h>
// #include <Wire.h>

// #include <LiquidCrystal_I2C.h>
// LiquidCrystal_I2C lcd(0x27, 16, 2);

// #include <RtcDS1307.h>
// RtcDS1307<TwoWire> Rtc(Wire);

// #include <DHT22.h>
// #define pinDATA 13
// DHT22 dht22(pinDATA);

// #include <WiFi.h>
// #include <WebServer.h>
// const char* ssid = "EspAP";
// const char* password = "12345678";
// WebServer server(80);
// String webpage = "";


// void updateWebpage() {
//     webpage = R"=====(
//         <!DOCTYPE html>
//             <html lang="en">
    
//             <head>
//                 <meta charset="UTF-8">
//                 <meta name="viewport" content="width=device-width, initial-scale=1.0">
//                 <title>ESP32 Sensor Data</title>
//                 <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@400;700&display=swap" rel="stylesheet">
//                 <style>
//                     body {
//                         font-family: 'Poppins', sans-serif;
//                         background: linear-gradient(135deg, #1e3c72, #2a5298);
//                         color: #fff;
//                         margin: 0;
//                         padding: 0;
//                         display: flex;
//                         flex-direction: column;
//                         align-items: center;
//                         justify-content: center;
//                         height: 100vh;
//                     }
    
//                     .tab-buttons {
//                         display: flex;
//                         justify-content: center;
//                         margin-bottom: 10px;
//                         background-color: transparent;
//                         padding: 10px;
//                     }
    
//                     .tab-button {
//                         background-color: rgba(51, 102, 153, 0.8);
//                         color: white;
//                         border: none;
//                         padding: 10px 20px;
//                         margin: 0 5px;
//                         border-radius: 5px;
//                         cursor: pointer;
//                         font-family: 'Poppins', sans-serif;
//                         transition: background-color 0.3s ease;
//                     }
    
//                     .tab-button:hover {
//                         background-color: rgba(51, 102, 153, 1);
//                     }
    
//                     .container {
//                         background-color: rgba(255, 255, 255, 0.2);
//                         padding: 20px;
//                         border-radius: 10px;
//                         box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
//                         text-align: center;
//                         margin-top: 20px;
//                     }
    
//                     h1 {
//                         color: #66b3ff;
//                         margin-bottom: 20px;
//                     }
    
//                     p {
//                         font-size: 1.2em;
//                         margin-bottom: 10px;
//                     }
    
//                     .data-value {
//                         font-weight: bold;
//                     }
    
//                     .tab-content {
//                         display: none;
//                     }
    
//                     .active {
//                         display: block;
//                     }
    
//                     #average table {
//                         width: 100%;
//                         border-collapse: collapse;
//                         margin-top: 20px;
//                     }
    
//                     #average th,
//                     #average td {
//                         border: 1px solid #ddd;
//                         padding: 8px;
//                         text-align: left;
//                     }
    
//                     #average th {
//                         background-color: rgba(51, 102, 153, 0.7); /* More opaque blue */
//                         color: white;
//                     }
    
//                     #average tr:nth-child(even) {
//                         background-color: rgba(255, 255, 255, 0.1);
//                     }
    
//                     .day-details {
//                         display: none;
//                     }
    
//                     .day-details.active {
//                         display: table-row-group;
//                     }
    
//                     .day-details tr {
//                         background-color: rgba(255, 255, 255, 0.15); /* Slightly darker background */
//                         color: #fff; /* White text */
//                     }
    
//                     .day-details.active tr:hover {
//                         background-color: rgba(255, 255, 255, 0.25);
//                     }
    
//                     /* OR, to make the main day rows less visible when details are shown: */
//                     #averageTableBody > tr { /* Style for the main day rows */
//                         transition: opacity 0.3s ease; /* Add transition for smooth effect */
//                     }
    
//                     .day-details.active ~ tr { /* Target main rows AFTER the active details */
//                         opacity: 0.7; /* Slightly reduce opacity */
//                     }
//                 </style>
//             </head>
    
//             <body>
    
//                 <div class="tab-buttons">
//                     <button class="tab-button active" onclick="openTab('current')">Current Data</button>
//                     <button class="tab-button" onclick="openTab('average')">Averages</button>
//                 </div>
    
//                 <div class="container">
//                     <h1>ESP32 Sensor Data</h1>
    
//                     <div id="current" class="tab-content active">
//                         <p>Temperature: <span class="data-value" id="currentTemp">10°C</span></p>
//                         <p>Humidity: <span class="data-value" id="currentHum">20%</span></p>
//                         <p>Time: <span class="data-value" id="currentHum">9:21 am</span></p>
//                     </div>
    
//                     <div id="average" class="tab-content">
//                         <table>
//                             <thead>
//                                 <tr>
//                                     <th>Day of the Week</th>
//                                     <th>Temperature (°C)</th>
//                                     <th>Humidity (%)</th>
//                                 </tr>
//                             </thead>
//                             <tbody id="averageTableBody">
//                                 <tr onclick="toggleDayDetails('monday')">
//                                     <td>Monday</td>
//                                     <td>22.5</td>
//                                     <td>55.2</td>
//                                 </tr>
//                                 <tbody class="day-details" id="monday">
//                                     <tr>
//                                         <td>Morning</td>
//                                         <td>21.8</td>
//                                         <td>54.5</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Day</td>
//                                         <td>23.2</td>
//                                         <td>55.9</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Evening</td>
//                                         <td>22.1</td>
//                                         <td>54.8</td>
//                                     </tr>
//                                 </tbody>
//                                 <tr onclick="toggleDayDetails('tuesday')">
//                                     <td>Tuesday</td>
//                                     <td>23.1</td>
//                                     <td>56.8</td>
//                                 </tr>
//                                 <tbody class="day-details" id="tuesday">
//                                     <tr>
//                                         <td>Morning</td>
//                                         <td>22.4</td>
//                                         <td>56.1</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Day</td>
//                                         <td>23.9</td>
//                                         <td>57.5</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Evening</td>
//                                         <td>22.8</td>
//                                         <td>57.0</td>
//                                     </tr>
//                                 </tbody>
//                                 <tr onclick="toggleDayDetails('wednesday')">
//                                     <td>Wednesday</td>
//                                     <td>24.2</td>
//                                     <td>58.1</td>
//                                 </tr>
//                                 <tbody class="day-details" id="wednesday">
//                                     <tr>
//                                         <td>Morning</td>
//                                         <td>23.5</td>
//                                         <td>57.4</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Day</td>
//                                         <td>25.0</td>
//                                         <td>58.8</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Evening</td>
//                                         <td>24.0</td>
//                                         <td>58.0</td>
//                                     </tr>
//                                 </tbody>
//                                 <tr onclick="toggleDayDetails('thursday')">
//                                     <td>Thursday</td>
//                                     <td>23.8</td>
//                                     <td>57.5</td>
//                                 </tr>
//                                 <tbody class="day-details" id="thursday">
//                                     <tr>
//                                         <td>Morning</td>
//                                         <td>23.1</td>
//                                         <td>56.8</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Day</td>
//                                         <td>24.6</td>
//                                         <td>58.2</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Evening</td>
//                                         <td>23.5</td>
//                                         <td>57.1</td>
//                                     </tr>
//                                 </tbody>
//                                 <tr onclick="toggleDayDetails('friday')">
//                                     <td>Friday</td>
//                                     <td>24.5</td>
//                                     <td>59.2</td>
//                                 </tr>
//                                 <tbody class="day-details" id="friday">
//                                     <tr>
//                                         <td>Morning</td>
//                                         <td>23.8</td>
//                                         <td>58.5</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Day</td>
//                                         <td>25.3</td>
//                                         <td>59.9</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Evening</td>
//                                         <td>24.2</td>
//                                         <td>59.0</td>
//                                     </tr>
//                                 </tbody>
//                                 <tr onclick="toggleDayDetails('saturday')">
//                                     <td>Saturday</td>
//                                     <td>25.1</td>
//                                     <td>60.8</td>
//                                 </tr>
//                                 <tbody class="day-details" id="saturday">
//                                     <tr>
//                                         <td>Morning</td>
//                                         <td>24.4</td>
//                                         <td>60.1</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Day</td>
//                                         <td>25.9</td>
//                                         <td>61.5</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Evening</td>
//                                         <td>24.8</td>
//                                         <td>60.7</td>
//                                     </tr>
//                                 </tbody>
//                                 <tr onclick="toggleDayDetails('sunday')">
//                                     <td>Sunday</td>
//                                     <td>24.9</td>
//                                     <td>60.5</td>
//                                 </tr>
//                                 <tbody class="day-details" id="sunday">
//                                     <tr>
//                                         <td>Morning</td>
//                                         <td>24.2</td>
//                                         <td>59.8</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Day</td>
//                                         <td>25.7</td>
//                                         <td>61.2</td>
//                                     </tr>
//                                     <tr>
//                                         <td>Evening</td>
//                                         <td>24.6</td>
//                                         <td>60.4</td>
//                                     </tr>
//                                 </tbody>
//                             </tbody>
//                         </table>
//                     </div>
//                 </div>
    
//                 <script>
//                     function openTab(tabName) {
//                         let i;
//                         let tabContent = document.getElementsByClassName("tab-content");
//                         let tabButtons = document.getElementsByClassName("tab-button");
    
//                         for (i = 0; i < tabContent.length; i++) {
//                             tabContent[i].style.display = "none";
//                         }
    
//                         for (i = 0; i < tabButtons.length; i++) {
//                             tabButtons[i].className = tabButtons[i].className.replace(" active", "");
//                         }
    
//                         document.getElementById(tabName).style.display = "block";
//                         event.currentTarget.className += " active";
//                     }
    
//                     function toggleDayDetails(dayId) {
//                         const dayDetails = document.getElementById(dayId);
//                         const allDayDetails = document.querySelectorAll('.day-details.active'); // Select all open day details
    
//                         allDayDetails.forEach(openDetails => { // Close any other open details
//                         if (openDetails.id !== dayId) {
//                             openDetails.classList.remove('active');
//                         }
//                     });
  
//                     dayDetails.classList.toggle('active'); // Toggle the clicked day's details
//                 }
//             </script>
//         </body>
//     )=====";
// }

// void handleRoot() {
//     updateWebpage();
//     server.send(200, "text/html", webpage);
// }

// void setup() {
//     Serial.begin(115200);
//     Wire.begin();
//     Rtc.Begin();
//     lcd.init();
//     lcd.backlight();

//     if (!Rtc.IsDateTimeValid()) {
//         RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
//         Rtc.SetDateTime(compiled);
//     }

//     if (!Rtc.GetIsRunning()) {
//         Rtc.SetIsRunning(true);
//     }

//     WiFi.softAP(ssid, password);
//     IPAddress IP = WiFi.softAPIP();

//     Serial.print("AP IP address: ");
//     Serial.println(IP);

//     server.on("/", handleRoot);
//     server.begin();

//     Serial.println("HTTP server started");
//     Serial.println();
// }

// void loop() {
//     server.handleClient();
    
//     RtcDateTime dt = Rtc.GetDateTime();

//     if (Rtc.IsDateTimeValid()) {
//         char dataString[17];
//         snprintf(dataString, sizeof(dataString), "Time: %02u:%02u:%02u", dt.Hour(), dt.Minute(), dt.Second());

//         lcd.setCursor(0, 0);
//         lcd.print(dataString);
//     }

//     float t = dht22.getTemperature();
//     float h = dht22.getHumidity();

//     if (true) {
//         char dataString[17];
//         snprintf(dataString, sizeof(dataString), "T:%.1fC H:%.1f%%%", t, h);

//         lcd.setCursor(0, 1);
//         lcd.print(dataString);
//     }

//     delay(1000);
// }