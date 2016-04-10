if table.getn(statsd.buffer) > 50 then 
   statsd.flush(ngx.socket.udp, "127.0.0.1", 8125) 
end
