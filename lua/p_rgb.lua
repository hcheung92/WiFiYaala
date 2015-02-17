local program = {}
program.mode=0
program.run=function()
  if program.mode==0 then led.set(0, 255,0,0,500)
  elseif program.mode==1 then led.set(0,0,255,0,500)
  elseif program.mode==2 then led.set(0,0,0,255,500)
  end
  program.mode = (program.mode+1)%3
  tmr.alarm(1, 1000, 0, program.run)
end
program.cancel=function()
  tmr.stop(1)
end
return program