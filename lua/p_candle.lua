local program={}
program.color=40
program.saturation=150
program.brightness=200
program.mode=0
function program.run()
  if program.mode == nil then program.mode=tmr.now()%9 end
  if program.mode < 5 then -- flicker
    local col = tmr.now() % 20
    local bright = tmr.now() % 50 + 20
    local r,g,b = hsl2rgb((program.color + (col - 10)) % 360, program.saturation, program.brightness - bright)
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
    local r,g,b = hsl2rgb(program.color + (col - 5), program.saturation, program.brightness)
    led.set(0, -1, r, g, b, 500)
    program.mode=nil
    tmr.alarm(1, 5000, 0, program.run)
  else -- flicker
    local col = tmr.now() % 20
    local bright = tmr.now() % 50
    local r,g,b = hsl2rgb((program.color + (col - 10)) % 360, program.saturation, program.brightness - bright)
    led.set(0, -1, r, g, b, 300)
    program.mode=nil
    tmr.alarm(1, 1000, 0, program.run)
  end
end
function program.cancel()
  tmr.stop(1)
end
return program
