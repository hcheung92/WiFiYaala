yaala = {}
yaala.button = function()
     if gpio.read(3) == 0 then
          tmr.alarm(0, 2000, 0, function ()
               off = 1
               programs.stop()
          end)
     else
          tmr.stop(0)
          if off == 0 then programs.change()
          else off = 0
          end
     end
end
yaala.init = function()
     dofile("server.lua")
     led.init(4)
     led.setType(led.PCA9685,0,3)
     dofile("lib.lua")
     dofile("programs.lua")
--     dofile("fade.lua")
     dofile("candle.lua")
--     dofile("demo.lua")
     programs.change()
     off = 0
     gpio.mode(3, gpio.INT)
     gpio.trig(3, "both", yaala.button)
end
if gpio.read(3) == 1 then  
     dofile("wifi.lua")
     dofile("wifi_credentials.lua")
     mywifi.init(yaala.init, 10000)
else
     print("button prevents lua-startup!")
end
