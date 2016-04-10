local mongol = require "resty.mongol"
local cjson = require "cjson"
local uuid = require "resty.uuid"
local md5 = require "resty.md5"
local string = require "resty.string"

local db_host = ngx.var.aksk_manager_host or "127.0.0.1"
local db_port = ngx.var.aksk_manager_port or 27017
local db_name = "aksk"
local col_name = "manager"


local conn = mongol:new()
local ok, err = conn:connect(db_host, db_port)
if not ok then
    ngx.log(ngx.ERR, "failed to connect db: ", err)
    ngx.exit(500)
end

local db = conn:new_db_handle(db_name)
local col = db:get_col(col_name)
local method = ngx.var.request_method

if method == "GET" then
    local args = ngx.req.get_uri_args()
    if not args then
        ngx.exit(404)
    else
        local r = col:find_one({ak=args["ak"]}, {sk=1})
        if not r then
            ngx.exit(404)
        else
            ngx.say(cjson.encode({sk=r["sk"]}))
        end
    end
end

if method == "DELETE" then
    local args = ngx.req.get_uri_args()
    if args then
        local r, err = col:delete({ak=args["ak"]})
        if not r then
            ngx.log(ngx.ERR, "failed to delete aksk", err)
        end
    end
end

if method == "POST" or method == "PUT" then
    ngx.req.read_body()
    local args = ngx.req.get_post_args()
    if not args then
        ngx.exit(400)
    else
        for k, v in pairs(args) do
            local m = ngx.re.match(k, "{\"sso_token\":\"(.*)\"}")
            if m then
                local token = m[1]
                local _md5 = md5:new()
                _md5:update(token)
                local ak = string.to_hex(_md5:final())
                local sk = uuid:generate()
                local data = {}
                data["$set"] = {sk=sk}
                local r, err = col:update({ak=ak}, data, 1, 0, true)
                if not r then
                    ngx.log(ngx.ERR, "failed to gen aksk", err)
                    ngx.exit(500)
                else
                    ngx.say(cjson.encode({ak=ak, sk=sk}))
                end
                break
            end
        end
    end
end
