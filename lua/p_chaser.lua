local program={}
program.settings={}

program.Mode = 0
program.settings.Mode = "0-1"
program.Color = -1   -- -1 for random colors, otherwise a hue value
program.settings.Color = "-1-360°"
program.Delay = 75
program.settings.Delay = "50-1000ms"
program.Brightness = 255
program.settings.Brightness = "1-255%"
program.Length = 8
program.settings.Length = "1-50"

-- internal use
program.dir = 0
program.index = 0 
program.hue = 0

function program.run() 
     if program.index == 0 then 
          if program.Color == -1 then 
               program.hue = (program.hue + math.random(60, 300)) %360
          else
               program.hue = program.Color
          end
     end
     
     local r,g,b = hsl2rgb(program.hue, 255, program.Brightness)
     led.set(program.index, program.index, r, g, b, program.Delay)

     if program.dir == 0 then
          if program.index > 0 then 
               led.set(program.index-1, program.index-1, 0, 0, 0, program.Delay*program.Length) 
          end 
    
          program.index = program.index +1         
          if program.index == led.inited() then 
               if program.Mode == 0 then -- revert dir
                    program.dir = 1 
                    program.index = program.index -1
               end
          end
     else
          if program.index < (led.inited() -1) then 
               led.set(program.index+1, program.index+1, 0, 0, 0, program.Delay*program.Length) 
          end
          
          program.index = program.index -1
          if program.index == 0 then
               if program.Mode == 0 then -- revert dir
                    program.dir = 0 
                    program.index = program.index +1
               end
          end
     end      

     tmr.alarm(1, program.Delay, 0, program.run)
end

function program.cancel()
  tmr.stop(1)
end

return program
