## Raspberry Pi Pico RP2040 - Arduino IDE

**A few words in the beginning**
* **Version: 1.0.0** The project is a work in progress and is **very beta version** - there may be bugs...
* This project not an official platform and is based on [**pico-sdk**](https://github.com/raspberrypi/pico-sdk)
* **Systems support**
* * windows
* * linux & darwin - **later ... I need 300Mb free space for compilers***

**Notes**
* _I am in Home-Office, it's hard for me to test hardwares as SPI, I2C ... etc_

![pico](https://raw.githubusercontent.com/Wiz-IO/LIB/master/pico/a1.jpg)

## Install Platform
_Note: be sure [**git**](https://git-scm.com/downloads) is installed_
* Arduino -> File -> Preferences -> Additional Boards Manager URL
* paste https://raw.githubusercontent.com/Wiz-IO/wizio-arduino-pico/main/package_wizio.pico_index.json
* Arduino -> Tools -> Board -> Boards Manager -> Find: wizio -> WizIO Pico by .. Install
* go to: C:\Users\ **USER** \AppData\Local\Arduino15\packages\WizIO\hardware\pico\1.0.0
* Delete files in folder 1.0.0 and execute:
* [ **git clone https://github.com/Wiz-IO/wizio-arduino-pico .** ] _with dot_


