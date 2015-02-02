led.init(4)
led.setType(led.PCA9685,0,3)

--wifi.setmode(wifi.STATION)
--wifi.sta.config("Kerschbaam","18LODAclWLAN")

--tmr.alarm(0, 3000, 0, function() 
--print(wifi.sta.getip())
--end)

dofile("lib.lua")
--dofile("fade.lua")
dofile("candle.lua")