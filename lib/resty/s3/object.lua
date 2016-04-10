local _M = {}                 
local mt = { __index = _M }
    
function _M:new(db, bucket_name)
    return setmetatable({
        bucket = db:get_gridfs(bucket_name)}, mt)                            
end 

function _M:get(object_name)
    local f = self.bucket:find_one({["filename"]=object_name})
    if not f then return nil end
    return f:read()
end
    
function _M:put(object_name)                    
    f, err = self.bucket:new({filename=object_name})
    if not f then return nil, err end
    return f
end 

function _M:list()
    local c = self.bucket.file_col:find({}, {_id=0})
    local ret = {}
    for idx, item in c:pairs() do
        ret[idx] = item
    end
    return ret
end 

function _M:delete(object_name)                 
    return self.bucket:remove({["filename"]=object_name}, 0, 1) 
end

function _M:head(object_name)
    return self.bucket.file_col:find_one({filename=object_name}, {_id=0})
end

return _M
