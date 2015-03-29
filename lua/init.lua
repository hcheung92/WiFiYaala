y_off=0
programs=0
function y_button (level)
  if level==0 then
    tmr.alarm(0, 1000, 0, function ()
      if gpio.read(3)==0 and programs.active ~= nil and programs.active.brightness ~= nil then -- brightness
        y_off=2
        programs.stop()
        tmr.alarm(0, 100, 1, function ()
          val = (programs.active.brightness + 5) % 255
          programs.active.brightness = val
          led.set(0,-1,val,val,val,100)
        end)
      else -- off
        y_off=1
        print("off!")
        programs.stop()
        led.set(0,-1,100,0,0,0)
        led.set(0,-1,0,0,0,1000)
      end
    end)
  else
    tmr.stop(0)
    if y_off==0 then programs.change()
    elseif y_off==2 then programs.active.run()
    end
    y_off=0
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
