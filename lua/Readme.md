lua-scripts
==========
The WifiYaala running on a ESP8622 chip boots the firmware-lua, which provides a lua interpreter. The entry-point is the script called "init.lua", which is loaded at the end of the bootup phase.

init.lua example
----------
<pre>
led.init(4)                         -- init four available led channels
led.setType(led.PCA9685,0,3)        -- set channels 0 - 3 as one PCA9685

tmr.alarm(0, 3000, 0, function()    -- delay initialization of wifi in 3 seconds
  wifi.setmode(wifi.STATION)        -- set wifi mode to "connect to wifi access point"
  wifi.sta.config("SSID","passwd")  -- set ssid and password and initiate connection
  print(wifi.sta.getip())           -- print the ip
end)

dofile("lib.lua")                   -- load some library functions
--dofile("fade.lua")                -- 
dofile("candle.lua")                -- start the candle
</pre>
