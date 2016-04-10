local math = require "math"
local udp = ngx.socket.udp
local format = string.format
local _M = {
    _VERSION = '0.01'
}

local mt = {
    __index = _M
}

function _M.new(self, host, port)
    local sock = udp()
    local ok, err = sock:setpeername(host, port)
    if not ok then
        ngx.log(ngx.ERR, "init udp socket failed")
        return nil, "init udp socket failed"
    end

    local init = {
        sock = sock,
        SC_TIMING = "ms",
        SC_COUNT = "c",
        SC_GAUGE = "g",
        SC_SET = "s"
    }

    return setmetatable(init, mt)
end


function _M.timing(self, stats, value )
    self:update_stats(stats, value, self.SC_TIMING)
end

function _M.gauge( self, stats, vaule )
    self:update_stats(stats, value, self.SC_GAUGE)
end

function _M.set( self, stats, value )
    self:update_stats(stats, value, self.SC_SET)
end

function _M.increment( self, stats, sample_rate)
    local sample_rate = sample_rate or 1
    self:count(stats, 1, sample_rate)
end

function _M.decrement( self, stats, sample_rate )
    local sample_rate = sample_rate or 1
    self:count(stats, -1, sample_rate)
end

function _M.count( self, stats, value, sample_rate )
    local sample_rate = sample_rate or 1
    self:update_stats(stats, value, self.SC_COUNT, sample_rate)
end

function _M.update_stats(self, stats, value, _type, sample_rate)
    local sample_rate = sample_rate or 1
    local data = self:format(stats, value, _type)
    local megs = self:sample(data, sample_rate)
    self:send(megs)
end

function _M.format(self, keys, value, _type )
    local data = {}
    local value = format("%d|%s", value, _type)
    local ks
    if type(keys) ~= "table" then
        ks = {keys}
    else
        ks = keys
    end
    for k,v in pairs(ks) do
        data[v] = value
    end
    return data
end

function _M.sample(self, data, sample_rate )
    if sample_rate >= 1 then
        return data
    elseif sample_rate < 1 then
        if math.random() <= sample_rate then
            local sample_data = {}
            for stat, value in pairs(data) do
                sample[stat] = format('%s|@%s', value, sample_rate)
            end
            return sample_data
        end
    return {}
    end
end

function _M.send( self, data )
    for k,v in pairs(data) do
        local msg = format("%s:%s", k, v)
        -- ngx.log(ngx.ERR, "send msg: "..msg)
        self.sock:send(msg)
    end
end

return _M