local mod_name = "resty.s3."

local cjson = require "cjson"
local upload = require "resty.upload"
local uuid = require "resty.uuid"
local md5 = require "resty.md5"
local bucket = require (mod_name.."bucket")
local object = require (mod_name.."object")
local image = require(mod_name.."image")

_M = {}                 
mt = { __index = _M}                                   


function _M:new(ctx)
    return setmetatable({
        main_domain = ctx["main_domain"],
        document_uri = ctx["document_uri"],
        uri_args = ctx["uri_args"],
        ngx_say = ctx["ngx_say"],
        ngx_print = ctx["ngx_print"],
        ngx_exit = ctx["ngx_exit"],
        ngx_log = ctx["ngx_log"],
        ngx_err_flag = ctx["ngx_err_flag"],
        ngx_re_match = ctx["ngx_re_match"],
        db = ctx["db"],
        cache = ctx["cache"],
        chunk_size = ctx["chunk_size"],
        read_timeout = ctx["read_timeout"],
        rename_object = ctx["rename_object"],
        thumbnail = ctx["thumbnail"],
        noise = ctx["noise"],
        noise_pwd = ctx["noise_pwd"],
    }, mt)                            
end 


function _M:check_sign()
    local raw_sign = self.uri_args["sign"]
    if not raw_sign then
        return false
    else
        local ak, sign = "", ""
        local m = self.ngx_re_match(raw_sign, "(?<ak>.*):(?<sign>.*)")
        if m then
            ak, sign = m["ak"], m["sign"]
        end
        local t = md5:new()
        if not t then return false end
        t:update(self.document_uri..ak)
        local _sign = rstring.to_hex(t:final())
        self.ngx_log(self.ngx_err_flag, self.document_uri, "@", _sign, "@", sign)
        if _sign ~= sign then
            return false
        else
            return true
        end
    end
end

function _M:list_bucket()
    local b = bucket:new(self.db)
    self.ngx_say(cjson.encode(b:list()))
end

function _M:list_object(bucket_name)
    local o = object:new(self.db, bucket_name)
    self.ngx_say(cjson.encode(o:list()))
end

function _M:get_object(bucket_name, object_name)
     
    local o = object:new(self.db, bucket_name)
    local r, err = o:get(object_name)
    if not r then
        self.ngx_log(self.ngx_err_flag, "failed to get object: ", err)
        self.ngx_exit(404)
        return
    end
    
    ngx.header["Content-Length"] = string.len(r)
    self.ngx_print(r)
end

function _M:delete_bucket(bucket_name)
    local b = bucket:new(self.db)
    local r, err = b:delete(bucket_name)
    if not r then
        self.ngx_log(self.ngx_err_flag, "failed to delete bucket: ", err)
        self.ngx_exit(500)
        return
    end
end

function _M:delete_object(bucket_name, object_name)
    local o = object:new(self.db, bucket_name)
    local r, err = o:delete(object_name)
    if not r then
        self.ngx_log(self.ngx_err_flag, "failed to delete bucket: ", err)
        self.ngx_exit(500)
        return
    end
    local key = self.cache:gen_key(bucket_name..object_name) 
    if not key then
        self.ngx_log(self.ngx_err_flag, "failed to generate cache key")
    else
        local r, err = self.cache:delete(key)
        if not r then
            self.ngx_log(self.ngx_err_flag, "failed to delete cache: ", err)
        end
    end
end

function _M:put_bucket(bucket_name)
    local b = bucket:new(self.db)
    local r, err = b:put(bucket_name)
    if not r then
        self.ngx_log(self.ngx_err_flag, "failed to put bucket: ", err)
        self.ngx_exit(500)
        return
    end
end

local function put_object_with_opts(self, form, obj, name, opts)
    local thumb = opts["thumb"]
    local noise = opts["noise"]
    local noise_pwd = opts["noise_pwd"]
    local fname = name
    local offset = 0
    local f, err
    local blob = ""
    
    while true do
        local typ, res, err = form:read()
        if not typ then
            self.ngx_log(self.ngx_err_flag, "failed to read: ", err)
            self.ngx_exit(500)
            return
        end
        if typ == "header" then
            if res[1] ~= "Content-Type" then
                if self.rename_object == "uuid" then
                    fname = uuid.generate()
                end
                m = ngx.re.match(res[2],'(.*)filename="(.*?)\\.(.*)"(.*)')
                if m and self.rename_object == "uuid" then
                    fname = fname.."."..m[3]
                end
                f = obj.bucket:find_one({["filename"]=fname})
                if f then
                    f["_md5"] = md5:new()
                    f["file_size"] = 0
                    f["file_md5"] = 0
                else 
                    f, err = obj:put(fname)
                    if not f then
                        self.ngx_log(self.ngx_err_flag, "failed to put object: ", err)
                        self.ngx_exit(500)
                        return
                    end
                end 
            end
        end
        if typ == "body" then
            blob = blob..res	
        end
        if typ == "eof" then
            break
        end
    end

    if noise == "1" then
        local r, err = image.noise(blob, noise_pwd)
        if err ~= nil then
            self.ngx_log(self.ngx_err_flag, "failed to noise image: ", err)
            self.ngx_exit(500)
            return
        end
        blob = r
    end
    f:write(blob, 0)
    f:update_md5()

    if thumb ~= "" then
        local r, err = image.thumb(blob, thumb)
        if not r then
            self.ngx_say(cjson.encode({errno=20001, error="failed to generate thumbnail: "..err}))
            self.ngx_exit(200)
            return
        end
       
        f, err = obj:put("thumbnail_"..fname)
        if not f then
            self.ngx_log(self.ngx_err_flag, "failed to put object: ", err)
            self.ngx_exit(500)
            return
        end
        if noise == "1" then
            local ni, err = image.noise(blob, noise_pwd)
            if err ~= nil then
                self.ngx_log(self.ngx_err_flag, "failed to noise image: ", err)
                self.ngx_exit(500)
                return
            end
            r = ni
        end
        f:write(r, 0)
        f:update_md5()
    end

    return fname
end

local function delete_cache(self, key_str)
    local key = self.cache:gen_key(key_str)
    if not key then
        self.ngx_log(self.ngx_err_flag, "failed to generate cache key")
    else
        local r, err = self.cache:delete(key)
        if not r then
            self.ngx_log(self.ngx_err_flag, "failed to delete cache: ", err)
        end
    end
end

function _M:put_object(bucket_name, object_name)
    local form, err = upload:new(self.chunk_size)
    if not form then
        self.ngx_log(self.ngx_err_flag, "failed to new upload: ", err)
        self.ngx_exit(500)
        return
    end
    form:set_timeout(self.read_timeout)

    local o = object:new(self.db, bucket_name)
    local fname = object_name

    local opts = {thumb = self.thumbnail, noise = self.noise, noise_pwd = self.noise_pwd}
    fname = put_object_with_opts(self, form, o, fname, opts)

    delete_cache(self, bucket_name..fname)
    if self.thumbnial ~= "" then
        delete_cache(self, bucket_name.."thumbnail_"..fname)
    end
       
    if self.rename_object ~= "" then
        if self.thumbnail ~= "" then
            self.ngx_say(cjson.encode({errno=10000, data={url=self.main_domain..bucket_name.."/"..fname, thumb=self.main_domain..bucket_name.."/thumbnail_"..fname}}))
        else
            self.ngx_say(cjson.encode({errno=10000, data={url=self.main_domain..bucket_name.."/"..fname}}))
        end
    end
end

function _M:head_object(bucket_name, object_name)
    local o = object:new(self.db, bucket_name)
    local r = o:head(object_name)
    if not r then
        return 0
    else
        return r["length"]
    end
end

return _M
