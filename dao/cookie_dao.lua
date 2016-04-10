--[[
author: jie123108@163.com
date: 20151107
]]

local json = require("util.json")
local config = require("config")
local util = require("util.util")
local userdao = require("dao.user_dao")

local cachename = "cookies"

local _M = {}

-- Cookie 信息保存为两种：
-- ck:${COOKIE} ==> userid
-- user:${userid} ==> userinfo

function _M.cookie_set(cookie, userinfo)
	local cache = ngx.shared[cachename]
	if cache then
		local userid = tostring(userinfo.id)
		local key_cookie = "ck:" .. cookie
		local key_userinfo = "user:" .. userid
		local exptime = config.cookie_config.expires + ngx.time()
		local ok, err = cache:safe_set(key_cookie, userid, exptime)
		if not ok then
			ngx.log(ngx.ERR, "cache:safe_set(", key_cookie, ",", userid, ") failed! err:", err)
			return false
		end
		userinfo = json.dumps(userinfo)
		local ok, err = cache:safe_set(key_userinfo, userinfo, exptime)
		if not ok then
			ngx.log(ngx.ERR, "cache:safe_set(", key_userinfo, ",", userinfo, ") failed! err:", err)
			return false
		end
		ngx.log(ngx.INFO, "cookie_set(cookie=", cookie, ",userinfo=", userinfo, 
							",exptime=",exptime, ") success!")
		return true
	else
		ngx.log(ngx.ERR, "lua_shared_dict named '", cachename, "' not defined!")
		return false
	end
end

local function get_userinfo_from_db(cache, userid)
	local key_userinfo = "user:" .. userid
	local exptime = config.cookie_config.expires + ngx.time()
	local dao = userdao:new()
	local ok, userinfo = dao:get_by_id(userid)
	if not ok then
		return ok, userinfo
	end
	local ok, err = cache:safe_set(key_userinfo, json.dumps(userinfo), exptime)
	if not ok then
		ngx.log(ngx.ERR, "cache:safe_set(", key_userinfo, ",", userid, ") failed! err:", err)
		return false, "safe_set-err"
	end
	return ok, userinfo
end

function _M.cookie_del(cookie)
	local cache = ngx.shared[cachename]
	if cache then
		local key_cookie = "ck:" .. cookie
		cache:delete(key_cookie)
		return true, "success"
	else
		ngx.log(ngx.ERR, "lua_shared_dict named '", cachename, "' not defined!")
		return false, "shared_dict_not_defined"
	end
end

function _M.cookie_get(cookie)
	local cache = ngx.shared[cachename]
	if cache then
		local key_cookie = "ck:" .. cookie
		local value, flags = cache:get(key_cookie)
		if not value then
			ngx.log(ngx.WARN, "cache:get(", key_cookie, ") failed! not exist!")
			return false, "not-exist"
		end
		local userid = tonumber(value)
		local key_userinfo = "user:" .. userid
		local value, flags = cache:get(key_userinfo)
		if not value then
			ngx.log(ngx.INFO, "-------------- 11111111 ----------------------")
			return get_userinfo_from_db(cache, userid)
		end
		local userinfo, err = json.loads(value)
		if err then
			ngx.log(ngx.ERR, "json.loads(", value, ") failed! err:", err)
			return get_userinfo_from_db(cache, userid)
		else
			-- 重新设置值，以延长过期时间
			_M.cookie_set(cookie, userinfo)

			return true, userinfo
		end
	else
		ngx.log(ngx.ERR, "lua_shared_dict named '", cachename, "' not defined!")
		return false, "shared_dict_not_defined"
	end
end

-- 清除用户信息缓存，当修改用户权限，角色相关信息时，需要调用该方法。
function _M.clean_userinfo()	
	local cache = ngx.shared[cachename]
	local keys = cache:get_keys(1024*10)
	if not keys then
		return true, 0
	end
	local cnt = 0
	for _,key in ipairs(keys) do 
		if util.startswith(key, "user:") then
			cache:delete(key)
			cnt = cnt + 1
		end
	end
	return true, cnt
end

return _M