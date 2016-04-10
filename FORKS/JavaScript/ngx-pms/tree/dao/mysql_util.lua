--[[
author: jie123108@163.com
date: 20150901
]]

local dbConfig = require("config")
local mysql = require("resty.mysql")
local json = require("util.json")
local error = require('dao.error')

local _M = {}
--[[
	先从连接池去连接，如果没有再建立连接
	返回：
		false, 出错信息
		true, 数据库连接
--]]
function _M.connection_get()
	if ngx.ctx.mysql_conn then
		return true, ngx.ctx.mysql_conn
	end

	local client, errmsg = mysql:new()
	if not client then
		ngx.log(ngx.ERR, "mysql:new failed! err:", errmsg)
		return	false, tostring(errmsg)
	end

	local timeout = dbConfig.db["timeout"] or 1000*10
	local max_idle_timeout = dbConfig.db["max_idle_timeout"] or 1000*10
	local pool_size = dbConfig.db["pool_size"] or 100

	client:set_timeout(timeout)	--10秒


	local db_ip = dbConfig.db["host"]
	

	local options = {
		host = db_ip,
		port = dbConfig.db["port"],
		user = dbConfig.db["user"],
		password = dbConfig.db["password"],
		database = dbConfig.db["database"]
	}

	local ok, errmsg = client:connect(options)
	if not ok then
		client:set_keepalive(max_idle_timeout, pool_size)
		ngx.log(ngx.ERR, "mysql:connect(", json.dumps(options) , ") failed! err:", tostring(errmsg))
		return	false, error.err_database
	end

	if dbConfig.db["DEFAULT_CHARSET"] then
		local query = "SET NAMES " .. dbConfig.db["DEFAULT_CHARSET"]
		local result, errmsg, errno, sqlstate = client:query(query)
		if not result then
			client:set_keepalive(max_idle_timeout, pool_size)
			ngx.log(ngx.ERR, "mysql:query(", query, ") failed! err:", tostring(errmsg), ", errno:", tostring(errno))
			return	false, error.err_sql
		end
	end

	return	true, client
end

--[[
	把连接放回连接池
	用set_keepalive代替close将开启连接池特性，可以为每个nginx工作进程指定最大空闲时间和连接池最大连接数
--]]
function _M.connection_put(client)
	if client and client ~= ngx.ctx.mysql_conn then
		local max_idle_timeout = dbConfig.db["max_idle_timeout"] or 1000*10
		local pool_size = dbConfig.db["pool_size"] or 100
		client:set_keepalive(max_idle_timeout, pool_size)
	end
end


--[[
	查询
	有结果集时返回结果集
	无结果集返回查询影响
	返回：
		false, 出错信息, sqlstate结构
		true, 结果集, sqlstate结构
--]]
local function mysql_query_internal(sql, connection)
	local ok, client = true, connection 
	if client == nil then
		ok, client = _M.connection_get()
	end
	if not ok then
		return	false, client
	end

	local result, errmsg, errno, sqlstate = client:query(sql)
	if client ~= connection then
		_M.connection_put(client)
	end

	if not result then
		ngx.log(ngx.ERR, "mysql:query(", sql, ") failed! err:", tostring(errmsg), ", errno:", tostring(errno))
		return	false, error.err_sql, errno
	end

	return	true, result
end

function _M.tx_begin(connection)
	return mysql_query_internal("START TRANSACTION", connection)
end

function _M.tx_commit(connection)
	return mysql_query_internal("COMMIT", connection)
end

function _M.tx_rollback(connection)
	return mysql_query_internal("ROLLBACK", connection)
end

function _M.query(sql, connection)
    local ok, res, errno = mysql_query_internal(sql, connection)
    if not ok then
        return nil, res, errno
    end
        
    return res
end

function _M.execute(sql, connection)
    local ok, res, errno = mysql_query_internal(sql, connection)
    if not ok then
        return -1, res, errno
    end

    return res.affected_rows, res.insert_id
end

function _M.execute_bat(sql, connection)
    local ok, res, errno = mysql_query_internal(sql, connection)
    if not ok then
        mysql_query_internal("ROLLBACK", connection)
        return -1, res, errno
    end

    return res.affected_rows
end

return	_M
