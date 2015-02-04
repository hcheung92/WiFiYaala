mywifi={}
mywifi.check = function(callback, remaining)
     if wifi.sta.status() == 5 then
          print(wifi.sta.getip())
          callback()
     elseif remaining > 0 then
          tmr.alarm(0, 1000, 0, function () mywifi.check(callback, remaining - 1000) end)
     else
          print("sorry, no ip (wifi status is " .. wifi.sta.status() ..")!")
          callback()
     end
end
mywifi.init = function(callback, timeout)
     wifi.setmode(wifi.STATION)
     wifi.sta.config(mywifi.ssid,mywifi.passwd)
     mywifi.check(callback, timeout)
end
