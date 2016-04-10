local _M = {}                 
local mt = { __index = _M}                                   
    
function _M:new(db)                        
    return setmetatable({
        db = db,              
        acl = nil}, mt)                            
end 

function _M:put(bucket_name)                    
    local r
    local err 
    r, err = self.db:create_col(bucket_name..".files")
    if not r then return nil, err end
    r, err = self.db:create_col(bucket_name..".chunks")
    if not r then return nil, err end
    return 1
end 

function _M:list()
    local c = self.db:list()
    local ret = {}
    for idx, item in c:pairs() do
        if item["name"] == "_id_" then
            local ns = item["ns"]
            local m = ns:match(".+%.(.+)%.files")
            if m then table.insert(ret, m) end
        end
    end
    return ret
end 

function _M:delete(bucket_name)                 
    return self.db:get_gridfs(bucket_name):drop()
end

return _M
