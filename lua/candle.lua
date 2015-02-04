programs.candle = function(color, brightness, mode, cnt)
     if mode == nil then mode = tmr.now() % 9 end
     if mode < 5 then -- flicker
          local col = tmr.now() % 20
          local bright = tmr.now() % 50 + 20
          local r,g,b = hsl2rgb((color + (col - 10)) % 360, 100, brightness - bright)
          local time = 150 + mode * 20
          led.set(0, r, g, b, time)
          if cnt == nil then cnt = tmr.now() % 10 + 5
          elseif cnt <= 0 then mode = nil
          else cnt = cnt - 1
          end
          tmr.alarm(1, time - 20, 0, function () programs.candle(color, brightness, mode, cnt) end)
     elseif mode < 8 then -- still
          local col = tmr.now() % 10
          local r,g,b = hsl2rgb(color + (col - 5), 100, brightness)
          led.set(0, r, g, b, 500)
          tmr.alarm(1, 5000, 0, function() programs.candle(color, brightness) end)
     else -- flicker
          local col = tmr.now() % 20
          local bright = tmr.now() % 50
          local r,g,b = hsl2rgb((color + (col - 10)) % 360, 100, brightness - bright)
          led.set(0, r, g, b, 300)
          tmr.alarm(1, 1000, 0, function () programs.candle(color, brightness) end)
     end
end
programs.candle_default = function() programs.candle(40, 200) end
table.insert(programs.names, "candle")
