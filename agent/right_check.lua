--[[
author: jie123108@163.com
date: 20151017
]]
local util = require("util.util")
local json = require("util.json")
local ck = require('util.cookie')

local login_url = "/nright/login"
local no_access_page = "/nright/no_access_page"
local right_check_url = "/nright/right_check"


local function uri_args_as_args(ext_args)
	local args = ngx.req.get_uri_args()
	local full_uri = (ngx.var.scheme or "http") .. "://" .. ngx.var.host .. ngx.var.uri
	args["uri"] = full_uri
	if ext_args and type(ext_args) == 'table' then
		for k, v in pairs(ext_args) do 
			args[k] = v
		end
	end
	return ngx.encode_args(args)
end

local function check_uri_permission(app, uri, cookie)
	local retry_max = 3
	local right_check_url_full = util.get_full_uri(right_check_url)
	local res, err = nil
    for i = 1,retry_max do    	
    	local args = ngx.encode_args({app=app, uri=uri, cookie=cookie})
        local url = right_check_url_full .. "?" .. args
        res, err = util.http_get(url, {})

        if res then
            ngx.log(ngx.INFO, "check permission request:", url, ", status:", res.status, ",body:", tostring(res.body))

            if res.status < 500 then
                break
            else
                ngx.log(ngx.ERR, string.format("request [curl -v %s] failed! status:%d", url, res.status))
            end
        else
            ngx.log(ngx.ERR, "fail request: ", url, " err: ", err)
        end
    end
    if not res then
        return false, 500
    end


    if res.status ~= 200 then
    	local body = res.body or ""
    	body = util.trim(body)
    	local userinfo, err = json.loads(body)
	    if err then
	    	userinfo = body
	    end

        return false, res.status, userinfo, res.headers
    else 
    	local userinfo, err = json.loads(res.body)
	    if err then
	    	ngx.log(ngx.ERR, "json.loads(", res.body, ") failed! err:", err)
	    end   

    	return true, res.status, userinfo, res.headers
    end
end

local function check_right()
	local url = ngx.var.uri
	if util.url_in_ignore_list(url) then
		ngx.log(ngx.INFO, "check right, ignore current request!")
		return
	end
	local app = ngx.var.app or "def"

	ngx.log(ngx.INFO, "Cookie: ", ngx.var.http_cookie, ", app=", app)
	
	local cookie_value = ck.get_cookie()
	if cookie_value == nil then
		ngx.log(ngx.WARN, "no rights to access [", url, "], need login!")
		util.redirect(login_url, uri_args_as_args())
	elseif cookie_value == "logouted" then
		ngx.log(ngx.WARN, "logouted, no rights to access [", url, "], need login!")
		util.redirect(login_url, uri_args_as_args())
	end
	ngx.log(ngx.INFO, "Cookie: ", cookie_value)
	ngx.ctx.cookie = cookie_value
	
	-- TODO: 取出COOKIE
	local ok, status, userinfo, headers = check_uri_permission(app, url, cookie_value)
	ngx.log(ngx.INFO, " check_uri_permission(app=", tostring(app),
		",url=", tostring(url), ",cookie=", tostring(cookie_value), ")=", 
		ok, ",", tostring(status), ", res.body:", tostring(userinfo))

	local username = nil
	if type(userinfo) == 'table' and userinfo.username then
		--ngx.log(ngx.INFO, "-------------------------:", json.dumps(userinfo))
		ngx.ctx.userinfo = userinfo
		username = userinfo.username
	end
	if headers and headers["Set-Cookie"] then
		local cookie_value = headers["Set-Cookie"]
		ngx.header['Set-Cookie'] = cookie_value
		ngx.log(ngx.INFO, "******* Re Set-Cookie:", cookie_value, " *******")
	end
	if ok then
		---
	else
		-- no permission.
		if status == ngx.HTTP_UNAUTHORIZED then
			if userinfo == "session-timeout" then
				util.redirect(login_url, uri_args_as_args())
			else 
				util.redirect(no_access_page, uri_args_as_args({username=username}))
			end
		else
			ngx.status = status
			ngx.send_headers()
			ngx.flush(true)
			ngx.say("NRight check permission failed! status:" .. tostring(status))
			ngx.flush(true)
		end
	end

end

check_right()