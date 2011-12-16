import time
import pycontroller

c = pycontroller.Controller()
for i in range(0,254)+list(reversed(range(0,254))):
    c.set_meter(i)
    c.lcd_write(str(i))
    time.sleep(.01)
    