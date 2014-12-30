# **NodeMcu** #
version 0.9.4
###A lua based firmware for wifi-soc esp8266
Build on [ESP8266 sdk 0.9.4](http://bbs.espressif.com/viewtopic.php?f=5&t=90)<br />
Lua core based on [eLua project](http://www.eluaproject.net/)<br />
File system based on [spiffs](https://github.com/pellepl/spiffs)<br />
Open source development kit for NodeMCU [nodemcu-devkit](https://github.com/nodemcu/nodemcu-devkit)<br />
Flash tool for NodeMCU [nodemcu-flasher](https://github.com/nodemcu/nodemcu-flasher)<br />

wiki: [nodemcu wiki](https://github.com/nodemcu/nodemcu-firmware/wiki)<br />
home: [nodemcu.com](http://www.nodemcu.com)<br />
bbs: [中文论坛Chinese bbs](http://bbs.nodemcu.com)<br />
Tencent QQ group QQ群: 309957875<br />

# Change log
2014-12-30<br />
modify uart.on api, when run_input set to 0, uart.on now can read raw data from uart.<br />
serial input now accept non-ascii chars.<br />
fix dev-kit gpio map.<br />
add setip, setmac, sleeptype api to wifi module. <br />
add tmr.time() api to get rtc time and calibration.

[more change log](https://github.com/nodemcu/nodemcu-firmware/wiki/nodemcu_api_en#change_log)<br />
[更多变更日志](https://github.com/nodemcu/nodemcu-firmware/wiki/nodemcu_api_cn#change_log)
# Summary
- Easy to access wireless router
- Based on Lua 5.1.4
- Event-Drive programming preferred.
- Build-in file, timer, pwm, i2c, 1-wire, net, gpio, wifi, adc, uart and system api.
- GPIO pin re-mapped, use the index to access gpio, i2c, pwm.
- GPIO Map Table:

##GPIO NEW TABLE ( Build 20141219 and later)

<a id="new_gpio_map"></a>
<table>
  <tr>
    <th scope="col">IO index</th><th scope="col">ESP8266 pin</th><th scope="col">IO index</th><th scope="col">ESP8266 pin</th>
  </tr>
  <tr>
    <td>0 [*]</td><td>GPIO16</td><td>8</td><td>GPIO15</td>
  </tr>
  <tr>
    <td>1</td><td>GPIO5</td><td>9</td><td>GPIO3</td>
   </tr>
   <tr>
    <td>2</td><td>GPIO4</td><td>10</td><td>GPIO1</td>
  </tr>
  <tr>
    <td>3</td><td>GPIO0</td><td>11</td><td>GPIO9</td>
   </tr>
   <tr>
    <td>4</td><td>GPIO2</td><td>12</td><td>GPIO10</td>
  </tr>
  <tr>
    <td>5</td><td>GPIO14</td><td></td><td></td>
   </tr>
   <tr>
    <td>6</td><td>GPIO12</td><td></td><td></td>
  </tr>
  <tr>
    <td>7</td><td>GPIO13</td<td></td><td></td>
   </tr>
</table>
#### [*] D0(GPIO16) can only be used as gpio read/write. no interrupt supported. no pwm/i2c/ow supported.

##GPIO OLD TABLE (Before build 20141212)

<a id="old_gpio_map"></a>
<table>
  <tr>
    <th scope="col">IO index</th><th scope="col">ESP8266 pin</th><th scope="col">IO index</th><th scope="col">ESP8266 pin</th>
  </tr>
  <tr>
    <td>0</td><td>GPIO12</td><td>8</td><td>GPIO0</td>
  </tr>
  <tr>
    <td>1</td><td>GPIO13</td><td>9</td><td>GPIO2</td>
   </tr>
   <tr>
    <td>2</td><td>GPIO14</td><td>10</td><td>GPIO4</td>
  </tr>
  <tr>
    <td>3</td><td>GPIO15</td><td>11</td><td>GPIO5</td>
   </tr>
   <tr>
    <td>4</td><td>GPIO3</td><td></td><td></td>
  </tr>
  <tr>
    <td>5</td><td>GPIO1</td><td></td><td></td>
   </tr>
   <tr>
    <td>6</td><td>GPIO9</td><td></td><td></td>
  </tr>
  <tr>
    <td>7</td><td>GPIO10</td<td></td><td></td>
   </tr>
</table>

#Build option
####*GNU toolchain is not tested*
####file ./app/include/user_config.h
```c
#define FLASH_512K
// #define FLASH_1M
// #define FLASH_2M
// #define FLASH_4M
...
#define LUA_USE_MODULES
#ifdef LUA_USE_MODULES
#define LUA_USE_MODULES_NODE
#define LUA_USE_MODULES_FILE
#define LUA_USE_MODULES_GPIO
#define LUA_USE_MODULES_WIFI
#define LUA_USE_MODULES_NET
#define LUA_USE_MODULES_PWM
#define LUA_USE_MODULES_I2C
#define LUA_USE_MODULES_TMR
#define LUA_USE_MODULES_ADC
#define LUA_USE_MODULES_UART
#define LUA_USE_MODULES_OW
//#define LUA_USE_MODULES_BIT
#endif /* LUA_USE_MODULES */
```

#Flash the firmware
nodemcu_512k.bin: 0x00000<br />
for most esp8266 modules, just pull GPIO0 down and restart.<br />
You can use the [nodemcu-flasher](https://github.com/nodemcu/nodemcu-flasher) to burn the firmware.

Or, if you build your own bin from source code.<br />
eagle.app.v6.flash.bin: 0x00000<br />
eagle.app.v6.irom0text.bin: 0x10000<br />
esp_init_data_default.bin: 0x7c000<br />
blank.bin: 0x7e000<br />


#Connect the hardware in serial
braudrate:9600

#Start play

####Connect to your ap

```lua
    print(wifi.sta.getip())
    --0.0.0.0
    wifi.setmode(wifi.STATION)
    wifi.sta.config("SSID","password")
    print(wifi.sta.getip())
    --192.168.18.110
```

####Manipulate hardware like a arduino
   
```lua
    pin = 1
    gpio.mode(pin,gpio.OUTPUT)
    gpio.write(pin,gpio.HIGH)
    print(gpio.read(pin))
```

####Write network application in nodejs style
   
```lua
    -- A simple http client
    conn=net.createConnection(net.TCP, 0) 
    conn:on("receive", function(conn, payload) print(payload) end )
    conn:connect(80,"115.239.210.27")
    conn:send("GET / HTTP/1.1\r\nHost: www.baidu.com\r\n"
        .."Connection: keep-alive\r\nAccept: */*\r\n\r\n")
```

####Or a simple http server
   
```lua
    -- A simple http server
    srv=net.createServer(net.TCP) 
    srv:listen(80,function(conn) 
      conn:on("receive",function(conn,payload) 
        print(payload) 
        conn:send("<h1> Hello, NodeMcu.</h1>")
      end) 
      conn:on("sent",function(conn) conn:close() end)
    end)
```

####Do something shining
```lua
  function led(r,g,b) 
    pwm.setduty(1,r) 
    pwm.setduty(2,g) 
    pwm.setduty(3,b) 
  end
  pwm.setup(1,500,512) 
  pwm.setup(2,500,512) 
  pwm.setup(3,500,512)
  pwm.start(1) 
  pwm.start(2) 
  pwm.start(3)
  led(512,0,0) -- red
  led(0,0,512) -- blue
```

####And blink it
```lua
  lighton=0
  tmr.alarm(1,1000,1,function()
    if lighton==0 then 
      lighton=1 
      led(512,512,512) 
    else 
      lighton=0 
      led(0,0,0) 
    end 
  end)
```

####If you want to run something when system started
```lua
  --init.lua will be excuted
  file.open("init.lua","w")
  file.writeline([[print("Hello, do this at the beginning.")]])
  file.close()
  node.restart()  -- this will restart the module.
```

####With below code, you can telnet to your esp8266 now
```lua
    -- a simple telnet server
    s=net.createServer(net.TCP,180) 
    s:listen(2323,function(c) 
       function s_output(str) 
          if(c~=nil) 
             then c:send(str) 
          end 
       end 
       node.output(s_output, 0)   -- re-direct output to function s_ouput.
       c:on("receive",function(c,l) 
          node.input(l)           -- works like pcall(loadstring(l)) but support multiple separate line
       end) 
       c:on("disconnection",function(c) 
          node.output(nil)        -- un-regist the redirect output function, output goes to serial
       end) 
       print("Welcome to NodeMcu world.")
    end)
```

####Use DS18B20 module extends your esp8266
```lua
    -- read temperature with DS18B20
    t=require("ds18b20")
    t.setup(9)
    addrs=t.addrs()
    -- Total DS18B20 numbers, assume it is 2
    print(table.getn(addrs))
    -- The first DS18B20
    print(t.read(addrs[1],t.C))
    print(t.read(addrs[1],t.F))
    print(t.read(addrs[1],t.K))
    -- The second DS18B20
    print(t.read(addrs[2],t.C))
    print(t.read(addrs[2],t.F))
    print(t.read(addrs[2],t.K))
    -- Just read
    print(t.read())
    -- Just read as centigrade
    print(t.read(nil,t.C))
    -- Don't forget to release it after use
    t = nil
	ds18b20 = nil
    package.loaded["ds18b20"]=nil   
```
