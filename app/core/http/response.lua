--
-- Response "class"
--
-- @author    xico@simbio.se
-- @copyright Simbiose
-- @license   LGPL version 3.0, see LICENSE
--

-- module dependencies, wipe unnecessary methods

local string, utils = require 'string', require 'utils'

local find, mime, statuses = string.find, utils.mime, utils.statuses

utils, string = nil, nil

-- find, format,    concat, insert,    type

-- Response "class".

local Response = class()
Response.__name = 'Response'

-- Set status `code`.
--
-- @number code
-- @return Response
-- @api    public

function Response:status (code)
  self.status = code
  return self
end

-- Send a response.
--
-- Examples:
--
--     res:send({ some: 'json' })
--     res:send('<p>some html</p>')
--     res:send(404, 'Sorry, cant find that')
--     res:send(404)
--
-- @api public

function Response:send (...)
  local req, app, status, body = self.req, self.app, ...

  if 2 == #{...} then
    if 'number' ~= type(status) and 'number' == type(body) then
      self.status, body = body, status
    else
      self.status = status
    end
  end

  local head, _type, encoding, length = 'HEAD' == req.method, type(body)

  if 'number' == _type then
    self.status, body = body, statuses[body]
  elseif 'string' == _type then
    if not self:get('Content-Type') then
      self:type('html')
    end
  elseif 'table' == _type then
    body = self:json(body)
  else
    body = ''
  end

  length = #body

  self:set('Content-Type', set_charset(self:get('Content-Type'), 'utf-8'))
  self:set('Content-Length', #body)

  local etag = length ~= nil and app:get('etag fn')
  if etag and ('GET' == req.method or 'HEAD' == req.method) then
    self:set('ETag', etag(body))
  end

  if req.fresh then
    self.status = 304
  end

  -- strip irrelevant headers
  if 204 == self.status or 304 == self.status then
    self:remove_header('Content-Type')
    self:remove_header('Content-Length')
    self:remove_header('Transfer-Encoding')
    body = ''
  end

  self:finish(head and nil or body)

  return self
end

-- Send JSON response.
--
-- Examples:
--
--     res:json(null)
--     res:json { user = 'roberto' }
--     res:json(500, 'oh noes!')
--     res:json(404, 'I dont have that')
--
-- @api public

function Response:json (...)
  if not self:get('Content-Type') then
    self:set('Content-Type', 'application/json')
  end
  self:send(...)
end

-- Set _Content-Type_ response header with `type` through `mime.lookup()`
-- when it does not contain "/", or set the Content-Type to `type` otherwise.
--
-- Examples:
--
--     res:type('.html')
--     res:type('html')
--     res:type('json')
--     res:type('application/json')
--     res:type('png')
--
-- @string _type
-- @return Response for chaining
-- @api    public

function Response:type (_type)
  return self:set('Content-Type', (find(_type, '/', 1, true) > 1 and _type or mime[_type]))
end

-- Set header `field` to `val`, or pass
-- an object of header fields.
--
-- Examples:
--
--    res.set('Foo', ['bar', 'baz']);
--    res.set('Accept', 'application/json');
--    res.set({ Accept: 'text/plain', 'X-API-Key': 'tobi' });
--
-- Aliased as `res.header()`.
--
-- @mixed  field
-- @string val
-- @return Response for chaining
-- @api    public

local function _header (self, field, val)
  if field and val then
    self:set_header(key, val)
  else
    for key in pairs(field) do
      self:set(key, field[key])
    end
  end
  return self
end

Response.set    = _header
Response.header = _header

-- Get value for header `field`.
--
-- @string field
-- @return string
-- @api    public

function Response:get (field)
  return self:get_header(field)
end

-- Set the location header to `url`.
--
-- The given `url` can also be "back", which redirects
-- to the _Referrer_ or _Referer_ headers or "/".
--
-- Examples:
--
--    res.location('/foo/bar').;
--    res.location('http://example.com');
--    res.location('../login');
--
-- @string url
-- @api    public

function Response:location (url)
  self:set('back' == url and self.req:get('Referrer') or '/')
  return self
end

-- Redirect to the given `url` with optional response `status`
-- defaulting to 302.
--
-- The resulting `url` is determined by `res.location()`, so
-- it will play nicely with mounted apps, relative paths,
-- `"back"` etc.
--
-- Examples:
--
--    res.redirect('/foo/bar');
--    res.redirect('http://example.com');
--    res.redirect(301, 'http://example.com');
--    res.redirect('http://example.com', 301);
--    res.redirect('../login'); // /blog/post/1 -> /blog/login
--
-- @string url
-- @number status
-- @api    public

function Response:redirect (...)
  local head, status, body, url, status = 'HEAD' == self.req.method, 302, '', ...

  if 2 == #{...} and 'number' == type(url) then
    status, url = ...
  end

  self:location(url)
  self.status = status
  self:set('Content-Length', 0)
  self:finish('')
end

return Response