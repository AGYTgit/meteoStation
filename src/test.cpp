// #include <Arduino.h>
// #include <Wire.h>

// #include <LiquidCrystal_I2C.h>
// LiquidCrystal_I2C lcd(0x27, 16, 2);

// #include <RtcDS1307.h>
// RtcDS1307<TwoWire> Rtc(Wire);

// #include <DHT22.h>
// #define pinDATA 13
// DHT22 dht22(pinDATA); 


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
// }

// void loop() {
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