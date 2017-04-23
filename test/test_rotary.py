#!/usr/bin/python

import Adafruit_BBIO.Encoder as Encoder

qep = Encoder.RotaryEncoder(1)
#qep = Encoder.QEP()
print qep

print("qep.enable(): {0}".format(qep.enable()))
#qep.getMode()
#qep.setAbsolute()
#qep.setRelative()
#qep.setFrequency(5000)
print("qep.getPosition(): {0}".format(qep.getPosition()))
#qep.setPosition(100)
#qep.disable()
