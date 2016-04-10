--[[
author: jie123108@163.com
date: 20150914
]]

local config = require("config")
local util = require("util.util")
local ck = require("resty.cookie")   -- https://github.com/cloudflare/lua-resty-cookie

local _M = {}

function _M.make_cookie(userinfo)
	local cookie = ngx.md5(tostring(userinfo["id"]) .. ":" .. 
				tostring(userinfo["username"]) .. ":" .. util.random_pwd(10))
	--return string.format("%s:%s:%s", tostring(userinfo["id"]), tostring(userinfo["username"]), cookie)
	return cookie
end

function _M.parse_cookie(cookie)
	--[[
	local arr = util.split(cookie, ':')
	if #arr == 3 then
		return tonumber(arr[1]), arr[2], arr[3]
	else 
		return nil, 'invalid cookie'
	end]]
	return cookie
end

function _M.get_cookie()
	local cookie, err = ck:new()
    if not cookie then
        ngx.log(ngx.ERR, "ck:new() failed!", err)
        return nil
    end

    local cookie_key = "nright"
    if config.cookie_config and config.cookie_config.key then
    	cookie_key = config.cookie_config.key
    end

	local cookie_value, err = cookie:get(cookie_key)
	return cookie_value
end

function _M.set_cookie(value)
	--"%s=%s;domain=%s;path=%s;expires=%s"
	if config.cookie_config == nil then
		return false
	end
    local key = "nright"
    if config.cookie_config and config.cookie_config.key then
    	key = config.cookie_config.key
    end

	local domain = config.cookie_config.domain
	local path = config.cookie_config.path
	local expires = config.cookie_config.expires

	local cookie_value = tostring(key) .. "=" .. tostring(value)
	if domain then
		cookie_value = cookie_value .. ";domain=" .. domain
	end
	if path == nil then
		path = "/"
	end
	cookie_value = cookie_value .. ";path=" .. path
	
	if expires then
		local expires_time = ngx.cookie_time(ngx.time() + expires)
		cookie_value = cookie_value .. ";expires=" .. expires_time
	end
	ngx.header['Set-Cookie'] = cookie_value
	ngx.log(ngx.INFO, "******* Set-Cookie:", cookie_value)
	return true
end

function _M.set_cookie_ex(key, value, domain)
	--"%s=%s;domain=%s;path=%s;expires=%s"
	if config.cookie_config == nil then
		return false
	end

	local path = config.cookie_config.path
	local expires = config.cookie_config.expires


	local cookie_value = key .. "=" .. value
	if domain then
		cookie_value = cookie_value .. ";domain=" .. domain
	end
	if path == nil then
		path = "/"
	end
	cookie_value = cookie_value .. ";path=" .. path
	
	if expires then
		local expires_time = ngx.cookie_time(ngx.time() + expires)
		cookie_value = cookie_value .. ";expires=" .. expires_time
	end
	ngx.header['Set-Cookie'] = cookie_value
	ngx.log(ngx.INFO, "******* Set-Cookie:", cookie_value)
	return true
end

return _M