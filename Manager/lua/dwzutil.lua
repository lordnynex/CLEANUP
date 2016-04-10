
local json = require("util.json")

local _M = {}

function _M.cons_resp(status, msg, args)
  args = args or {}
  local callbackType = args.callbackType or ""
  local navTabId = args.navTabId or ""
  local rel = args.rel or ""
  local forwardUrl = args.forwardUrl or ""
  local warn = args.warn
  local rsp = {
      statusCode=tostring(status), 
      message=tostring(msg), 
      callbackType=callbackType,
      navTabId=navTabId,
      rel=rel,
      warn=warn,
      forwardUrl=forwardUrl}
    
    return json.dumps(rsp)
end

return _M