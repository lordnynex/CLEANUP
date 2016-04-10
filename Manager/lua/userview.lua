--[[
author: jie123108@163.com
date: 20151019
]]

local template = require "resty.template"
local config = require("config")
local userdao = require('dao.user_dao')
local viewpub = require("Manager.lua.viewpub")
local roledao = require("dao.role_dao")
local userpermdao = require("dao.user_perm_dao")
local mysql = require("dao.mysql_util")
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
    if args.username == "" then
        args.username = nil
    end
    if args.email == "" then
        args.email = nil
    end
    if args.tel == "" then
        args.tel = nil
    end
    local cur_userinfo = ngx.ctx.userinfo
    local app = nil
    if cur_userinfo.manager ~= "super" then
        app = cur_userinfo.app
        ngx.log(ngx.INFO, "----------------: cur_userinfo.app: ", app)
    end
    args['app'] = app
    args['with_public'] = true
    ngx.log(ngx.INFO, "--------------:", cur_userinfo.manager)

	local dao = userdao:new()
	local pageNum = tonumber(args.pageNum) or 1
	local numPerPage = tonumber(args.numPerPage) or config.defNumPerPage
	local ok, users = dao:list(args, pageNum, numPerPage)
	if not ok then
		if users == error.err_data_not_exist then
			totals = 0
			users = {}
		else
			errmsg = users
			users = nil
		end
	else 
		ok, totals = dao:count(args)
		if not ok then
			totals = 0
		end
	end
	
	template.caching(tmpl_caching)
	template.render("user_list.html", {errmsg=errmsg, users=users, 
                    username=args.username, email=args.email, tel=args.tel,
                    pageNum=pageNum, numPerPage=numPerPage, totals=totals})
	ngx.exit(0)
end

function _M.add_render()
	local args = ngx.req.get_uri_args()
    local id = tonumber(args.id)
	
    local cur_userinfo = ngx.ctx.userinfo

    local ok, userinfo = nil
    if id then
        local dao = userdao:new()
        dao:set_app(cur_userinfo.app)
        ok, userinfo = dao:get_by_id(id)
        if not ok then
            ngx.log(ngx.ERR, "userdao:get_by_id(", id, ") failed! err:", tostring(userinfo))
            ngx.say(dwz.cons_resp(300, "修改用户信息时出错：" .. tostring(userinfo)))
            ngx.exit(0)
        end
    end
    
    local app, apps = viewpub.get_app_and_apps(true, true)
	local permissions = viewpub.get_permissions(app)
    -- 权限ID->权限名称的映射表，用于WEB页面展示使用。
    local permissions_all = viewpub.get_permissions()
    local perm_map = viewpub.perm_map(permissions_all)

    local dao = roledao:new()
    local role_ok, roles = dao:list(app, 1, 1024)
    if not role_ok then
        if roles == error.err_data_not_exist then

        else
            ngx.log(ngx.ERR, "roledao:list(", tostring(roles), ") failed! err:", tostring(roles))
        end
        roles = {{id="", name="无", remark=""}}
    else
        table.insert(roles, 1, {id="", name="无", remark=""})
    end


    if userinfo then
        permissions = viewpub.perm_sub(permissions, userinfo.user_permissions)
    end
    local cur_manager = nil
    if ngx.ctx.userinfo then
        cur_manager = ngx.ctx.userinfo.manager
    end

	template.caching(tmpl_caching)
	template.render("user_add.html", {permission_others=permissions, perm_map=perm_map, 
            apps=apps, roles=roles, userinfo=userinfo,cur_manager=cur_manager})
	ngx.exit(0)
end

function _M.add_post()
	ngx.req.read_body()
    local args, err = ngx.req.get_post_args()
    local id = tonumber(args.id)
    local username = args.username
    local email = args.email
    local tel = args.tel 
    local app = args.app
    local manager = args.manager or ""
    local role_id = args.role_id or ""
    local permission = args.permission or {}
    local create_time = ngx.time()
    local update_time = ngx.time()

    local cur_userinfo = ngx.ctx.userinfo
    local permissioin_app = cur_userinfo.app
    if type(permission) == 'table' then
        permission = table.concat(permission, "|")
    end
    local userinfo = {username=username, email=email, tel=tel,
    					app=app,manager=manager,role_id=role_id,
    					create_time=create_time,update_time=update_time}
    
    -- 检查用户是否存在
    local dao = userdao:new()
    if id then
        local ok, exist = dao:exist_exclude("username", username, id)
        if ok and exist then
            ngx.say(dwz.cons_resp(300, "修改用户信息时出错了, 用户名[" .. username .. "]已经存在!"))
            ngx.exit(0)
        end
        local ok, exist = dao:exist_exclude("email", email, id)
        if ok and exist then
            ngx.say(dwz.cons_resp(300, "修改用户信息时出错了, EMAIL[" .. email .. "]已经存在!"))
            ngx.exit(0)
        end
        if tel and string.len(tel) > 0 then
            local ok, exist = dao:exist_exclude("tel", tel, id)
            if ok and exist then
                ngx.say(dwz.cons_resp(300, "修改用户信息时出错了, TEL[" .. tel .. "]已经存在!"))
                ngx.exit(0)
            end
        else 
            userinfo["tel"] = nil
        end
        userinfo["create_time"] = nil

        -- 开启事务
        local ok, connection = mysql.connection_get()
        if not ok then
            ngx.log(ngx.ERR, "mysql.connection_get failed! err:", tostring(connection))
            ngx.say(dwz.cons_resp(300, "获取数据库链接出错了:" .. tostring(connection)))
            ngx.exit(0)
        end
        local tx_ok, tx_err = mysql.tx_begin(connection)
        if not tx_ok then
            ngx.log(ngx.ERR, "mysql.tx_begin failed! err:", tostring(tx_err))
        end

        local dao = userdao:new(connection)
        local ok, err = dao:update(userinfo, {id=id})
        if not ok then
            if tx_ok then 
                tx_ok, tx_err = mysql.tx_rollback(connection)
            end
            mysql.connection_put(connection)

            ngx.log(ngx.ERR, "userdao:update(", json.dumps(userinfo),",", json.dumps({id=id}), ") failed! err:", tostring(err))
            if err == error.err_data_exist then
                ngx.say(dwz.cons_resp(300, "修改用户信息时出错了: 数据重复"))
            else
                ngx.say(dwz.cons_resp(300, "修改用户信息时出错了:" .. tostring(err)))
            end     
            ngx.exit(0)
        end
        local dao = userpermdao:new(connection)
        if permission == nil or permission == "" then
            local ok, err = dao:delete(id, permissioin_app)
            if not ok and err ~= error.err_data_not_exist then
                if tx_ok then 
                    tx_ok, tx_err = mysql.tx_rollback(connection)
                end
                mysql.connection_put(connection)
                ngx.log(ngx.ERR, "userpermdao:delete(", id,",", permissioin_app, ") failed! err:", tostring(err))
                ngx.say(dwz.cons_resp(300, "修改用户权限时出错了:" .. tostring(err)))
                ngx.exit(0)
            end
        else
            local userperm_values = {userid=id, app=permissioin_app, permission=permission, 
                                     create_time=ngx.time(), update_time=ngx.time()}
            local ok, err = dao:saveOrUpdate(userperm_values)
            if not ok then
                if tx_ok then 
                    tx_ok, tx_err = mysql.tx_rollback(connection)
                end
                mysql.connection_put(connection)
                ngx.log(ngx.ERR, "userpermdao:saveOrUpdate(", json.dumps(userperm_values), ") failed! err:", tostring(err))
                ngx.say(dwz.cons_resp(300, "修改用户权限时出错了:" .. tostring(err)))
                ngx.exit(0)
            end
        end
        -- 提交事物
        tx_ok, tx_err = mysql.tx_commit(connection)
        mysql.connection_put(connection)

        ngx.say(dwz.cons_resp(200, "用户【" .. username .. "】修改成功", {navTabId="user_list", callbackType="closeCurrent"}))
    else
        local ok, exist = dao:exist("username", username)
        if ok and exist then
            ngx.say(dwz.cons_resp(300, "保存用户信息时出错了, 用户名[" .. username .. "]已经存在!"))
            ngx.exit(0)
        end
        local ok, exist = dao:exist("email", email)
        if ok and exist then
            ngx.say(dwz.cons_resp(300, "保存用户信息时出错了, EMAIL[" .. email .. "]已经存在!"))
            ngx.exit(0)
        end
        if tel and string.len(tel) > 0 then
            local ok, exist = dao:exist("tel", tel)
            if ok and exist then
                ngx.say(dwz.cons_resp(300, "保存用户信息时出错了, TEL[" .. tel .. "]已经存在!"))
                ngx.exit(0)
            end
        else 
            userinfo["tel"] = nil
        end
        local password = util.random_pwd(16)
        userinfo["password"] = util.make_pwd(password)

        -- 开启事务
        local ok, connection = mysql.connection_get()
        if not ok then
            ngx.log(ngx.ERR, "mysql.connection_get failed! err:", tostring(connection))
            ngx.say(dwz.cons_resp(300, "获取数据库链接出错了:" .. tostring(connection)))
            ngx.exit(0)
        end
        local tx_ok, tx_err = mysql.tx_begin(connection)
        if not tx_ok then
            ngx.log(ngx.ERR, "mysql.tx_begin failed! err:", tostring(tx_err))
        end

        local dao = userdao:new(connection)        
        local ok, id = dao:save(userinfo)
        if not ok then
            local err = id
            if tx_ok then 
                tx_ok, tx_err = mysql.tx_rollback(connection)
            end
            mysql.connection_put(connection)
            ngx.log(ngx.ERR, "userdao:save(", json.dumps(userinfo), ") failed! err:", tostring(err))
            if err == error.err_data_exist then
                ngx.say(dwz.cons_resp(300, "保存用户信息时出错了: 数据重复"))
            else
               ngx.say(dwz.cons_resp(300, "保存用户信息时出错了:" .. tostring(err)))
            end     
            ngx.exit(0)
        end

        if permission ~= nil and permission ~= "" then   
            local dao = userpermdao:new(connection)         
            local userperm_values = {userid=id, app=permissioin_app, permission=permission, 
                                     create_time=ngx.time(), update_time=ngx.time()}
            local ok, err = dao:saveOrUpdate(userperm_values)
            if not ok then
                if tx_ok then 
                    tx_ok, tx_err = mysql.tx_rollback(connection)
                end
                mysql.connection_put(connection)
                ngx.log(ngx.ERR, "userpermdao:saveOrUpdate(", json.dumps(userperm_values), ") failed! err:", tostring(err))
                ngx.say(dwz.cons_resp(300, "保存用户权限时出错了:" .. tostring(err)))
                ngx.exit(0)
            end
        end

        -- 提交事物
        tx_ok, tx_err = mysql.tx_commit(connection)
        mysql.connection_put(connection)

         --TODO: 密码提示框会小时问题修改。
        ngx.say(dwz.cons_resp(200, string.format([[用户【%s】添加成功: <br/>
&nbsp;&nbsp;用户名：%s<br/>&nbsp;&nbsp;密码：%s<br/>
请记住以上信息！]], username, username, password), {navTabId="user_list", callbackType="closeCurrent"}))
    end
end

function _M.del_post()
    local args = ngx.req.get_uri_args()
    local id = tonumber(args.id)
    if not id then
        ngx.say(dwz.cons_resp(300, "删除用户信息时出错了, 缺少用户ID!"))
        ngx.exit(0)
    end
    -- 检查用户是否存在
    local dao = userdao:new()
    local ok, userinfo = dao:get_by_id(id)
    if not ok then
        ngx.say(dwz.cons_resp(300, "删除用户信息时出错了，错误：" .. tostring(userinfo)))
        ngx.exit(0)
    end 
    if userinfo.manager == "super" then
        ngx.say(dwz.cons_resp(300, "超级管理员不能删除！"))
        ngx.exit(0)
    elseif userinfo.manager == "admin" then
        ngx.say(dwz.cons_resp(300, "管理员用户不能删除！"))
        ngx.exit(0)
    end
    local cur_userinfo = ngx.ctx.userinfo
    -- 普通管理员不能删除public的用户。
    if cur_userinfo.manager == "admin" and userinfo.app == "public" then
        ngx.say(dwz.cons_resp(300, "普通管理员不能删除公共帐号！"))
        ngx.exit(0)
    end

    local ok, err = dao:delete_by_id(id)
    if not ok then
        ngx.log(ngx.ERR, "dao:delete_by_id(", tostring(id), ") failed! err:", tostring(err))
        
        if err == error.err_data_exist then
            ngx.say(dwz.cons_resp(300, "删除用户信息时出错了，用户不存在"))
        else
           ngx.say(dwz.cons_resp(300, "删除用户信息时出错了:" .. tostring(err)))
        end     
        ngx.exit(0)
    end
   
    --TODO: 密码提示框会小时问题修改。
    ngx.say(dwz.cons_resp(200, "用户【" .. userinfo.username .. "】删除成功！", {navTabId="user_list"}))
end

function _M.detail_render()
    local method_name = ngx.req.get_method()
    local args, err = nil
    if method_name == "POST" then
        ngx.req.read_body()
        args, err = ngx.req.get_post_args()
    else
        args = ngx.req.get_uri_args()
    end
    local id = tonumber(args.id)
    if not id then
        ngx.log(ngx.ERR, "args [id] missing!")
        ngx.say(dwz.cons_resp(300, "缺少必要的参数"))
        ngx.exit(0)
    end

    local app = viewpub.get_app_and_apps(false)
    local permissions = viewpub.get_permissions(app)
    -- 权限ID->权限名称的映射表，用于WEB页面展示使用。
    local perm_map = viewpub.perm_map(permissions)

    local dao = userdao:new()
    local ok, userinfo = dao:get_by_id(id)    

    if ok then
        --ngx.log(ngx.INFO, "---[", json.dumps(userinfo), "]---")
        template.caching(tmpl_caching)
        template.render("user_detail.html", {userinfo=userinfo, perm_map=perm_map})
        ngx.exit(0)
    else
        ngx.log(ngx.ERR, "dao:get_by_id(", tostring(id), ") failed err:", tostring(userinfo))
        ngx.say(dwz.cons_resp(300, "系统错误，查询用户信息时出错！"))
        ngx.exit(0)
    end

end

return _M
