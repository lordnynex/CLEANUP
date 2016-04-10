local mod_name = "resty.s3."

local mongol = require "resty.mongol"
local lfs = require "lfs"
local handler = require (mod_name.."handler")
local cachel = require (mod_name.."cachel")

local resource = ngx.var.resource or "misc"

local db_addrs = ngx.var.db_addrs or "127.0.0.1:27017"
local db_name = ngx.var.db_name or resource
local db_user = ngx.var.db_user or ""
local db_passwd = ngx.var.db_passwd or ""

local cache_dir = ngx.var.cache_dir or "/tmp/nginx_cache/"

local chunk_size = ngx.var.chunk_size or 204800
local read_timeout = ngx.var.read_timeout or 600000

local with_sign = ngx.var.with_sign or false

local rename_object = ngx.var.rename_object or ""
local thumbnail = ngx.var.thumbnail or "" 
local noise = ngx.var.noise or "" 
local noise_pwd = ngx.var.noise_pwd or "*" 

local forbidden = tonumber(ngx.var.forbidden) or 0

local main_domain = "http://*.com/"

local db_list = {}
local db_host, db_port = "", 0
local db_addrs_len = string.len(db_addrs)
local pos_start = 1
while pos_start < db_addrs_len do
    local pos_end = string.find(db_addrs, ",", pos_start)
    if not pos_end then
        pos_end = string.len(db_addrs)+1
    end
    local addr = string.sub(db_addrs, pos_start, pos_end-1)
    table.insert(db_list, addr)
    pos_start = pos_end + 1 	
    
    if db_host == "" then
        local m = ngx.re.match(addr, "(.*):(.*)")
        if m then
            db_host, db_port = m[1], m[2]
        end 
    end
end

local base_conn = mongol:new()
local ok, err = base_conn:connect(db_host, tonumber(db_port))
if not ok then
    ngx.log(ngx.ERR, "failed to connect db: ", err)
    ngx.exit(500)
end

local conn = base_conn:getprimary(db_list)
local ok, err = conn:new_db_handle("admin"):auth(db_user, db_passwd)
if not ok then
    ngx.log(ngx.ERR, "failed to auth db: ", err)
    ngx.exit(500)
end

local db = conn:new_db_handle(db_name)

local r, err = lfs.attributes(cache_dir, "mode")
if not r or r ~= "directory" then
    local ok, err = lfs.mkdir(cache_dir)
    if not ok then
        ngx.log(ngx.ERR, "failed to init cache dir: ", err)
    end
end

local cache = cachel:new(cache_dir)

local pattern = "/"..resource.."(.*)"
if resource == "misc" then pattern = "(.*)" end
local m = ngx.re.match(ngx.var.uri, pattern)
local path = nil
local bucket_name = nil
local object_name = nil
if m then path = m[1] end

if path and path ~= "/" and path ~= "" then
    local m, err = ngx.re.match(path, "/(?<obj_bct>.*?)/(?<obj>.*)|/(?<bct>.*)")
    if m then
        if m["bct"] or (m["obj_bct"] and (not m["obj"] or m["obj"] == "")) then
            bucket_name = m["bct"] or m["obj_bct"]
        else
            bucket_name, object_name = m["obj_bct"], m["obj"]
        end
    else
        ngx.log(ngx.ERR, "invalid uri: ", err)
        ngx.exit(404)
    end
end

local _h = handler:new({
    main_domain = main_domain,
    document_uri = ngx.var.document_uri,
    uri_args = ngx.req.get_uri_args(),
    ngx_say = ngx.say,
    ngx_print = ngx.print,
    ngx_exit = ngx.exit,
    ngx_log = ngx.log,
    ngx_err_flag = ngx.ERR,
    ngx_re_match = ngx.re.match,
    db = db,
    cache = cache,
    chunk_size = chunk_size,
    read_timeout = read_timeout,
    rename_object = rename_object,
    thumbnail = thumbnail,
    noise = noise,
    noise_pwd = noise_pwd,
})


if with_sign and not _h:check_sign() then
    ngx.log(ngx.ERR, "failed to check sign")
    ngx.exit(401)
end

local method = ngx.var.request_method
if method == "GET" then
    if not bucket_name then
        if forbidden >= 1 then
            ngx.exit(405)
        end
        _h:list_bucket()
    elseif bucket_name and object_name then
        _h:get_object(bucket_name, object_name)
    else
        if forbidden >= 1 then
            ngx.exit(405)
        end
        _h:list_object(bucket_name)
    end
end

if method == "POST" then
    if bucket_name and object_name then
        _h:put_object(bucket_name, object_name)
    else
        ngx.exit(405)
    end
end

if method == "PUT" then
    if bucket_name and not object_name then
        _h:put_bucket(bucket_name)
    else
        ngx.exit(405)
    end
end

if method == "DELETE" then
    if bucket_name and object_name then
        _h:delete_object(bucket_name, object_name)
    elseif bucket_name then
        _h:delete_bucket(bucket_name)
    else
        ngx.exit(405)
    end
end

if method == "HEAD" then
    if bucket_name and object_name then
        ngx.header["Content-Length"] = _h:head_object(bucket_name, object_name)
    end
end
