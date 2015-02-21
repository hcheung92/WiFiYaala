local program={}
-- external config
program.mode = 0
program.color = 0   -- 0 for random colors, otherwise a hue value
program.delay = 50

-- internal use
program.dir = 0
program.index = 0 
program.hue = 0

function program.run() 
     if program.index == 0 then 
          if program.color == 0 then 
               program.hue = ((tmr.now())%360)
          else
               program.hue = program.color
          end
     end
     
     local r,g,b = hsl2rgb(program.hue, 255, 255)
     led.set(program.index, program.index, r, g, b, program.delay)

     if program.dir == 0 then
          if program.index > 0 then 
               led.set(program.index-1, program.index-1, 0, 0, 0, program.delay*5) 
          end 
    
          program.index = program.index +1         
          if program.index == led.inited() then 
               if program.mode == 1 then -- again from start
                    program.index = 0;
               else -- reverse
                    program.dir = 1 
                    program.index = program.index -1
               end
           end
     else
          if program.index < (led.inited() -1) then 
               led.set(program.index+1, program.index+1, 0, 0, 0, program.delay*5) 
          end
          
          program.index = program.index -1
          if program.index == 0 then program.dir = 0 end
     end      

     tmr.alarm(1, program.delay, 0, program.run)
end

function program.cancel()
  tmr.stop(1)
end

return program
