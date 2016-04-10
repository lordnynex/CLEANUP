--[[
author: jie123108@163.com
date: 20151014
]]
local template = require "resty.template"
local config = require("config")
local permdao = require("dao.perm_dao")
local mysql = require("dao.mysql_util")
local viewpub = require("Manager.lua.viewpub")
local json = require("util.json")
local dwz = require("Manager.lua.dwzutil")
local util = require("util.util")
local error = require('dao.error')
local tmpl_caching = config.tmpl_caching
if tmpl_caching == nil then
	tmpl_caching = false
end

local _M = {}

function _M.list_render()
	local errmsg = nil
	local totals = 0
	ngx.req.read_body()
    local args, err = ngx.req.get_post_args()
	local pageNum = tonumber(args.pageNum) or 1
	local numPerPage = tonumber(args.numPerPage) or config.defNumPerPage

    local cur_userinfo = ngx.ctx.userinfo
    local app = nil
    if cur_userinfo.manager ~= "super" then
        app = cur_userinfo.app
    end

    local totals = 0
    local dao = permdao:new()
    local ok, permissions = dao:list(app, pageNum, numPerPage)
    if not ok then
        if permissions == error.err_data_not_exist then

        else
            errmsg = permissions
            ngx.log(ngx.ERR, "permdao:list(", tostring(app), ") failed! err:", tostring(permissions))
        end
        permissions = {}
    else
        ok, totals = dao:count(app)
        if not ok then
            totals = 0
        end
    end
    local cur_manager = nil
    if ngx.ctx.userinfo then
        cur_manager = ngx.ctx.userinfo.manager
    end
	template.caching(tmpl_caching)
	template.render("perm_list.html", {errmsg=errmsg, perms=permissions, cur_manager=cur_manager,
                    pageNum=pageNum, numPerPage=numPerPage, totals=totals})
	ngx.exit(0)
end

function _M.add_render()
	local args = ngx.req.get_uri_args()
    local id = args.id
    local ok, permission = nil
    if id then
        local dao = permdao:new()
        ok, permission = dao:get_by_id(id)
        if not ok then
            ngx.log(ngx.ERR, "permdao:get_by_id(", id, ") failed! err:", tostring(permission))
            ngx.say(dwz.cons_resp(300, "修改权限信息时出错：" .. tostring(permission)))
            ngx.exit(0)
        end
    end
    local app, apps = viewpub.get_app_and_apps()

	template.caching(tmpl_caching)
	template.render("perm_add.html", {permission=permission, apps=apps})
	ngx.exit(0)
end

function _M.add_post()
	ngx.req.read_body()
    local args, err = ngx.req.get_post_args()

    local update = args.update or false
    local id = args.id
    local name = args.name
    local remark = args.remark
    local app = args.app
    local create_time = ngx.time()
    local update_time = ngx.time()

    local perminfo = {id=id,name=name,remark=remark, app=app,
    				create_time=create_time,update_time=update_time}

    -- 检查应用是否存在。
    local dao = permdao:new()
    if update then
        local ok, exist = dao:exist_exclude("name", name, id)
        if ok and exist then
            ngx.say(dwz.cons_resp(300, "修改权限信息时出错了, 权限名称[" .. name .. "]已经存在!"))
            ngx.exit(0)
        end

        perminfo['id'] = nil
        perminfo['create_time'] = nil
        local ok, err = dao:update(perminfo, {id=id})
        if not ok then
            ngx.log(ngx.ERR, "permdao:update(", json.dumps(perminfo), ") failed! err:", tostring(err))
            if err == error.err_data_exist then
               ngx.say(dwz.cons_resp(300, "修改权限信息时出错了: 数据重复"))
            else
               ngx.say(dwz.cons_resp(300, "修改权限信息时出错了:" .. tostring(err)))
            end
            ngx.exit(0)
        end
        ngx.say(dwz.cons_resp(200, "权限【" .. id .. "】修改成功", {navTabId="perm_list", callbackType="closeCurrent"}))
    else
        local ok, exist = dao:exist("id", id)
        if ok and exist then
            ngx.say(dwz.cons_resp(300, "保存权限信息时出错了, 权限ID[" .. id .. "]已经存在!"))
            ngx.exit(0)
        end
        local ok, exist = dao:exist("name", name)
        if ok and exist then
            ngx.say(dwz.cons_resp(300, "保存权限信息时出错了, 权限名称[" .. name .. "]已经存在!"))
            ngx.exit(0)
        end

        local ok, err = dao:save(perminfo)
        if not ok then
        	ngx.log(ngx.ERR, "permdao:save(", json.dumps(perminfo), ") failed! err:", tostring(err))
            if err == error.err_data_exist then
               ngx.say(dwz.cons_resp(300, "保存应用信息时出错了: 数据重复"))
            else
        	   ngx.say(dwz.cons_resp(300, "保存应用信息时出错了:" .. tostring(err)))
            end
        	ngx.exit(0)
        end
        ngx.say(dwz.cons_resp(200, "权限【" .. id .. "】添加成功", {navTabId="perm_list", callbackType="closeCurrent"}))
    end
end


function _M.del_post()
    local args = ngx.req.get_uri_args()
    local id = args.id
    if not id then
        ngx.say(dwz.cons_resp(300, "删除权限信息时出错了, 缺少权限ID!"))
        ngx.exit(0)
    end
    -- 检查用户是否存在
    local dao = permdao:new()
    local ok, perminfo = dao:get_by_id(id)
    if not ok then
        if perminfo == error.err_data_not_exist then
            perminfo = '权限ID不存在'
        end
        ngx.say(dwz.cons_resp(300, "删除权限信息时出错了，错误：" .. tostring(perminfo)))
        ngx.exit(0)
    end

    local ok, err = dao:delete_by_id(id)
    if not ok then
        ngx.log(ngx.ERR, "dao:delete_by_id(", tostring(id), ") failed! err:", tostring(err))
        
        if err == error.err_data_exist then
            ngx.say(dwz.cons_resp(300, "删除权限信息时出错了，权限不存在"))
        else
           ngx.say(dwz.cons_resp(300, "删除权限信息时出错了:" .. tostring(err)))
        end     
        ngx.exit(0)
    end
   
    ngx.say(dwz.cons_resp(200, "权限【" .. id .. "】删除成功！", {navTabId="perm_list"}))
end

return _M
