local program={}
program.settings={}

program.Color = 240   -- -1 for random colors, otherwise a hue value
program.settings.Color = "-1-360°"
program.Brightness = 50
program.settings.Brightness = "1-100%"
program.Delay = 5
program.settings.Delay = "5-60s"

function program.run() 
     local r,g,b = led.get(0, -1)

     if ((r+b+g) < program.Brightness) then
          local i = math.random(0, led.inited()-1)
          r,g,b = led.get(i-1, i+1) 
          if (r+b+g) < (program.Brightness /2) then
               local lightnes = math.random(127, 255)
               if program.Color == -1 then
                    local hue = math.random(0, 360)
                    r,g,b = led.hsl2rgb(hue, 255, lightnes)
               else    
                    r,g,b = led.hsl2rgb(program.Color, 255, lightnes)
               end
               -- Set one LED on
               led.set(i, i, r, g, b, 0)
               -- and start dimming 
               local time = math.random(program.Delay, program.Delay*3) *1000
               led.set(i, i, 0, 0, 0, time)
          end
     end

     tmr.alarm(1, 200, 0, program.run)
end

function program.cancel()
  tmr.stop(1)
end

-- Need to turn off all leds, otherwise run will not start setting stars
led.set(0, -1, 0, 0, 0, 2000)
return program