local program={}
-- external config
program.delay = 100
--program.color = 1                  -- -1 for random color
program.color = 360/led.inited()     -- one complete rainbow over whole stripe

-- internal use
program.hue = 0

function program.run() 
     local r,g,b
     local i
     for i = (led.inited() -1), 1, -1 do
          r,g,b = led.get((i-1), (i-1))
          led.set(i, i, r, g, b, program.delay)
     end

     if program.color == -1 then
          program.hue = (tmr.now()) %360
     else    
          program.hue = (program.hue + program.color) %360
     end

     r,g,b = hsl2rgb(program.hue, 255, 255)
     led.set(0, 0, r, g, b, program.delay)
 
     tmr.alarm(1, program.delay, 0, program.run)
end

function program.cancel()
  tmr.stop(1)
end

return program