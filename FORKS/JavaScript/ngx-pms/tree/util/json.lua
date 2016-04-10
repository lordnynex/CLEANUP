local cjson = require "cjson"

local _M = {}

function _M.loads(str)
    local ok, jso = pcall(function() return cjson.decode(str) end)
    if ok then
        return jso
    else
        return nil, jso
    end
end

function _M.dumps(tab)
	if tab and type(tab) == 'table' then
		return cjson.encode(tab)
	else
		return tostring(tab)
	end
end

return _M