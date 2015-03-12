-- based on: https://www.youtube.com/watch?v=8rX66dcKYrw
local program={}
-- external config
program.delay = 30000

-- internal use
program.step = 0

function program.run() 
     local r,g,b
     local i
     
     if program.step == 0 then
          -- dim to dark blue
          led.set(0, -1, 0, 0, 25, program.delay)
          tmr.alarm(1, program.delay, 0, program.run)

     elseif program.step == 1 then
          local sat, pos
          local quater = led.inited() /4
          for i = 0, quater, 1 do
               sat = 255/quater *i 
               -- blue on the side
               r,g,b = hsl2rgb(240, (255-sat) , 255)
               led.set(i, i, r, g, b, program.delay)
               led.set(-i, -i, r, g, b, program.delay)
               -- orange in the mid
               r,g,b = hsl2rgb(30, sat , 255)
               pos = i + quater
               led.set(pos, pos, r, g, b, program.delay)
               led.set(-pos, -pos, r, g, b, program.delay)
          end
          tmr.alarm(1, program.delay, 0, program.run)
               
     elseif program.step == 2 then
          -- over saturate now
          for i = 0, (led.inited() -1), 1 do
               r, g, b = led.get(i, i)
               r = r +100
               if r > 255 then r = 255 end
               g = g +100
               if g > 255 then g = 255 end
               b = b +100
               if b > 255 then b = 255 end
               led.set(i, i, r, g, b, program.delay)
          end
          tmr.alarm(1, program.delay, 0, program.run)
     end
            
     program.step = program.step +1
end

function program.cancel()
  tmr.stop(1)
end

return program