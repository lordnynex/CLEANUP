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
    local dao = basedao:new("role", 
                   {id='string', 
                    name='string', 
                    remark='string', 
                    app='string', 
                    permission='string',
                    create_time='number',
                    update_time='number'},connection)

    return setmetatable({ dao = dao}, mt)
end

function _M:list(app, page, page_size)
    local sql_where = nil
    if app then
        sql_where = "where app=" .. ngx.quote_sql_str(app)
    end
    return self.dao:list(sql_where, page, page_size)
end

function _M:count(app)
    local sql_where = nil
    if app then
        sql_where = "where app=" .. ngx.quote_sql_str(app)
    end
    local ok, obj = self.dao:count_by(sql_where)
    return ok, obj
end

function _M:save(values)
    return self.dao:save(values)
end

function _M:update(values, update_by_values)
    return self.dao:update(values, update_by_values)
end

function _M:exist(field, value)
    return self.dao:exist(field, value)
end

function _M:get_by_id(id)
	id = ngx.quote_sql_str(id)
    local ok, obj = self.dao:get_by("where id=" .. tostring(id))
    if not ok then
        return  ok, obj
    end

    local permissions = {}
    if obj.permission then
        permissions = util.split(obj.permission, "|")
    end
    obj.permissions = permissions

    return ok, obj
end

function _M:exist_exclude(field, value, id)
    return self.dao:exist_exclude(field, value, id)
end

function _M:delete_by_id(id)
    local where = "where id=" .. ngx.quote_sql_str(id)
    return self.dao:delete_by(where)
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