--[[
author: jie123108@163.com
date: 20151108
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
    local dao = basedao:new("user_permission", 
                   {userid='number', 
                    app='string', 
                    permission='string',
                    create_time='number',
                    update_time='number'}, connection)

    return setmetatable({ dao = dao}, mt)
end


function _M:save(values)
    return self.dao:save(values)
end

function _M:update(values, update_by_values)
    return self.dao:update(values, update_by_values)
end

function _M:saveOrUpdate(values)
    return self.dao:saveOrUpdate(values)
end

local function get_where_sql(userid, app)
    local where = "WHERE userid=" .. tostring(userid)
    if app then
        where = where .. " AND app=" .. ngx.quote_sql_str(app)
    end
    return where
end

function _M:get(userid, app)
    local where = get_where_sql(userid, app)
    local ok, objs = self.dao:list_by(where)
    if not ok then
        return ok, objs
    end
    --ngx.log(ngx.INFO, "#################[[", json.dumps(objs), "]]")
    local permissions = {}
    for _, obj in ipairs(objs) do 
        if obj.permission then
            local tmp_permissions = util.split(obj.permission, "|")
            if tmp_permissions then
                for _, p in ipairs(tmp_permissions) do 
                    table.insert(permissions, p)
                end
            end
        end
    end
    local permission = table.concat(permissions, "|")
    local obj = {userid=userid, app=app,permission=permission, permissions=permissions}

    return ok, obj
end

function _M:delete(userid, app)
    local where = get_where_sql(userid, app)
    return self.dao:delete_by(where)
end


return  _M
