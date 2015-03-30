local program={}
program.settings={}

program.brightness = 255
program.settings.brightness = "1-255%"
program.fadetime = 5
program.settings.fadetime = "1-300s"

-- internal use
program.hue = 0

function program.run()
  program.hue = (program.hue + math.random(60, 300)) %360
  local sat = math.random(0, 255)
  local r,g,b = hsl2rgb(program.hue, sat, program.brightness)
  local time = program.fadetime *1000
  led.set(0, -1, r, g, b, time)
  tmr.alarm(1, time, 0, program.run)
end

function program.cancel()
  tmr.stop(1)
end

return program
