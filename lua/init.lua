y_off=0
programs=0
function y_button ()
  if gpio.read(3)==0 then
    tmr.alarm(0, 2000, 0, function ()
      y_off=1
      print("off!")
      programs.stop()
      led.set(0,-1,100,0,0,0)
      led.set(0,-1,0,0,0,1000)
    end)
  else
    tmr.stop(0)
    if y_off==0 then programs.change()
    else y_off=0
    end
  end
end
function y_init ()
  hostname.set("WifiYaala")
  require("config")
  require("lib")
  programs=require("programs")
  gpio.mode(3, gpio.INT)
  gpio.trig(3, "both", y_button)
  led.set(0,-1,0,15,0,100)
end
if gpio.read(3) == 1 then
  y_init()
else
  print("button prevents lua-startup!")
end
