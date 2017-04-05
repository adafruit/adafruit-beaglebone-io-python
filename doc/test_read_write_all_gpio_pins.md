This Python program will open, read and write to all the GPIO pins and the USR LEDs:
```
import Adafruit_BBIO.GPIO as GPIO
import time

for i in range(4):
    GPIO.setup("USR%d" % i, GPIO.OUT)
    GPIO.output("USR%d" % i, GPIO.HIGH)
    GPIO.output("USR%d" % i, GPIO.LOW)

for i in [3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46]:
    GPIO.setup("P8_%d" % i, GPIO.OUT)
    GPIO.output("P8_%d" % i, GPIO.HIGH)
    GPIO.output("P8_%d" % i, GPIO.LOW)

for i in [11,12,13,14,15,16,17,18,21,22,23,24,25,26,27,28,29,30,31,41,42]:
    GPIO.setup("P9_%d" % i, GPIO.OUT)
    GPIO.output("P9_%d" % i, GPIO.HIGH)
    GPIO.output("P9_%d" % i, GPIO.LOW)
```

This can be useful when run with strace to verify that the library is operating as expected:
```
sudo strace -e file python ~/gpio_output_test.py 2>&1 |grep '/sys/'|less
```

refer to this gist for more info:
https://gist.github.com/pdp7/e7753faff9a9bd27389a0173c71f9312
