local program={}
program.settings={}

program.Delay = 200
program.settings.Delay = "50-10000ms"
--program.color = -1                  -- -1 for random color
program.Color = 360/led.inited()     -- one complete rainbow over whole stripe
program.settings.Color = "-1-50°"
program.Brightness = 255
program.settings.Brightness = "1-255%"

-- internal use
program.hue = 0

function program.run() 
     local r,g,b
     local i
     for i = (led.inited() -1), 1, -1 do
          r,g,b = led.get((i-1), (i-1))
          led.set(i, i, r, g, b, program.Delay)
     end

     if program.Color == -1 then
          program.hue = math.random(0, 360)
     else    
          program.hue = (program.hue + program.Color) %360
     end

     r,g,b = hsl2rgb(program.hue, 255, program.Brightness)
     led.set(0, 0, r, g, b, program.Delay)
 
     tmr.alarm(1, program.Delay, 0, program.run)
end

function program.cancel()
  tmr.stop(1)
end

return program
