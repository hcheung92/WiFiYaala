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
  return programs.change()
 end
end
function programs.stop()
 if programs.active ~= nil then
   programs.active.cancel()
 end
end
function programs.dofile(file)
   programs.dounload()
   programs.file=file
   tmr.alarm(0, 200, 0, programs.doload)
   return file
end
function programs.dounload()
   programs.active=nil
   tmr.alarm(1, 200, 0, function() end)
   collectgarbage()
end
function programs.doload()
 programs.active=dofile(programs.file)
 programs.active.run()
 return programs.file
end
return programs
