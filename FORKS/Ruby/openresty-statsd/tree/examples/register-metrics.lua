local status = ngx.var.status
local req_time = ngx.now() - ngx.req.start_time()

statsd.incr("test.status." .. status)
statsd.time("test.response", req_time)
