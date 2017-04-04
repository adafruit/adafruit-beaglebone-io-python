#!/bin/bash

# Result: chown debian:debian /sys/class/gpio

chown -R debian:debian /sys/devices/gpio
chown -R debian:debian /sys/class/gpio
chown -R debian:debian /sys/devices/platform/ocp
chown -R debian:debian /sys/class/pwm

#GPIO ( active_low device direction edge power subsystem uevent value )
find /sys/class/gpio -type d -exec chmod 2770 {} \;
find /sys/class/gpio -name "direction" -exec chmod 0660 {} \;
find /sys/class/gpio -name "edge" -exec chmod 0660 {} \;
find /sys/class/gpio -name "value" -exec chmod 0660 {} \;
find /sys/class/gpio -name "active_low" -exec chmod 0660 {} \;
find /sys/class/gpio -name "device" -exec chmod 0660 {} \;
find /sys/class/gpio -name "power" -exec chmod 0660 {} \;
find /sys/class/gpio -name "subsystem" -exec chmod 0660 {} \;

#PWM ( duty_cycle enable period polarity power uevent )
find /sys/devices/platform/ocp -name "duty_cycle" -exec chmod 0660 {} \;
find /sys/devices/platform/ocp -name "enable" -exec chmod 0660 {} \;
find /sys/devices/platform/ocp -name "period" -exec chmod 0660 {} \;
find /sys/devices/platform/ocp -name "polarity" -exec chmod 0660 {} \;
find /sys/devices/platform/ocp -name "power" -exec chmod 0660 {} \;
find /sys/class/pwm -name "duty_cycle" -exec chmod 0660 {} \;
find /sys/class/pwm -name "enable" -exec chmod 0660 {} \;
find /sys/class/pwm -name "period" -exec chmod 0660 {} \;
find /sys/class/pwm -name "polarity" -exec chmod 0660 {} \;
find /sys/class/pwm -name "power" -exec chmod 0660 {} \;

chmod 0220 /sys/class/gpio/export
chmod 0220 /sys/class/gpio/unexport
chmod 0220 /sys/class/pwm/export
chmod 0220 /sys/class/pwm/unexport

find /sys/class/gpio -name "uevent" -exec chmod 0660 {} \;
find /sys/class/gpio -name "autosuspend_delay_ms" -exec chmod 0660 {} \;
find /sys/class/gpio -name "control" -exec chmod 0660 {} \;
find /sys/class/pwm -name "uevent" -exec chmod 0660 {} \;
find /sys/devices/platform/ocp -name "uevent" -exec chmod 0660 {} \;

# Additional code for getting AIN pins set up
ain_activator=/sys/devices/platform/bone_capemgr/slots
chown -R debian:debian $ain_activator/
chmod -R 2770 $ain_activator/
