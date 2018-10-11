import Adafruit_BBIO.PWM as PWM 

pins = [
"P9_16", #PWM1B
"P9_21", #PWM0B
"P9_22", #PWM0A
"P9_28", #ECAP2
"P9_29", #PWM0B
"P9_31", #PWM0A
"P9_42", #ECAP0
"P8_13", #PWM2B
"P8_19", #PWM2A
"P8_34", #PWM1B
"P8_36", #PWM1A
"P8_45", #PWM2A
"P8_46"  #PWM2B
]

# /sys/devices/platform/ocp/48300000.epwmss/48300100.ecap/pwm/pwmchip0/pwm-0:0/duty_cycle
# /sys/devices/platform/ocp/48304000.epwmss/48304100.ecap/pwm/pwmchip5/pwm-5:0/duty_cycle

for pin in pins:
  print(pin)
  PWM.start(pin, 50, 2000, 1)
  PWM.stop(pin)
  PWM.cleanup()
 
