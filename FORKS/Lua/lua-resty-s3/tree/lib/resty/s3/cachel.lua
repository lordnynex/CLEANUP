local md5 = require "resty.md5"
local string = require "resty.string"

local _M = {}
local mt = { __index = _M }

function _M:new(cache_dir)
    return setmetatable({
        cache_dir = cache_dir}, mt)
end

function _M:gen_key(data)
    local key = nil
    local k = md5:new()
    if k then
        k:update(data)
        key = string.to_hex(k:final())
    end
    return key
end

function _M:get(key)
    local file, err = io.open(self.cache_dir..key, "rb")
    if not file then
        return nil, err
    else
        return file:read("*a")
    end
end

function _M:set(key, value)
    local file, err = io.open(self.cache_dir..key, "w+")
    if not file then
        return nil, err
    else
        file:write(value)
        file:close()
        return 1
    end
end

function _M:delete(key)
    local r, err = os.remove(self.cache_dir..key)
    if not r then
        return nil, err
    else
        return 1
    end
end

return _M
