
这是一个基于openresty的smtp实现，从luaSocket的smtp.lua模仿而来

但由于ngx_openresty中tcpsock已经很好的做到了同步非阻塞，因此基于它实现起来更加简单

邮件内容部分，实现比较简单，仅支持简单的文本，适用于网站发验证码之类的应用，暂不支持附件扩展


lib:	conf/smtp.lua

test:	conf/sendCheckMail.lua
