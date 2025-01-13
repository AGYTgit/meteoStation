import dht
import machine
import time
from machine import I2C, Pin
from wifi import WIFI
from lcd_api import LcdApi
from i2c_lcd import I2cLcd

i2c = I2C(0, scl=Pin(19), sda=Pin(21), freq=400000)
I2C_ADDR = 0x27
LCD_ROWS = 2
LCD_COLS = 20
lcd = I2cLcd(i2c, I2C_ADDR, LCD_ROWS, LCD_COLS)
lcd.backlight_on()

dht_pin = machine.Pin(15)
sensor = dht.DHT11(dht_pin)

wifi = WIFI()
wifi.init()

temp = 0
humid = 0
temp_old = 0
humid_old = 0

def start():
    global temp, humid, temp_old, humid_old
    
    while True:
        try:
            sensor.measure()
            temp_old = temp
            humid_old = humid
            temp = sensor.temperature()
            humid = sensor.humidity()
            
            if temp == temp_old and humid == humid_old:
                time.sleep(1)
                continue

            lcd.clear()
            lcd.move_to(0, 0)
            lcd.putstr(f'Temp: {temp}C')

            lcd.move_to(0, 1)
            lcd.putstr(f'Humidity: {humid}%')

            wifi.send_data(temp, humid)

            print(f"Sent values -> Temp: {temp}, Humid: {humid}")

            time.sleep(1)

        except OSError as e:
            print(f"Error reading DHT sensor: {e}")
            lcd.clear()
            lcd.putstr('Sensor error')
            time.sleep(2)

start()


