#!/usr/bin/python

import Adafruit_BBIO.Encoder as Encoder
# P8.11: eqep2b_in
# P8.12: eqep2a_in
# run "config-pin p8.11 qep && config-pin p8.12 qep"
qep = Encoder.RotaryEncoder(2)
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
