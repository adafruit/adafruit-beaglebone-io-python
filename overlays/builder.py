from subprocess import call
import os

def compile():
    call(["dtc", "-O", "dtb", "-o", "overlays/ADAFRUIT-SPI0-00-00A0.dtbo", "-b", "o", "-@", "overlays/ADAFRUIT-SPI0-00-00A0.dts"])
    call(["dtc", "-O", "dtb", "-o", "overlays/ADAFRUIT-SPI0-01-00A0.dtbo", "-b", "o", "-@", "overlays/ADAFRUIT-SPI0-01-00A0.dts"])
    call(["dtc", "-O", "dtb", "-o", "overlays/ADAFRUIT-SPI1-00-00A0.dtbo", "-b", "o", "-@", "overlays/ADAFRUIT-SPI1-00-00A0.dts"])
    call(["dtc", "-O", "dtb", "-o", "overlays/ADAFRUIT-SPI1-01-00A0.dtbo", "-b", "o", "-@", "overlays/ADAFRUIT-SPI1-01-00A0.dts"])
def copy():
    os.rename("overlays/ADAFRUIT-SPI0-00-00A0.dtbo", "/lib/firmware/ADAFRUIT-SPI0-00-00A0.dtbo")
    os.rename("overlays/ADAFRUIT-SPI0-01-00A0.dtbo", "/lib/firmware/ADAFRUIT-SPI0-01-00A0.dtbo")
    os.rename("overlays/ADAFRUIT-SPI1-00-00A0.dtbo", "/lib/firmware/ADAFRUIT-SPI1-00-00A0.dtbo")
    os.rename("overlays/ADAFRUIT-SPI1-01-00A0.dtbo", "/lib/firmware/ADAFRUIT-SPI1-01-00A0.dtbo")