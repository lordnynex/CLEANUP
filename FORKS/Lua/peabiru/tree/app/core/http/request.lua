--
-- Request "class"
--
-- @author    xico@simbio.se
-- @copyright Simbiose
-- @license   LGPL version 3.0, see LICENSE
--

-- load modules, wipe unnecessary methods

local string, utils = require 'string', require 'utils'

local lower, fresh, class, status = string.lower, utils.fresh, utils.class, utils.status

string, utils = nil, nil

-- Request "class".

local Request = class()
Request.__name = 'Request'


function Request:__init ()
  --self.content_length = ngx.var.content_length
  --self.content_type   = content_type()
  self.method         = ngx.var.request_method
  self.headers        = ngx.req.get_headers()
  self.hostname       = ngx.var.hostname
  --self.params = params()
  self.path_info      = ngx.var.uri
  self.query_string   = ngx.var.query_string or ''
  self.path           = self.path_info .. '?' .. self.query_string
  self.schema         = ngx.var.schema
  --self.is_ssl         = self.schema == 'https'
  self.host           = ngx.var.host
  self.port           = ngx.var.server_port
  --self.host_and_port  = self.host .. ":" .. self.port
  --self.user_agent = ngx.var.http_user_agent
  --self.is_xhr = ngx.var.http_x_requested_with == "XMLHttpRequest"
  --self.referer = ngx.var.http_referer


  ngx.req.read_body()
  self.body   = ngx.req.get_post_args()
  self.query  = ngx.req.get_uri_args()
  self.params = {}
end

-- Return request header.
--
-- The `Referrer` header field is special-cased,
-- both `Referrer` and `Referer` are interchangeable.
--
-- Examples:
--
--     req:header('Content-Type')
--     >> "text/plain"
--
--     req:header 'content-type'
--     >> "text/plain"
--
--     req:header 'Something'
--     >> nil
--
-- @string name
-- @return string
-- @api    public

function Request:header (name)
  name = lower(name)
  if name == 'referer' or name == 'referrer' then
    return self.headers.referrer or self.headers.referer
  end
  return self.headers[name]
end

-- Return the value of param `name` when present or `default`.
--
-- - Checks route placeholders, ex: _/user/:id_
-- - Checks body params, ex: id=12, {"id":12}
-- - Checks query string params, ex: ?id=12
--
-- To utilize request bodies, `req.body`
-- should be an object. This can be done by using
-- the `body_parser()` middleware.
--
-- @string name
-- @mixed  [default]
-- @return string
-- @api public

function Request:param (name, default)
  if self.params and self.params[name] then
    return self.params[name]
  elseif self.body and self.body[name] then
    return self.body[name]
  elseif self.query and self.query[name] then
    return self.query[name]
  else
    return default
  end
end

-- Check if the request is fresh, aka
-- Last-Modified and/or the ETag
-- still match.
--
-- @return boolean
-- @api    public

Request:__getter('fresh', function (self)
  local method, code = self.method, self.res.status

  -- GET or HEAD for weak freshness validation only
  if 'GET' ~= method and 'HEAD' ~= method then
    return false
  end

  -- 2xx or 304 as per rfc2616 14.26
  if (code > 199 and code < 300) or 304 == code then
    return fresh(self.headers, self.res._headers)
  end

  return false
end)

-- Check if the request is stale, aka
-- "Last-Modified" and / or the "ETag" for the
-- resource has changed.
--
-- @return boolean
-- @api    public

Request:__getter('stale', function (self)
  return not self.fresh
end)

-- check security
--
-- @return boolean
-- @api    public

Request:__getter('secure', function (self)
  return 'https' == self.schema
end)

-- Check if the request was an _XMLHttpRequest_.
--
-- @return boolean
-- @api    public

Request:__getter('xhr', function (self)
  return 'xmlhttprequest' == lower(self:header('X-Requested-With') or '')
end)

return Request