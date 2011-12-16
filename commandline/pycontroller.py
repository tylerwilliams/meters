import os
import random
import ctypes

class Controller(object):
    def __init__(self):
        self.clib = ctypes.cdll.LoadLibrary(os.getcwd() + '/controller.so')
        self.clib.open_handle()
    
    def toggle_led(self, state):
        if not state in [0,1]:
            raise Exception("State must be 0 or 1")
        self.clib.toggle_led(state)
    
    def get_meter(self):
        return self.clib.get_meter_value()

    def set_meter(self, val):
        assert 0 <= val <= 255
        self.clib.set_meter_value(val)
    
    def lcd_command(self, command):
        self.clib.lcd_command(command)
    
    def lcd_goto(self, x, y):
        self.clib.lcd_goto(x, y)
    
    def lcd_clear(self):
        self.clib.clear_lcd()
    
    def lcd_write(self, txt):
        self.clib.lcd_puts(ctypes.c_char_p(txt))
    
    def __del__(self):
        self.clib.close_handle()
