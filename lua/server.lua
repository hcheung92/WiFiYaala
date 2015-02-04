srv=net.createServer(net.TCP) 
srv:listen(80,function(conn) 
     conn:on("receive",function(conn,payload)
          local url = string.match(string.sub(payload, 1, string.find(payload, "\n") - 1), "%w+ ([%w%p]+) [%w%p]+")
     conn:send("<h1> Hello, NodeMcu.</h1>URL:" .. url .. ".")
     if url == "/toggle" then
          programs.change()
     elseif url == "/off" then
          programs.stop()
     end
          end) 
     conn:on("sent",function(conn) conn:close() end)
end)
print("server is up...")
