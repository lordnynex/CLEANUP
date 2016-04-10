--[[
author: jie123108@163.com
date: 20151017
]]

local mysql_util = require("dao.mysql_util")
local util = require("util.util")
local json = require("util.json")
local config = require("config")
local basedao = require "dao.basedao"
local roledao = require("dao.role_dao")
local userpermdao = require("dao.user_perm_dao")

local error = require('dao.error')

local _M = {}

local mt = { __index = _M }

function _M:new(connection)
    local dao = basedao:new("user", 
                   {id='number', 
                    username='string', 
                    email='string', 
                    tel='string', 
                    password='string', 
                    app='string', 
                    manager='string',
                    role_id='string', 
                    create_time='number',
                    update_time='number'}, connection)

    return setmetatable({ dao = dao}, mt)
end

local function get_where_sql(args)
    if args == nil then
        return nil
    end
    local fields = {}
    if args.app then
        if args.with_public then
            table.insert(fields, "(app = 'public' or app = " .. ngx.quote_sql_str(args.app) .. ")")
        else 
            table.insert(fields, "app = " .. ngx.quote_sql_str(args.app))
        end
    end
    if args.username then
        table.insert(fields, "username LIKE " .. ngx.quote_sql_str("%" .. args.username .. "%"))
    end
    if args.email then
        table.insert(fields, "email LIKE " .. ngx.quote_sql_str("%" .. args.email .. "%"))
    end
    if args.tel then
        table.insert(fields, "tel LIKE " .. ngx.quote_sql_str("%" .. args.tel .. "%"))
    end
    if #fields == 0 then
        return nil
    end
    return "WHERE " .. table.concat(fields, " AND ")
end


local function _get_user_permission(userid, app)
    local user_permissions = nil
    local permission = nil
    local dao = userpermdao:new()
    local ok, user_perms = dao:get(userid, app)
    if ok then
        user_permissions = user_perms.permissions
        permission = user_perms.permission
    else 
        ngx.log(ngx.ERR, "userpermdao:get(", userid, ") failed! err:", tostring(user_perms))
    end
    return user_permissions, permission
end

local function _get_role_permissioin(role_id)
    local role_permissions = nil
    if role_id and role_id ~= "" then
        local rdao = roledao:new()
        local ok, role = rdao:get_by_id(role_id)
        if not ok then
            ngx.log(ngx.ERR, "roledao:get_by_id(", role_id, ") failed! err:", tostring(role))
        else 
            if role.permissions and type(role.permissions) == 'table' then
                role_permissions = role.permissions
            end
        end
    end
    return role_permissions
end

function _M:list(args, page, page_size)   
    local sql_where = get_where_sql(args)
    local ok, objs = self.dao:list(sql_where, page, page_size)
    if ok and type(objs) == 'table' then
        for _, obj in ipairs(objs) do 
            local user_permissions, permission = _get_user_permission(obj.id)
            obj.permission = permission 
            obj.user_permissions = user_permissions 
        end
    end
    return ok, objs
end

function _M:count(args)
    local sql_where = get_where_sql(args)
    return self.dao:count_by(sql_where)
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

function _M:exist_exclude(field, value, id)
    return self.dao:exist_exclude(field, value, id)
end

    --[[ userinfo属性说明：
    permission: 字符串|分割的原始权限列表
    user_permissions：用户权限列表，list格式
    role_permissions: 用户从角色继承来的权限列表，list格式
    permissions：用户所有权限列表，map格式。
    permission_alt：权限提示列表。
    ]]
function _user_get_internal(dao, id, username, app)
    local ok, obj 
    if id then
        id = tonumber(id)
        ok, obj = dao:get_by("where id=" .. tostring(id))
    elseif username then
        username = ngx.quote_sql_str(username)
        ok, obj = dao:get_by("where username=" .. username)
    else
        ngx.log(ngx.ERR, "_user_get_internal failed! args 'id','username' missing!")
        return false, error.err_data_not_exist
    end
    
    if not ok then
        return  ok, obj
    end

    local permissions = nil
    ngx.log(ngx.INFO, "user.permission: ", tostring(obj.permission))

    if obj.permission then
        permissions = util.split(obj.permission, "|")
    end

    local role_permissions = _get_role_permissioin(obj.role_id)
    local user_permissions, permission = _get_user_permission(obj.id, app)

    obj.permissions = util.merge_array_as_map(user_permissions, role_permissions)
    obj.user_permissions = user_permissions 
    obj.role_permissions = role_permissions
    if user_permissions then
        obj.permission_alt = table.concat(user_permissions, ",")
    end
    --[[
    ngx.log(ngx.INFO, "------------------------------------")
    for k, v in pairs(obj.permissions) do
        ngx.log(ngx.INFO, "---- ", k)
    end
    ]]
    return  ok, obj
end

function _M:get_by_name(username)
    return _user_get_internal(self.dao, nil, username, self.app)
end

function _M:get_by_id(userid)
   return _user_get_internal(self.dao, userid, nil, self.app) 
end

function _M:set_app(app)
    self.app = app
    return true
end

function _M:delete_by_id(userid)
    local where = "where id=" .. userid
    return self.dao:delete_by(where)
end

function _M:change_pwd(userid, password)
    return self.dao:update({password=password}, {id=userid})
end
return  _M
