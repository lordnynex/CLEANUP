
local _M = {}

--[[ 忽略列表,列表中的项不进行检查，节省时间。
格式：
	ignore_list = {
	equals={"/test", "/login"},
	suffix={".doc", ".jpg"},
	prefix={"/demo", "/error"},
	}
]]
_M.ignore_list = {
	equals={"/favicon.ico"},
	suffix={},
	prefix={}
}

--[[
-- 登录URL。
_M.login_url =  "/nright/login"
-- 权限检查URL
_M.right_check_url = "/nright/right_check"
-- 没权限时，显示的页面
_M.no_access_page = "/nright/no_access_page"
-- 信息栏地址。
_M.infobar_page = "/nright/infobar"

]]
-- Cookie 设置相关参数。
_M.cookie_config = {key="nright", path="/", expires=3600}

-- 数据库配置。
_M.db = {host="127.0.0.1", port=3306,user="root", password="123456",
		database="pms",DEFAULT_CHARSET="utf8"}

-- 列表显示时，默认分页大小
_M.defNumPerPage = 15

-- Password加密使用的盐，在系统使用之前修改，系统开始使用后，请不要修改。
_M.password_magic = '#*nright@0Ol1llOO'

return _M