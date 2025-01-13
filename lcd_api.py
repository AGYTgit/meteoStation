import time

class LcdApi:
    def __init__(self, num_lines, num_columns):
        self.num_lines = num_lines
        self.num_columns = num_columns
        self.cursor_x = 0
        self.cursor_y = 0
        self.display_control = 0x04
        self.display_mode = 0x02
        self.backlight = 0x08

    def clear(self):
        self.write_cmd(0x01)
        time.sleep_ms(2)

    def home(self):
        self.write_cmd(0x02)
        time.sleep_ms(2)

    def move_to(self, col, row):
        if row >= self.num_lines:
            row = self.num_lines - 1
        if col >= self.num_columns:
            col = self.num_columns - 1
        self.cursor_x = col
        self.cursor_y = row
        addr = col + (0x40 if row == 1 else 0x00)
        self.write_cmd(0x80 | addr)

    def putstr(self, string):
        for char in string:
            self.write_char(ord(char))

    def write_cmd(self, cmd):
        raise NotImplementedError

    def write_char(self, char):
        raise NotImplementedError

