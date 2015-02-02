function run (lightness, time, fadetime)
     tmr.alarm(1, time, 1, function()
          h = ((tmr.now())%360)
          s = ((tmr.now())%255)
          local r,g,b = hsl2rgb(h, s, lightness)
          led.set(0,r,g,b,fadetime)
     end)
end

run(255, 20000, 5000)