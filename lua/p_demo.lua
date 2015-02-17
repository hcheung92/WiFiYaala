local program={}
program.mode=0
program.cnt=0
function program.run()
     local time = 100
     local h = 0
     local s = 255
     local l = 255
     local fadetime = 0
     if program.mode == 0 then
          h = ((tmr.now())%360)
          fadetime = 2500
          time = 3000
     elseif program.mode == 1 then
          h = (program.cnt * 7) %360
          l = (program.cnt * 50) %255
          fadetime = 500
          time = 500
     elseif program.mode == 2 then
          h = (program.cnt % 3) * 120
          l = (program.cnt % 2) * 255
          fadetime = 0
          time = 100 - l / 3
     elseif program.mode == 3 then
          h = ((tmr.now())%360)
          l = (program.cnt % 2) * 255
          if l == 0 then time = 15 + program.cnt * 10
          else time = 15
          end
     elseif program.mode == 4 then
          h = ((tmr.now())%360)
          l = (program.cnt % 2) * 255
          if l == 0 then time = 515 - program.cnt * 10
          else time = 15
          end
          fadetime = 100
     elseif program.mode == 5 then
          h = (program.cnt % 2) * 120
          if program.cnt % 2 == 0 then time = 110 - program.cnt * 2
          else time = 10 + program.cnt * 2
          end
     elseif program.mode == 6 then
          h = ((tmr.now())%360)
          l = (program.cnt % 3) * 255
          times = {2000, 1000, 500}
          time = times[(program.cnt%3) + 1]
          fadetime = time+200
     end
     local r,g,b = hsl2rgb(h, s, l)
     led.set(0,r,g,b,fadetime)
     if program.cnt > 50 then
          program.mode = (program.mode + 1) % 7
          program.cnt = 0
     else program.cnt = program.cnt + 1
     end
     tmr.alarm(1, time, 0, program.run)
end
return program