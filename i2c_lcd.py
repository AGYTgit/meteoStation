import time
from lcd_api import LcdApi
from machine import I2C

class I2cLcd(LcdApi):
    def __init__(self, i2c, i2c_addr, num_lines, num_columns):
        self.i2c = i2c
        self.i2c_addr = i2c_addr
        self.num_lines = num_lines
        self.num_columns = num_columns
        self.backlight = 0x08
        super().__init__(num_lines, num_columns)
        self.init_lcd()

    def init_lcd(self):
        self.write_cmd(0x33)
        self.write_cmd(0x32)
        self.write_cmd(0x28)
        self.write_cmd(0x0C)
        self.write_cmd(0x06)
        self.clear()

    def write_cmd(self, cmd):
        self.i2c_send_byte(cmd, False)

    def write_char(self, char):
        self.i2c_send_byte(char, True)

    def i2c_send_byte(self, data, is_char):
        byte = (data & 0xF0) | self.backlight | (0x01 if is_char else 0x00)
        self.i2c.writeto(self.i2c_addr, bytearray([byte | 0x04]))
        time.sleep_us(500)
        self.i2c.writeto(self.i2c_addr, bytearray([byte & ~0x04]))

        byte = ((data << 4) & 0xF0) | self.backlight | (0x01 if is_char else 0x00)
        self.i2c.writeto(self.i2c_addr, bytearray([byte | 0x04]))
        time.sleep_us(500)
        self.i2c.writeto(self.i2c_addr, bytearray([byte & ~0x04]))

    def backlight_on(self):
        self.backlight = 0x08
        self.write_cmd(0x00)

    def backlight_off(self):
        self.backlight = 0x00
        self.write_cmd(0x00)
