local program={}
program.settings={}
program.color=40
program.settings.color="0-360"
program.saturation=150
program.settings.saturation="0-255%"
program.brightness=200
program.settings.brightness="0-255%"
program.mode=0
function program.run()
  if program.mode == nil then program.mode=tmr.now()%9 end
  if program.mode < 5 then -- flicker
    local col = tmr.now() % 20
    local bright = tmr.now() % (program.brightness / 4) + (program.brightness / 8)
    local r,g,b = led.hsl2rgb((program.color + (col - 10)) % 360, program.saturation, program.brightness - bright)
    local time = 150 + program.mode * 20
    led.set(0, -1, r, g, b, time)
    if program.cnt == nil then program.cnt = tmr.now() % 10 + 5
    elseif program.cnt <= 0 then 
      program.mode=nil
      program.cnt=nil
    else program.cnt = program.cnt - 1
    end
    tmr.alarm(1, time - 20, 0, program.run)
  elseif program.mode < 8 then -- still
    local col = tmr.now() % 10
    local r,g,b = led.hsl2rgb(program.color + (col - 5), program.saturation, program.brightness)
    led.set(0, -1, r, g, b, 500)
    program.mode=nil
    tmr.alarm(1, 5000, 0, program.run)
  else -- flicker
    local col = tmr.now() % 20
    local bright = tmr.now() % (program.brightness / 4)
    local r,g,b = led.hsl2rgb((program.color + (col - 10)) % 360, program.saturation, program.brightness - bright)
    led.set(0, -1, r, g, b, 300)
    program.mode=nil
    tmr.alarm(1, 1000, 0, program.run)
  end
end
function program.cancel()
  tmr.stop(1)
end
return program
