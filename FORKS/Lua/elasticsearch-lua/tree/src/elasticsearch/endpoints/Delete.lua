-------------------------------------------------------------------------------
-- Importing modules
-------------------------------------------------------------------------------
local Endpoint = require "elasticsearch.endpoints.Endpoint"

-------------------------------------------------------------------------------
-- Declaring module
-------------------------------------------------------------------------------
local Delete = Endpoint:new()

-------------------------------------------------------------------------------
-- Declaring Instance variables
-------------------------------------------------------------------------------

-- The parameters that are allowed to be used in params
Delete.allowedParams = {
  "consistency",
  "parent",
  "refresh",
  "replication",
  "routing",
  "timeout",
  "version",
  "version_type",
}

-------------------------------------------------------------------------------
-- Function to calculate the http request method
--
-- @return    string    The HTTP request method
-------------------------------------------------------------------------------
function Delete:getMethod()
  return "DELETE"
end

-------------------------------------------------------------------------------
-- Function to calculate the URI
--
-- @return    string    The URI
-------------------------------------------------------------------------------
function Delete:getUri()
  if self.id == nil then
    return nil, "id not specified for Delete"
  end
  if self.index == nil then
    return nil, "index not specified for Delete"
  end
  if self.type == nil then
    return nil, "type not specified for Delete"
  end
  return "/" .. self.index .. "/" .. self.type .. "/" .. self.id
end

-------------------------------------------------------------------------------
-- Returns an instance of Delete class
-------------------------------------------------------------------------------
function Delete:new(o)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  return o
end

return Delete
