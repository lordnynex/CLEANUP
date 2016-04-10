-- smtp方法发送mail
local smtp = require("smtp")
local cjson = require "cjson"

from = "<zhaomangzheng@126.com>" -- 发件人

-- 发送列表
rcpt = {
	"<zhaomangzheng@163.com>",
	"<805920692@qq.com>"
}

mesgt = {
	headers = {
		to = "zhaomangzheng@163.com", -- 收件人
		cc = '<805920692@qq.com>', -- 抄送
		subject = 'hello',
	},
	body = "<p>验证码为：783234</p><a href=\"#\">欢迎访问</a>"
}
mesgt.headers["content-type"] = 'text/html; charset="utf-8"'

local mailt = {
	server="smtp.126.com",
	user="zhaomangzheng@126.com",
	password="binshared",
	from = from,
	rcpt = rcpt,
--	source = smtp:message(mesgt)
	mesgt = mesgt
}


local s = smtp:new(mailt.server, mailt.port, mailt.create)
local ext = s:greet(mailt.domain)

local auth = s:auth(mailt.user, mailt.password, ext)

--local source = s:message(mesgt)
--mailt.source = source

local BACK_STRING = "success"
local code, reply = s:send(mailt)
if not code then
	ngx.log(ngx.ERR, "mail send error: ", reply)
	BACK_STRING = "fail"
end
s:quit()
s:close()



ngx.req.set_header('Content-Type', "json")
local rt = {}
rt.msg = BACK_STRING
ngx.say(cjson.encode(rt))
