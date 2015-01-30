WiFi Yaala
==========
Yaala = Yet Another Ambient Light Approach using an ESP8622 chip

Background
----------
Despite some early prototypes this is the third major attempt to create a fully featured and yet simple to control RGB LED system.  

Project Status
--------------
As a base we are using the nodeMCU core to provide a fully functional eLUA console. We are adding stuff specific for our application like a LED core.
Additionally, we add some optional nice to have features like the Domain Name support for the DHCP client.

It is currently in a VERY early state. Documentation does not yet exist. The only thing which is working right now is the driver support for APA102 and PCA9685 LEDs. Besides others, WS28XX LEDs (glitch free, using hardware support) are getting implemented right now.
Stay tuned...

Requirements
-------------
GCC toolchain, see https://github.com/esp8266/esp8266-wiki/wiki

Building
--------
Goto firmware-lua and type "make && make flash"

Credits
----------
This work is based  on:

- Espressif SDK (http://bbs.espressif.com/viewforum.php?f=5)
- NodeMCU (https://github.com/nodemcu/nodemcu-firmware)
- eLUA (http://www.eluaproject.net/)
- and various other unnamed contributors

Due to frequent major changes in the code---especially in the SDK---everything is copied into this repo.
