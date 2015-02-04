programs = {}
programs.current = 1
programs.names = {}
programs.change = function ()
     if programs.current > #programs.names then
          print("no program available")
          programs.current = 1
          return
     end
     local name = programs.names[programs.current]
     tmr.stop(1)
     print("program " .. name)
     programs[name.."_default"]()
     programs.current = 1 + (programs.current % #programs.names)
end
programs.stop = function()
     print("off!")
     tmr.stop(1)
     led.set(0, 200,0,0,0)
     led.set(0, 0,0,0,1000)
end
