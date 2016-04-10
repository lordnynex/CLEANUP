--[[
author: jie123108@163.com
date: 20151020
]]
local template = require "resty.template"
local config = require("config")
local util = require("util.util")
local ck = require('util.cookie')
local userdao = require('dao.user_dao')
local cookiedao = require("dao.cookie_dao")
local r = require "util.res"
local dwz = require("Manager.lua.dwzutil")

local login_url = "/passport/login"

local _M = {}

local function get_and_parse_cookie()
	local url = ngx.var.uri
	local cookie_value = ck.get_cookie()
	if cookie_value == nil then
		ngx.log(ngx.WARN, "no rights to access [", url, "], need login!")
		util.redirect(login_url)
	elseif cookie_value == "logouted" then
		ngx.log(ngx.WARN, "logouted, no rights to access [", url, "], need login!")
		util.redirect(login_url)
	end
	ngx.log(ngx.INFO, "Cookie: ", cookie_value)
	ngx.ctx.cookie = cookie_value
	
	local cookie = ck.parse_cookie(cookie_value)
	return cookie
end

function _M.check()
	ngx.log(ngx.INFO, "Cookie: ", ngx.var.http_cookie)
	local cookie = get_and_parse_cookie()

	local ok, userinfo = cookiedao.cookie_get(cookie)
	if ok then
		ngx.ctx.userinfo = userinfo
	else
		ngx.log(ngx.ERR, "cookie_get(", tostring(cookie), ") failed! err:", userinfo)
		util.redirect(login_url)
	end
end

function _M.login_render(args)
	template.caching(tmpl_caching)
	template.render("login.html", args)
	ngx.exit(0)
end

function _M.login_post()
	ngx.req.read_body()
    local args, err = ngx.req.get_post_args()
    if args.username == nil or args.username == "" then
    	args["errmsg"] = "用户名不能为空！"
    	_M.login_render(args)
    end
    if args.password == nil or args.password == "" then
    	args["errmsg"] = "密码不能为空！"
    	_M.login_render(args)
    end
    args.uri = args.uri or "/"

    local dao = userdao:new()
	local ok, userinfo = dao:get_by_name(args.username)
	if ok then
		local pwd_md5 = util.make_pwd(args.password)
		if userinfo.password ~= pwd_md5 then
			args["errmsg"] = "用户名或密码错误！"
    		_M.login_render(args)
		end
		-- 没有管理权限。
		if not (userinfo.manager == "admin" or userinfo.manager == "super") then
			args["errmsg"] = "该用户没有管理员权限！"
    		_M.login_render(args)
		end

		local cookie = ck.make_cookie(userinfo)
		ck.set_cookie(cookie)
		cookiedao.cookie_set(cookie, userinfo)

		util.redirect(args.uri)
	else 
		args["errmsg"] = "用户名或密码错误！"
    	_M.login_render(args)
	end

end

function _M.logout_post()
	local userinfo = ngx.ctx.userinfo
	local username = nil
	if userinfo then
		username = userinfo.username
	end
	ngx.ctx.userinfo = nil
	ck.set_cookie("logouted")
	-- TODO: cache_cookie_clear
    _M.login_render({username=username})
end

function _M.changepwd_render()
	template.caching(tmpl_caching)
	template.render("changepwd.html")
	ngx.exit(0)
end

function _M.changepwd_post()
	ngx.req.read_body()
    local args, err = ngx.req.get_post_args()
    local userinfo = ngx.ctx.userinfo
    if userinfo == nil then
    	args["errmsg"] = "请登录后再修改密码！"
    	_M.login_render(args)
    end
	local cookie = get_and_parse_cookie()
    local old_pwd_md5 = util.make_pwd(args.old_password)
	if userinfo.password ~= old_pwd_md5 then
		ngx.say(dwz.cons_resp(300, "旧密码错误！"))
    	ngx.exit(0)
	end

	if args.password ~= args.repassword then
		ngx.say(dwz.cons_resp(300, "请两次输入一样的密码！"))
    	ngx.exit(0)
	end
	local pwd_md5 = util.make_pwd(args.password)
	local dao = userdao:new()
	local ok = dao:change_pwd(userinfo.id, pwd_md5)
	if ok then
		userinfo.password = pwd_md5
		cookiedao.cookie_set(cookie, userinfo)
		local msg = "用户【" .. tostring(userinfo.username).. "】密码修改成功！"
		ngx.log(ngx.INFO, "user [", tostring(userinfo.username), "] success to change password!")
		ngx.say(dwz.cons_resp(200, msg, {callbackType="closeCurrent"}))
    	ngx.exit(0)
	else 
		local msg = "用户【" .. tostring(userinfo.username).. "】修改密码时出错，请联系管理员！"
		ngx.log(ngx.INFO, "user [", tostring(userinfo.username), "] fail to change password!")
		ngx.say(dwz.cons_resp(300, msg))
    	ngx.exit(0)
	end
end

return _M