local program = {}
program.settings={}
program.mode=0
program.brightness=255
program.settings.brightness="0-255%"
program.run=function()
  if program.mode==0 then led.set(0,-1,program.brightness,0,0,500)
  elseif program.mode==1 then led.set(0,-1,0,program.brightness,0,500)
  elseif program.mode==2 then led.set(0,-1,0,0,program.brightness,500)
  end
  program.mode = (program.mode+1)%3
  tmr.alarm(1, 1000, 0, program.run)
end
program.cancel=function()
  tmr.stop(1)
end
return program
