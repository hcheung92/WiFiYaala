local program={}
-- external config
program.color = 240   -- -1 for random colors, otherwise a hue value
program.lum = 100

function program.run() 
     local r,g,b = led.get(0, -1)

     if ((r+b+g) < program.lum) then
          local index = (tmr.now()) % led.inited()
          
          if program.color == -1 then
               local hue = ((tmr.now())%360)
               r,g,b = hsl2rgb(hue, 255, 255)
          else    
               r,g,b = hsl2rgb(program.color, 255, 255)
          end
          -- Set one led on
          led.set(index, index, r, g, b, 0)
          -- and start dimming 
          local time = ((tmr.now())%10000) + 4000
          led.set(index, index, 0, 0, 0, time)
     end

     tmr.alarm(1, 500, 0, program.run)
end

function program.cancel()
  tmr.stop(1)
end

-- Need to turn off all leds, otherwise run will not start setting stars
led.set(0, -1, 0, 0, 0, 2000)
return program
