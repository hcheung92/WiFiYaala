WiFi Yaala
==========
Yaala = Yet Another Ambient Light Approach    using a ESP8622 chip

Background
----------
Despite some early prototypes this is the third major attempt to create a fully featured and yet simple to control RGB LED system.  

Project Status
--------------
It is currently in a VERY early state. The only thing which is working right now is the driver support for APA102 and PCA9685 LEDs.
Stay tuned...

Requirements
-------------
GCC toolchain, see https://github.com/esp8266/esp8266-wiki/wiki

Building
--------
Goto firmware/src and type make

Credits
----------
This work is based  on:
1) Espressif SDK (http://bbs.espressif.com/viewforum.php?f=5)
2) Sprite_tm's Webserver (http://www.esp8266.com/viewtopic.php?f=6&t=376)
3) Heatshrink by atomicobject (https://github.com/atomicobject/heatshrink)
Due to frequent major changes in the code---especially in the SDK---everything is copied into this repo.