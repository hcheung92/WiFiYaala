local program={}
program.settings={}

program.Delay = 20
program.settings.Delay = "10-300s"
program.Brightness = 255
program.settings.Brightness = "1-255%"

function program.run() 
     local start = math.random(1, led.inited()) -1
     local ende = math.random(1, led.inited()) -1
     local hue = math.random(0, 360)
     local r,g,b = led.hsl2rgb(hue, 255, program.Brightness)
     local time = math.random((program.Delay /10), program.Delay) *1000
 
     led.set(start, ende, r, g, b, time)
     tmr.alarm(1, time, 0, program.run)
end

function program.cancel()
  tmr.stop(1)
end

return program