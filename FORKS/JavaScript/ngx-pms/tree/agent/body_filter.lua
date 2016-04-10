--[[
author: jie123108@163.com
date: 20151017
]]
local config = require("config")

local login_url = "/nright/login"
local logout_url = "/nright/logout"
local change_pwd_url = "/nright/change_pwd"
local change_pwd_post_url = "/nright/change_pwd_post"
local login_post_url = "/nright/login_post"
local no_access_page = "/nright/no_access_page"
local right_check_url = "/nright/right_check"

local ignore_list = {login_url, login_post_url, right_check_url, change_pwd_url, change_pwd_post_url}

local function is_ignore_url(url)
	if ignore_list == nil then
		return false
	end
	local matched = false
	-- 精确匹配。
	if type(ignore_list)=='table' then
		for i, item in ipairs(ignore_list) do 
			--ngx.log(ngx.INFO, "### compare(", item, ",", url, ")...")
			if item == url then
				matched = true
				break
			end
		end
	end
	return matched
end

ngx.log(ngx.INFO, "url:", ngx.var.uri)
if is_ignore_url(ngx.var.uri) then
	ngx.log(ngx.INFO, "### body-filter ignore : ", ngx.var.uri)
	return
end

local topbar_tpl = [[
<style type="text/css">
<!--
.topbar {
	top: 0px;
	height:25px;
	background-color: #E5E5E5;
	font-size: 12px;
	background-position: top;
	border-top-width: thin;
	border-right-width: thin;
	border-bottom-width: thin;
	border-left-width: thin;
	border-top-style: none;
	border-right-style: solid;
	border-bottom-style: solid;
	border-left-style: none;
	border-top-color: #0033FF;
	border-right-color: #000000;
	border-bottom-color: #000000;
	border-left-color: #0033FF;
}
-->
</style>
<table width="100%%" border="0" cellspacing="1" cellpadding="0" class="topbar">
  <tr>
  	<td align="left">&nbsp;&nbsp;Nginx Permission System</td>
    <td align="right">&nbsp;&nbsp;USER: %s| <a %s>Change Password</a> | <a href="%s" target="_self">Logout</a>&nbsp;&nbsp;&nbsp;&nbsp;</td>
  </tr>
</table>
]]

local function get_infobar()
	--template.caching(tmpl_caching)
	--local infobar = template.compile(topbar_tpl){username='liuxiaojie'}
	local username = "NONE"
	local userinfo = ngx.ctx.userinfo
	if userinfo then
		username = userinfo.username
	elseif ngx.var.arg_username then
		username = ngx.var.arg_username
	end
	local href = string.format([[href="%s"  target="_blank"]], change_pwd_url)
	if config.not_allow_change_pwd then
		href = string.format([[href="#" onclick="javascript:alert('have no permission to change password!');"]])
	end
	ngx.log(ngx.INFO, "user [", username, "] request...")
	local replace = string.format(topbar_tpl, username, href, logout_url)
	return true, replace
end

local ok, body = get_infobar()
if ok then
	ngx.arg[1] =  ngx.re.sub(ngx.arg[1], "\\<body[^\\>]*\\>", "$0 " .. body , "jom")
	ngx.log(ngx.INFO, "### add infobar. ###")
end