programs.demo = function (mode, cnt)
     local time = 100
     local h = 0
     local s = 255
     local l = 255
     local fadetime = 0
     if mode == 0 then
          h = ((tmr.now())%360)
          fadetime = 2500
          time = 3000
     elseif mode == 1 then
          h = (cnt * 7) %360
          l = (cnt * 50) %255
          fadetime = 500
          time = 500
     elseif mode == 2 then
          h = (cnt % 3) * 120
          l = (cnt % 2) * 255
          fadetime = 0
          time = 100 - l / 3
     elseif mode == 3 then
          h = ((tmr.now())%360)
          l = (cnt % 2) * 255
          if l == 0 then time = 15 + cnt * 10
          else time = 15
          end
     elseif mode == 4 then
          h = ((tmr.now())%360)
          l = (cnt % 2) * 255
          if l == 0 then time = 515 - cnt * 10
          else time = 15
          end
          fadetime = 100
     elseif mode == 5 then
          h = (cnt % 2) * 120
          if cnt % 2 == 0 then time = 110 - cnt * 2
          else time = 10 + cnt * 2
          end
     elseif mode == 6 then
          h = ((tmr.now())%360)
          l = (cnt % 3) * 255
          times = {2000, 1000, 500}
          time = times[(cnt%3) + 1]
          fadetime = time+200
     end
     local r,g,b = hsl2rgb(h, s, l)
     led.set(0,r,g,b,fadetime)
     if cnt > 50 then
          mode = (mode + 1) % 7
          cnt = 0
     else cnt = cnt + 1
     end
     tmr.alarm(1, time, 0, function() programs.demo(mode, cnt) end)
end
programs.demo_default = function () programs.demo(6,0) end
table.insert(programs.names, "demo")