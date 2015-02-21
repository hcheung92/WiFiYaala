local programs={}
programs.file=""
function programs.list()
 local list={}
 local l = file.list()
 for k,v in pairs(l) do
  if string.sub(k,1, 2) == "p_" then
   table.insert(list,k)
  end
 end
 return list
end
function programs.change()
 local next=false
 local list=programs.list()
 if programs.file=="" then next=true end
 for k,v in pairs(list) do
  if next then
   return programs.dofile(v)
  else
   if programs.file==v then next=true end
  end
 end
 programs.file=""
 if #list == 0 then
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
function programs.dofile(file)
   programs.file=file
   tmr.alarm(0, 200, 0, programs.doload)
   return file
end
function programs.doload()
 if string.sub(programs.file, -3) == ".lc" then
   programs.active=require(string.sub(programs.file, 1, -4))
 else
   programs.active=dofile(programs.file)
 end
 programs.active.run()
 return programs.file
end
return programs
