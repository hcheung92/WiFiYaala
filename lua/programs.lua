local programs={}
programs.current=0
programs.file=""
function programs.change(nr)
 programs.current = programs.current + 1
 local l = file.list()
 local idx = 0
 for k,v in pairs(l) do
  if string.sub(k,1, 2) == "p_" then
   idx=idx+1;
   if idx == programs.current then
    print("change prg:"..k)
    if programs.active ~= nil then
      package.loaded[programs.file]=nil
      programs.active.cancel()
      programs.active=nil
    end
    programs.file=k
    tmr.alarm(0, 200, 0, programs.doload)
    return
   end
  end
 end
 programs.current = 0
 if idx == 0 then
  print("no programs found")
 else
  programs.change()
 end
end
function programs.stop()
 print("off!")
 if programs.active ~= nil then
   programs.active.cancel()
 end
 led.set(0,led.inited()-1,0,0,0,1000)
end
function programs.doload()
 if string.sub(programs.file, -3) == ".lc" then
   programs.active=require(string.sub(programs.file, 1, -4))
 else
   programs.active=dofile(programs.file)
 end
 programs.active.run()
end
return programs
