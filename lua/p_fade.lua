local program={}
program.lightness=255
program.time=20000
program.fadetime=5000
function program.run()
  h = ((tmr.now())%360)
  s = ((tmr.now())%255)
  local r,g,b = hsl2rgb(h, s, program.lightness)
  led.set(0,led.inited()-1,r,g,b,program.fadetime)
  tmr.alarm(1, program.time, 0, program.run)
end
function program.cancel()
  tmr.stop(1)
end
return program
