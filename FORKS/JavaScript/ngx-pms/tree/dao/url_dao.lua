--[[
author: jie123108@163.com
date: 20151020
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
    local dao = basedao:new("url_perm", 
                   {id='string',                     
                    app='string',   
                    type='string', 
                    url='string',           
                    url_len='number',
                    permission='string',  
                    create_time='number',
                    update_time='number'}, connection)

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

function _M:_exist_internal(app,type,url, id)
    app = ngx.quote_sql_str(app)
    type = ngx.quote_sql_str(type)
    url = ngx.quote_sql_str(url)

    local where = string.format("WHERE app=%s AND type=%s AND url=%s", app, type, url)
    if id then
        id = tostring(id)
        where = where .. " AND id != " .. id
    end
    local ok, count = self.dao:count_by(where)
    if ok then
        return ok, count>0
    else
        return ok, count 
    end
end

function _M:exist(app,type,url)
    return self:_exist_internal(app, type, url)
end

function _M:exist_exclude(app,type,url, id)
    return self:_exist_internal(app, type, url, id)
end

function _M:get_by_id(id)
    return self.dao:get_by("where id=" .. tostring(id))
end

function _M:delete_by_id(id)
    local where = "where id=" .. tostring(id)
    return self.dao:delete_by(where)
end

function _M:url_perm_get(app, url)
    app = ngx.quote_sql_str(app)
    url = ngx.quote_sql_str(url)

    local wheres = {}
    -- 优先查找精确匹配。
    table.insert(wheres, "where app=" .. app .. " and type='equal' and url = " .. url)
    -- 然后查找后缀匹配的。
    table.insert(wheres, "where app=" .. app .. " and type='suffix' and url = right(" .. url .. ", length(url)) order by url_len desc limit 1")
    -- 然后查找前缀匹配的。
    table.insert(wheres, "where app=" .. app .. " and type='prefix' and url = substr(" .. url .. ", 1, length(url)) order by url_len desc limit 1")

    local ok, obj, where_ = nil,nil
    for i, where in ipairs(wheres) do
        where_ = where
        ok, obj = self.dao:get_by(where)
        if ok then
            return ok, obj
        elseif obj ~= error.err_data_not_exist then --出错
            ngx.log(ngx.ERR, "url_dao.get_by(" .. tostring(where) .. ") failed! err:", tostring(obj))
            return ok, obj
        end
    end
    if not ok then
        ngx.log(ngx.ERR, "url_dao.get_by(" .. tostring(where_) .. ") failed! err:", tostring(obj))
    end
    return ok, obj
end

return  _M
