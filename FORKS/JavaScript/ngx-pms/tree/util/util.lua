--[[
author: jie123108@163.com
date: 20150901
]]

local _M = {}

local config = require(config_name or "config")
local http = require "resty.http"   -- https://github.com/pintsized/lua-resty-http

function _M.ifnull(var, value)
    if var == nil then
        return value
    end
    return var
end


function _M.popen(cmd, filter)
    local fp = io.popen(cmd .. '; echo "retcode:$?"', "r")
    local line_reader = fp:lines()
    local lines = {}
    local lastline = nil
    for line in line_reader do
        lastline = line
        if not _M.startswith(line, "retcode:") then
            if filter == nil then
                table.insert(lines, line)
            else
                line = filter(line)
                if line ~= nil then
                    table.insert(lines, line)
                end
            end
        end
    end
    fp:close()
    if lastline == nil or string.sub(lastline, 1, 8) ~= "retcode:" then
        return false, lastline, -1
    else
        local code = tonumber(string.sub(lastline, 9))
        return code == 0, lines, code
    end
end


function _M.trim (s)
    return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end

function _M.replace(s, s1, s2)
    local str = string.gsub(s, s1, s2)
    return str
end

function _M.endswith(str,endstr)
   return endstr=='' or string.sub(str,-string.len(endstr))==endstr
end

function _M.startswith(str,startstr)
   return startstr=='' or string.sub(str,1, string.len(startstr))==startstr
end

function _M.fillspace(str, n)
    str = str or ""
    n = n or 1
    local l = string.len(str)
    if l >= n then
        return str
    else
        return str .. string.rep('', n-l)
    end
end

-- ngx.log(ngx.INFO, "config.idc_name:", config.idc_name, ", config.is_root:", config.is_root)
-- delimiter 应该是单个字符。如果是多个字符，表示以其中任意一个字符做分割。
function _M.split(s, delimiter)
    local result = {};
    for match in string.gmatch(s, "[^"..delimiter.."]+") do
        table.insert(result, match);
    end
    return result;
end

-- delim 可以是多个字符。
-- maxNb 最多分割项数
function _M.splitex(str, delim, maxNb)
    -- Eliminate bad cases...
    if delim == nil or string.find(str, delim) == nil then
        return { str }
    end
    if maxNb == nil or maxNb < 1 then
        maxNb = 0    -- No limit
    end
    local result = {}
    local pat = "(.-)" .. delim .. "()"
    local nb = 0
    local lastPos
    for part, pos in string.gmatch(str, pat) do
        nb = nb + 1
        result[nb] = part
        lastPos = pos
        if nb == maxNb then break end
    end
    -- Handle the last field
    if nb ~= maxNb then
        result[nb + 1] = string.sub(str, lastPos)
    end
    return result
end

function _M.is_ip(strip)
    if strip == nil or strip == "" then
        return false
    end
    return string.match(strip, "^%d+.%d+.%d+.%d+") ~= nil
end

local domain_suffix = {
["top"]=1,["com"]=1,["net"]=1,["org"]=1,["edu"]=1,["gov"]=1,["int"]=1,["mil"]=1,["cn"]=1,["中国"]=1,
["公司"]=1,["网络"]=1,["tel"]=1,["biz"]=1,["cc"]=1,["tv"]=1,["info"]=1,["name"]=1,["hk"]=1,["mobi"]=1,
["asia"]=1,["cd"]=1,["travel"]=1,["pro"]=1,["museum"]=1,["coop"]=1,["aero"]=1
}

function _M.is_domain(domain)
    local dot = 0
    local idx = 1
    for i=1,10 do
        local nidx = string.find(domain, '%.', idx+1)
        if not nidx then break end
        dot=dot+1
        idx = nidx
    end

    if dot < 1 then
        return false
    end

    local lastsec = string.sub(domain, idx+1)
    return string.len(lastsec) < 5 and domain_suffix[lastsec] == 1
end

function _M.headerstr(headers)
    if headers == nil or headers == {} then
        return ""
    end
    local lines = {}
    for k, v in pairs(headers) do
        if type(v) == 'table' then
            v = table.concat(v, ',')
        end
        if k ~= "User-Agent" then
            table.insert(lines, "-H'" .. k .. ": " .. v .. "'");
        end
    end
    return table.concat(lines, " ")
end

function _M.http_get(uri, myheaders, timeout)
    if myheaders == nil then myheaders = {} end

    local timeout_str = "-"
    if timeout then
        timeout_str = tostring(timeout)
    end
    ngx.log(ngx.INFO, "GET REQUEST [ ", "curl -v ", _M.headerstr(myheaders), " '", uri, "' -o /dev/null ] timeout:", timeout_str)
    local httpc = http.new()
    if timeout then
        httpc:set_timeout(timeout)
    end
    local begin = ngx.now()
    local res, err = httpc:request_uri(uri, {method = "GET", headers = myheaders})
    local cost = ngx.now()-begin
    if not res then
        ngx.log(ngx.ERR, "FAIL REQUEST [ ", "curl -v ", _M.headerstr(myheaders), 
                    " '", uri, "' -o /dev/null ] err:", err, ", cost:", cost)
    elseif res.status >= 400 then
        ngx.log(ngx.ERR, "FAIL REQUEST [ ", "curl -v ", _M.headerstr(myheaders), 
                    " '", uri, "' -o /dev/null ] status:", res.status, ", const:", cost)
    end
    return res, err
end

function _M.redirect(uri, args)
    local uri_and_args = uri 
    if args then
        uri_and_args = uri_and_args .. "?" .. args 
    end
    ngx.header['Location'] = uri_and_args
    ngx.exit(ngx.HTTP_MOVED_TEMPORARILY)
end

function _M.get_full_uri(uri)
    if _M.startswith(uri, '/') then
        local hostinfo = "http://127.0.0.1"
        if tostring(ngx.var.server_port) ~= "80" then
            hostinfo = hostinfo .. ":" .. tostring(ngx.var.server_port)
        end
        uri = hostinfo .. uri
    end
    return uri
end

function _M.tableIsNull(res)
    local ret = true
    if type(res) == "table" then
        for key, val in pairs(res) do
            if key then
                ret = false
                break
            end
        end
    end 
    return  ret
end

function _M.merge_array_unique(t1, t2)
    local t = _M.merge_array_as_map(t1,t2)
    local r = {}
    for k,_ in pairs(t) do
        table.insert(r, k)
    end
    return r
end

function _M.merge_array_as_map(t1, t2)
    local t = {}
    if t1 then
        for _,v in ipairs(t1) do
            t[v] = true
        end
    end
    if t2 then
        for _,v in ipairs(t2) do
            t[v] = true
        end
    end
    return t
end

function _M.url_in_ignore_list(url)
    if config.ignore_list == nil then
        return false
    end
    local matched = false
    -- 精确匹配。
    if type(config.ignore_list.equals)=='table' then
        for i, item in ipairs(config.ignore_list.equals) do 
            if item == url then
                matched = true
                break
            end
        end
    end

    -- 后缀匹配
    if not matched and type(config.ignore_list.suffix)=='table' then
        for i, item in ipairs(config.ignore_list.suffix) do 
            if _M.endswith(url, item) then
                matched = true
                break
            end
        end
    end

    -- 前缀匹配。
    if not matched and type(config.ignore_list.prefix)=='table' then
        for i, item in ipairs(config.ignore_list.prefix) do 
            if _M.startswith(url, item) then
                matched = true
                break
            end
        end
    end

    -- 正则匹配。
    -- TODO: 代码实现。

    return matched
end

function _M.make_pwd(password)
    local pwd_str = config.password_magic or '#*nright@0Ol1llOO'
    pwd_str = pwd_str .. password
    return ngx.md5(pwd_str)
end

local function init_pwd_tables()
    math.randomseed(os.time())
    local pwd_tables = {}
    for i=string.byte('a'),string.byte('z') do
        table.insert(pwd_tables,string.char(i))
    end
    for i=string.byte('A'),string.byte('Z') do
        table.insert(pwd_tables,string.char(i))
    end
    for i=string.byte('0'),string.byte('9') do
        table.insert(pwd_tables,string.char(i))
    end
    table.insert(pwd_tables, "!@#$%^&*")
    return pwd_tables
end

_M.pwd_tables = init_pwd_tables()

function _M.random_pwd(length)
    length = length or 10
    local t = {}
    for i = 1,length do
        local idx = math.random(#_M.pwd_tables)
        table.insert(t, _M.pwd_tables[idx])
    end
    return table.concat(t)
end

function _M.localtime(seconds, format)
    seconds = tonumber(seconds)
    format = format or "%Y-%m-%d %H:%M:%S"
    return os.date(format, seconds)
end

function _M.str_clip(str, n)
    if not str then return str end
    local len = string.len(str)
    if len <= n then
        return str
    end
    return string.sub(str, 1, n-2) .. " .."
end

function _M.permission_alt(permission)
    if not permission then return "" end
    return (string.gsub(permission, "|", "\n"))
end

return _M