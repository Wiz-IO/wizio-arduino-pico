# NOT READY, DO NOT INSTALL, CLONE, DOWNLOAD... 

## Raspberry Pi Pico RP2040 - Arduino IDE

**A few words in the beginning**
* **Version: 1.0.0** The project is a work in progress and is **very beta version** - there may be bugs...
* This project not an official platform and is based on [**pico-sdk**](https://github.com/raspberrypi/pico-sdk)
* **Systems support**
* * windows
* * linux & darwin - **LATER** ... I need 300 Mb free space for compilers

**Notes**
* _I am in Home-Office, it's hard for me to test hardwares as SPI, I2C ... etc_

![pico](https://raw.githubusercontent.com/Wiz-IO/LIB/master/pico/a1.jpg)

## Install
_Note: be sure [**git**](https://git-scm.com/downloads) is installed_
* Arduino -> File -> Preferences -> Additional Boards Manager URL
* paste https://raw.githubusercontent.com/Wiz-IO/wizio-arduino-pico/main/package_wizio.pico_index.json
* Arduino -> Tools -> Board -> Boards Manager -> Find: wizio -> WizIO Pico by .. Install
* go to: C:\Users\ **USER** \AppData\Local\Arduino15\packages\WizIO\hardware\pico\1.0.0
* Delete install files in folder \1.0.0 and execute:
* **[ git clone https://github.com/Wiz-IO/wizio-arduino-pico . ]** _with dot_
* Arduino -> Tools -> Board -> Select board ... and make fun

## Update ( if need )
* go to: C:\Users\ **USER** \AppData\Local\Arduino15\packages\WizIO\hardware\pico\1.0.0
* **[ git pull ]**
* or delete all in \1.0.0 and [ git clone https://github.com/Wiz-IO/wizio-arduino-pico . ]

### STDIO ( only one will work )
* Arduino -> Tools -> SDTDIO
* * PICO_STDIO_ARDUINO in code Serial.begin(115200, true); // true redirect stdio to Serial
* * PICO_STDIO_UART - if Serial is not used
* * PICO_STDIO_USB - tinyusb

### Examples TODO

***

>If you want to help / support:   
[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=ESUP9LCZMZTD6)

