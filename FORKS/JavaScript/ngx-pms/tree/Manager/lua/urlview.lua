--[[
author: jie123108@163.com
date: 20151014
]]
local template = require "resty.template"
local config = require("config")
local urldao = require("dao.url_dao")
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
    local dao = urldao:new()
    local ok, urls = dao:list(app, pageNum, numPerPage)
    if not ok then
        if urls == error.err_data_not_exist then

        else
            errmsg = urls
            ngx.log(ngx.ERR, "urldao:list(", tostring(app), ") failed! err:", tostring(urls))
        end
        urls = {}
    else
        ok, totals = dao:count(app)
        if not ok then
            totals = 0
        end
    end
    local _, type_maps = viewpub.get_url_types()
    if ngx.ctx.userinfo then
        cur_manager = ngx.ctx.userinfo.manager
    end
	template.caching(tmpl_caching)
	template.render("url_list.html", {errmsg=errmsg, urls=urls, type_maps=type_maps,cur_manager=cur_manager,
                    pageNum=pageNum, numPerPage=numPerPage, totals=totals})
	ngx.exit(0)
end

function _M.add_render()
	local args = ngx.req.get_uri_args()
    local id = args.id
    local ok, urlinfo = nil
    if id then
        local dao = urldao:new()
        ok, urlinfo = dao:get_by_id(id)
        if not ok then
            ngx.log(ngx.ERR, "urldao:get_by_id(", id, ") failed! err:", tostring(urlinfo))
            ngx.say(dwz.cons_resp(300, "修改URL信息时出错：" .. tostring(urlinfo)))
            ngx.exit(0)
        end
    end
    local app, apps = viewpub.get_app_and_apps()
    local get_sys_perms = true
    local permissions = viewpub.get_permissions(app, get_sys_perms)
    local types = viewpub.get_url_types()

	template.caching(tmpl_caching)
	template.render("url_add.html", {urlinfo=urlinfo, apps=apps, permissions=permissions, types=types})
	ngx.exit(0)
end

function _M.add_post()
	ngx.req.read_body()
    local args, err = ngx.req.get_post_args()

    local id = tonumber(args.id)

    local app = args.app
    local type = args.type
    local url = args.url or ""
    local url_len = string.len(url)
    local permission = args.permission
    local create_time = ngx.time()
    local update_time = ngx.time()
    
    local urlinfo = {app=app,type=type,url=url,url_len=url_len,permission=permission,
    				create_time=create_time,update_time=update_time}

    -- 检查应用是否存在。
    local dao = urldao:new()
    if id then
        -- 精确匹配及前缀匹配的URL必须以'/'开头。
        if (type == "equal" or type == "prefix") and string.sub(url,1,1) ~= '/' then
            ngx.log(ngx.INFO, "-------------- url: ", url)
            ngx.say(dwz.cons_resp(300, "修改URL信息时出错了, 精确匹配或前缀匹配的URL必须以'/'开头!"))
            ngx.exit(0)
        end
        local ok, exist = dao:exist_exclude(app,type,url, id)
        if ok and exist then
            ngx.say(dwz.cons_resp(300, string.format("修改URL信息时出错了, URL{app:%s,type:%s,url:%s}已经存在!", app,type, url)))
            ngx.exit(0)
        end

        urlinfo['create_time'] = nil
        local ok, err = dao:update(urlinfo, {id=id})
        if not ok then
            ngx.log(ngx.ERR, "urldao:update(", json.dumps(urlinfo), ") failed! err:", tostring(err))
            if err == error.err_data_exist then
               ngx.say(dwz.cons_resp(300, "修改URL信息时出错了: 数据重复"))
            else
               ngx.say(dwz.cons_resp(300, "修改URL信息时出错了:" .. tostring(err)))
            end
            ngx.exit(0)
        end
        ngx.say(dwz.cons_resp(200, "URL【" .. url .. "】修改成功", {navTabId="url_list", callbackType="closeCurrent"}))
    else
        -- 精确匹配及前缀匹配的URL必须以'/'开头。
        if (type == "equal" or type == "prefix") and string.sub(url,1,1) ~= '/' then
            ngx.say(dwz.cons_resp(300, "添加URL信息时出错了, 精确匹配或前缀匹配的URL必须以'/'开头!"))
            ngx.exit(0)
        end
        local ok, exist = dao:exist(app,type,url)
        if ok and exist then
            ngx.say(dwz.cons_resp(300, string.format("保存URL信息时出错了, URL{app:%s,type:%s,url:%s}已经存在!", app,type, url)))
            ngx.exit(0)
        end

        local ok, err = dao:save(urlinfo)
        if not ok then
        	ngx.log(ngx.ERR, "urldao:save(", json.dumps(urlinfo), ") failed! err:", tostring(err))
            if err == error.err_data_exist then
               ngx.say(dwz.cons_resp(300, "保存URL信息时出错了: 数据重复"))
            else
        	   ngx.say(dwz.cons_resp(300, "保存URL信息时出错了:" .. tostring(err)))
            end
        	ngx.exit(0)
        end
        ngx.say(dwz.cons_resp(200, "URL【" .. url .. "】添加成功", {navTabId="url_list", callbackType="closeCurrent"}))
    end
end


function _M.del_post()
    local args = ngx.req.get_uri_args()
    local id = args.id
    if not id then
        ngx.say(dwz.cons_resp(300, "删除URL信息时出错了, 缺少URLID!"))
        ngx.exit(0)
    end
    -- 检查用户是否存在
    local dao = urldao:new()
    local ok, urlinfo = dao:get_by_id(id)
    if not ok then
        if urlinfo == error.err_data_not_exist then
            urlinfo = 'URLID不存在'
        end
        ngx.say(dwz.cons_resp(300, "删除URL信息时出错了，错误：" .. tostring(urlinfo)))
        ngx.exit(0)
    end

    local ok, err = dao:delete_by_id(id)
    if not ok then
        ngx.log(ngx.ERR, "dao:delete_by_id(", tostring(id), ") failed! err:", tostring(err))
        
        if err == error.err_data_exist then
            ngx.say(dwz.cons_resp(300, "删除URL信息时出错了，URL不存在"))
        else
           ngx.say(dwz.cons_resp(300, "删除URL信息时出错了:" .. tostring(err)))
        end     
        ngx.exit(0)
    end
   
    ngx.say(dwz.cons_resp(200, "URL【" .. id .. "】删除成功！", {navTabId="url_list"}))
end

return _M
