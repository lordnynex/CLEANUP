--[[
author: jie123108@163.com
date: 20151017
]]

local mysql_util = require("dao.mysql_util")
local util = require("util.util")
local json = require("util.json")
local config = require("config")
local basedao = require "dao.basedao"
local error = require('dao.error')

local _M = {}

local mt = { __index = _M }

function _M:new(connection)
    local dao = basedao:new("application", 
        {app='string', appname='string', remark='string', 
        create_time='number', update_time='number'}, connection)

    return setmetatable({ dao = dao}, mt)
end

function _M:list(page, page_size)
    return self.dao:list(nil, page, page_size)
end

function _M:count()
    local ok, obj = self.dao:count_by(nil)
    return ok, obj
end

function _M:save(values)
    return self.dao:save(values)
end

function _M:exist(field, value)
    return self.dao:exist(field, value)
end

function _M:get_by_app(app)
    return self.dao:get_by("where app=" .. ngx.quote_sql_str(app))
end

return  _M

--[[
local appdao = _M

ok, err  = appdao.save({app="KB01", appname="kuaibo 01 proj"})
ngx.say(ok, err)
ok, err  = appdao.save({app="KB002", appname="kuaibo 002 proj"})
ngx.say(ok, err)

local ok, values = appdao.count()
ngx.say("[", ok, ",", json.dumps(values), "]")

]]