import Adafruit_BBIO.PWM as PWM
PWM.start("P9_14", 50, 2000, 1)
PWM.cleanup()
PWM.start("P9_14", 50, 2000, 0)
PWM.cleanup()
