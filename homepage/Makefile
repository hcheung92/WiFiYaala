#Esptool.py path and port
ESPTOOL		?= /opt/Espressif/esp_prog/esptool/esptool.py
ESPPORT		?= /dev/ttyUSB0

webpages.espfs: html/ mkespfsimage/mkespfsimage
	rm -f html/.DS_Store 
	cd html; find | ../mkespfsimage/mkespfsimage  > ../webpages.espfs; cd ..

mkespfsimage/mkespfsimage: mkespfsimage/
	make -C mkespfsimage

htmlflash: webpages.espfs
	if [ $$(stat -c '%s' webpages.espfs) -gt $$(( 0x2E000 )) ]; then echo "webpages.espfs too big!"; false; fi
	-$(ESPTOOL) --port $(ESPPORT) write_flash 0x12000 webpages.espfs
