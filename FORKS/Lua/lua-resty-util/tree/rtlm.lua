local args = ngx.req.get_uri_args()
local logfile = args.item
if logfile == nil then
    ngx.log(ngx.ERR, "error arg of item")
    ngx.say("Require item argument")
    ngx.exit(415)
end
local wsserver = require "resty.websocket.server"
local ffi  = require "ffi"
local cjson = require "cjson"
local heartbeatFailureCount = 0

ffi.cdef[[
        typedef signed char     int8_t;
        typedef unsigned char   uint8_t;
        typedef signed int  int16_t;
        typedef unsigned int    uint16_t;
        typedef signed long int     int32_t;
        typedef unsigned long int   uint32_t;
        typedef signed long long int    int64_t;
        typedef unsigned long long int  uint64_t;
        typedef unsigned long long int  uint64_t;
        typedef int mode_t;
        typedef long int __off64_t;
        typedef __off64_t off_t;
        typedef long int ssize_t;

        int open(const char * pathname, int flags);
        int close(int fd);
        off_t lseek(int fildes, off_t offset, int whence);
        ssize_t read(int fd, void * buf, size_t count);

        int inotify_init(void);
        int inotify_init1(int flags);
        int inotify_add_watch(int fd, const char *pathname, uint32_t mask);
        int inotify_rm_watch(int fd, int wd);
]]

local wb, err = wsserver.new{
    timeout = 5000,
    max_payload_len = 65535,
}
if not wb then
    ngx.log(ngx.ERR, "Failed to new websocket: ", err)
    return ngx.exit(500)
end

local SEEK_END = 2
local PLATFORM = ffi.os
local O_NONBLOCK
-- linux: 2048
-- OSX: 4
if PLATFORM == "Linux" then
    O_NONBLOCK = 2048
elseif PLATFORM == "OSX" then
    O_NONBLOCK = 4
end

local IN_MODIFY = 0x00000002
local chunk_size = 4096
local buffer = ffi.new('char[?]', chunk_size)
local logpath = "/you/path"
local postfix = ".log"
local filename = logpath..logfile..postfix
local cfilename = ffi.new("char["..#filename+1 .."]", filename)
local file = io.open(filename, "r")
local fd = ffi.C.inotify_init1(O_NONBLOCK)
local wd = ffi.C.inotify_add_watch(fd, cfilename, IN_MODIFY)
if not file and (fd ~= -1) and (wd ~= -1) then
    ngx.log(ngx.ERR, "open file and init inotify watch failed")
    ngx.exit(415)
end
local offset = file:seek("end")

local function pushlogs()
    while true do
    --  when event occur for monitored file, get the offset and filesize of monitored file
    --  then to calculate that how many bytes have been appended to monitored file and read it
    --  send to websocket client
        ngx.sleep(0.5)
        local nread = ffi.C.read(fd, buffer,chunk_size);
        nread = tonumber(nread)
        if nread >0 then
            text = file:read("*all")
            if #text > 0 then
                local bytes, err = wb:send_text(text)
            end
        elseif nread < -1 then
            ngx.log(ngx.ERR, "get event failed")
            return ngx.exit(500)
        end
    end
end

local function heartbeat()
    while true do
        local bytes, err = wb:send_ping()
        if not bytes then
            ngx.log(ngx.ERR, "failed to send frame: ", err)
        end
        ngx.sleep(30)
    end
end

local t_heartbeat = ngx.thread.spawn(heartbeat)
local t_push = ngx.thread.spawn(pushlogs)

local function cleanjobs()
    ftype = io.type(file)
    if ftype == "file" then
    	file:close()
    end
    ffi.C.inotify_rm_watch(fd, wd)
    ffi.C.close(fd)
    local ok, err = ngx.thread.kill(t_push)
    if err then
        ngx.log(ngx.ERR, "kill tsend thead failed")
    end
    local ok, err = ngx.thread.kill(t_heartbeat)
    if err then
        ngx.log(ngx.ERR, "kill send_ping thead failed")
    end
    ngx.log(ngx.ERR, "Close log file and rm wactch instanse ")
end

local ok, err = ngx.on_abort(cleanjobs)
if not ok then
     ngx.log(ngx.ERR, "failed to register the on_abort callback: ", err)
     ngx.exit(500)
end

while true do
    if heartbeatFailureCount >=3 then
         cleanjobs()
        return ngx.exit(415)
    end
    local data, typ, err = wb:recv_frame()
    if wb.fata then
        ngx.log(ngx.ERR, "fatal error already happened")
        cleanjobs()
        return ngx.exit(444)
    end

    if typ == "close" then
        cleanjobs()
        local bytes, err = wb:send_close(1000, "enough, enough!")
        if not bytes then
            ngx.log(ngx.ERR, "failed to send the close frame: ", err)
            return
        end
        local code = err
        ngx.log(ngx.INFO, "closing with status code ", code, " and message ", data)
        return
    end


    if err and string.find(err, ": timeout", 1, true) then
        ngx.log(ngx.ERR, "Read timeout, time: ", heartbeatFailureCount)
        heartbeatFailureCount = heartbeatFailureCount + 1
    elseif typ == "ping" then
        local bytes, err = wb:send_pong(data)
        if not bytes then
            ngx.log(ngx.ERR, "failed to send pong frame: ", err)
        end
    end
end
