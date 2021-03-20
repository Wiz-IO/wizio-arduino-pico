# NOT READY, DO NOT INSTALL, CLONE, DOWNLOAD... 

## Raspberry Pi Pico RP2040 - Arduino IDE

**A few words in the beginning**
* **Version: 1.0.0** The project is a work in progress and is **very beta version** - there may be bugs...
* This project not an official platform and is based on [**pico-sdk**](https://github.com/raspberrypi/pico-sdk)
* **Systems support**
* * **windows**
* * linux & darwin **LATER** ... _I need 300 Mb free space for compilers_

**Notes**
* _I am in Home-Office, it's hard for me to test hardwares as SPI, I2C ... etc_

![pico](https://raw.githubusercontent.com/Wiz-IO/LIB/master/pico/a1.jpg)

# Install
_Note: be sure [**git**](https://git-scm.com/downloads) is installed_
* Arduino -> File -> Preferences -> Additional Boards Manager URL
* paste https://raw.githubusercontent.com/Wiz-IO/wizio-arduino-pico/main/package_wizio.pico_index.json
* Arduino -> Tools -> Board -> Boards Manager -> Find: WizIO Pico by ... **Install**
* Now, go to folder C:\Users\ **USER** \AppData\Local\Arduino15\packages\WizIO\hardware\pico\1.0.0
### and clone this git
* Delete files from \1.0.0 folder and execute
* **[ git clone https://github.com/Wiz-IO/wizio-arduino-pico . ]** _with dot_
* Arduino -> Tools -> Board -> Select board ... **and make fun**

### Update ( if need ... the project is a work in progress )
* Go to: C:\Users\ **USER** \AppData\Local\Arduino15\packages\WizIO\hardware\pico\1.0.0
* execute **[ git pull ]**
* or goto [Clone](https://github.com/Wiz-IO/wizio-arduino-pico/blob/master/README.md#clone) ( the best variant )

***
### UPLOAD Sketch

Connect Pico as Mass Storage Device - click Compile & Upload ...

_UF2 exist in build folder_

### STDIO ( only one will work )
* Arduino -> Tools -> SDTDIO -> Select
* * PICO_STDIO_ARDUINO - in code Serial.begin(115200, **true**); // true redirect printf to Serial
* * PICO_STDIO_UART - if Serial is not used
* * PICO_STDIO_USB - tinyusb

### Examples & Others - TODO

***

If you want to help / support:  

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=ESUP9LCZMZTD6)

