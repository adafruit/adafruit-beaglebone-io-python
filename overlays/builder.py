from subprocess import call
import os
import shutil
import glob

def compile():
    call(["dtc", "-O", "dtb", "-o", "overlays/ADAFRUIT-SPI0-00-00A0.dtbo", "-b", "o", "-@", "overlays/ADAFRUIT-SPI0-00-00A0.dts"])
    call(["dtc", "-O", "dtb", "-o", "overlays/ADAFRUIT-SPI0-01-00A0.dtbo", "-b", "o", "-@", "overlays/ADAFRUIT-SPI0-01-00A0.dts"])
    call(["dtc", "-O", "dtb", "-o", "overlays/ADAFRUIT-SPI1-00-00A0.dtbo", "-b", "o", "-@", "overlays/ADAFRUIT-SPI1-00-00A0.dts"])
    call(["dtc", "-O", "dtb", "-o", "overlays/ADAFRUIT-SPI1-01-00A0.dtbo", "-b", "o", "-@", "overlays/ADAFRUIT-SPI1-01-00A0.dts"])

def copy():
    for fl in glob.glob("/lib/firmware/ADAFRUIT-SPI*-00A0.dtbo"):
        os.remove(fl)
    shutil.move("overlays/ADAFRUIT-SPI0-00-00A0.dtbo", "/lib/firmware/ADAFRUIT-SPI0-00-00A0.dtbo")
    shutil.move("overlays/ADAFRUIT-SPI0-01-00A0.dtbo", "/lib/firmware/ADAFRUIT-SPI0-01-00A0.dtbo")
    shutil.move("overlays/ADAFRUIT-SPI1-00-00A0.dtbo", "/lib/firmware/ADAFRUIT-SPI1-00-00A0.dtbo")
    shutil.move("overlays/ADAFRUIT-SPI1-01-00A0.dtbo", "/lib/firmware/ADAFRUIT-SPI1-01-00A0.dtbo")