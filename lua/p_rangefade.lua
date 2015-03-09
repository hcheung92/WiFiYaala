local program={}
-- external config
program.delay = 20000
program.lightness = 255

function program.run() 
     local start = math.random(1, led.inited()) -1
     local ende = math.random(1, led.inited()) -1

     local hue = math.random(0, 359)
     local r,g,b = hsl2rgb(hue, 255, program.lightness)
     local time = math.random((program.delay /10), program.delay)
 
     led.set(start, ende, r, g, b, time)

     tmr.alarm(1, time, 0, program.run)
end

function program.cancel()
  tmr.stop(1)
end

return program