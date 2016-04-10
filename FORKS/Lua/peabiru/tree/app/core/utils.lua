--
-- utils
--
-- @author    xico@simbio.se
-- @copyright Simbiose
-- @license   LGPL version 3.0, see LICENSE
--

-- Module dependencies.

local io, os, string, table =
  require 'io', require 'os', require 'string', require 'table'

local crc32, base64, md5
if ngx and ngx.encode_base64 then
  crc32, base64, md5 = ngx.crc32_long, ngx.encode_base64, ngx.md5_bin
else
  local ok, lcrypt = pcall(require, 'lcrypt')
  if ok then
    crc32, base64, md5 = lcrypt.crc32, lcrypt.base64_encode, function (value)
      return lcrypt.hash(lcrypt.hashes.md5, lcrypt.hash_modes.hash, value):done()
    end
  else
    error('missing dependency lcrypt')
  end
  lcrypt = nil
end

local utils, setmetatable, getmetatable, type, pairs, rawset, rawget, stdout, concat, insert,
  find, format, gsub, match, gmatch, len, byte = 
  {}, setmetatable, getmetatable, type, pairs, rawset, rawget, io.stdout, table.concat, table.insert,
  string.find, string.format, string.gsub, string.match, string.gmatch, string.len, string.byte

io, os, string, table = nil, nil, nil, nil

-- Simple detection of charset parameter in content-type
local charset_reg_exp, months = ';%s*charset%s*=',
  {Jan=1, Feb=2, Mar=3, Apr=4, May=5, Jun=6, Jul=7, Ago=8, Set=9, Oct=10, Nov=11, Dec=12}

utils.mime = {
  ai = 'application/postscript', asc = 'application/pgp-signature', atom = 'application/atom+xml',
  bin = 'application/octet-stream', bz2 = 'application/x-bzip2', deb = 'application/x-deb',
  doc = 'application/msword', eps = 'application/postscript', gz = 'application/x-gzip',
  iso = 'application/octet-stream', jar = 'application/java-archive', js = 'application/javascript',
  json = 'application/json', latex = 'application/x-latex', man = 'application/x-troff-man',
  oda = 'application/oda', odt = 'application/vnd.oasis.opendocument.text',
  odp = 'application/vnd.oasis.opendocument.presentation', pdf = 'application/pdf',
  ods = 'application/vnd.oasis.opendocument.spreadsheet', ps = 'application/postscript',
  odg = 'application/vnd.oasis.opendocument.graphics', torrent = 'application/x-bittorrent',
  pgp = 'application/pgp-encrypted', ppt = 'application/vnd.ms-powerpoint',
  rar = 'application/x-rar-compressed', rss = 'application/rss+xml', tar = 'application/x-tar',
  swf = 'application/x-shockwave-flash', xhtml = 'application/xhtml+xml', ogv = 'video/ogg',
  xls = 'application/vnd.ms-excel', xsl = 'application/xml', zip = 'application/zip',
  flac = 'audio/x-flac', mid = 'audio/midi', mp3 = 'audio/mpeg', m3u = 'audio/x-mpegurl',
  oga = 'audio/ogg', ogg = 'audio/ogg', wav = 'audio/x-wav', bmp = 'image/x-ms-bmp', 
  gif = 'image/gif', ico = 'image/vnd.microsoft.icon', jpe = 'image/jpeg', jpeg = 'image/jpeg', 
  jpg = 'image/jpeg',  pbm = 'image/x-portable-bitmap', pgm = 'image/x-portable-graymap', 
  png = 'image/png', svg = 'image/svg+xml', svgz = 'image/svg+xml', tif = 'image/tiff', 
  tiff = 'image/tiff', conf = 'text/plain', cpp = 'text/x-c', css = 'text/css', csv = 'text/csv', 
  diff = 'text/x-diff', htm = 'text/html', html = 'text/html', manifest = 'text/cache-manifest', 
  md = 'text/x-markdown', rockspec = 'text/x-lua', sgm = 'text/x-sgml', sgml = 'text/x-sgml', 
  text = 'text/plain', tsv = 'text/tab-separated-values', txt = 'text/plain', xml = 'text/xml', 
  yaml = 'text/yaml', yml = 'text/yml', avi = 'video/x-msvideo', flv = 'video/x-flv',
  m1v = 'video/mpeg', mov = 'video/quicktime', mp4 = 'video/mp4', mpg = 'video/mpeg'
}

utils.statuses = {
  [100]='Continue', [101]='Switching Protocols', [200]='OK', [201]='Created', [202]='Accepted',
  [203]='Non-Authoritative Information', [204]='No Content', [205]='Reset Content',
  [206]='Partial Content', [300]='Multiple Choices', [301]='Moved Permanently', [302]='Found',
  [303]='See Other', [304]='Not Modified', [400]='Bad Request', [401]='Unauthorized',
  [402]='Payment Required', [403]='Forbidden', [404]='Not Found', [405]='Method Not Allowed',
  [406]='Not Acceptable', [408]='Request Timeout', [409]='Conflict', [410]='Gone',
  [415]='Unsupported Media Type', [416]='Range Not Satisfiable', [500]='Internal Server Error',
  [501]='Not Implemented', [502]='Bad Gateway', [503]='Service Unavailable',
  [504]='Gateway Timeout', [505]='HTTP Version Not Supported'
}

-- utc to time
--
-- @string utc
-- @return table

local function utc2time(utc)
  local t = {}
  t.day, t.month, t.year, t.hour, t.min, t.sec = match(utc, '(%d+)%s(%w+)%s(%d+)%s(%d+):(%d+):(%d+)')
  t.month = months[t.month]
  return os.time(t)
end

-- basic class manager with support to mixin, single inheritance, getters and setters
do
  -- hold all "classes" here
  local classes = setmetatable({}, {__mode='k'})

  -- deep copy tables "clone"
  --
  -- @table  t
  -- @table  dest
  -- @return table
  -- @api    private

  local function deep_copy (t, dest)
    local t, r = t or {}, dest or {}
    for k, v in pairs(t) do
      r[k] = (type(v) == 'table' and k ~= '__index') and deep_copy(v) or v
    end
    return r
  end

  -- check instance of metatable
  --
  -- @metatable self
  -- @table     klass
  -- @return    boolean
  -- @api       private

  local function is (self, klass)
    local m = getmetatable(self)
    while m do 
      if m == klass then return true end
      m = m.super
    end
    return false
  end

  -- include new methods "mixin"
  --
  -- @metatable self
  -- @table     ...
  -- @api       private

  local function include (self, ...)
    if not ... then return end
    for k,v in pairs(...) do
      if self[k] then self[k] = nil end
      self[k] = v
    end
  end

  -- extends current "class" to another "inheritance"
  --
  -- @metatable self
  -- @table     ...
  -- @return    metatable
  -- @api       private

  local function extends (self, ...)
    local meta = {}
    if ... then deep_copy(..., deep_copy(self, meta)) else deep_copy(self, meta) end
    setmetatable(meta, getmetatable(self))
    return meta
  end

  -- propagates new methods accross child "classes"
  --
  -- @metatable self
  -- @string    key
  -- @mixed     value
  -- @api       private

  local function new_index (self, key, value)
    rawset(self, key, value)
    for i=1, #classes[self] do
      classes[self][i][key] = value
    end
  end

  -- create a new "class", with inherited base (optional)
  --
  -- @table  base
  -- @return metatable

  function utils.class (base)
    local c, mt, getters, setters = {}, {}, {}, {}
    if type(base) == 'table' then
      for i,v in pairs(base) do c[i] = v end
      c.super = base
    end

    c.is         = is
    c.extends    = extends
    c.include    = include
    c.__getter   = function(self, name, fn) getters[name] = fn end
    c.__setter   = function(self, name, fn) setters[name] = fn end
    c.__newindex = function(self, key, val)
      if setters[key] then return setters[key](self, val) end
      rawset(self, key, val)
    end
    c.__index     = function(self, key, ...)
      return getters[key] and getters[key](self) or rawget(c, key)
    end
    mt.__newindex = new_index
    classes[c]    = {}

    mt.__call = function(self, ...)
      local obj = {}
      setmetatable(obj, c)
      if c.__init then c.__init(obj, ...) end
      return obj
    end

    if c.super then
      insert(classes[c.super], c)
    end

    setmetatable(c, mt)
    return c
  end
end

-- Return strong ETag for `body`.
--
-- @string body
-- @string [encoding]
-- @return string
-- @api    private

function utils.etag (body, encoding)
  -- fast-path empty body
  if len(body) == 0 then return '"1B2M2Y8AsgTpgAmY7PhCfg=="' end
  --
  return concat {'"', base64(md5(body)), '"'}
end

-- Return weak ETag for `body`.
--
-- @string body
-- @string [encoding]
-- @return string
-- @api    private

function utils.weak_etag (body, encoding)
  -- fast-path empty body
  if len(body) == 0 then return 'W/"0-0"' end
  --
  return concat {'W/"', format('%x', len(body)), '-', crc32(body), '"'}
end

-- Check if `path` looks absolute.
--
-- @string path
-- @return boolean
-- @api    private

function utils.is_absolute (path)
  if 47 == byte(path, 1) then return true end -- '/'
  if 58 == byte(path, 2) and 92 == byte(path, 3) then return true end -- ':' && '\\'
  if 92 == byte(path, 1) and 92 == byte(path, 2) then return true end -- '\\\\'
end

-- Flatten the given `arr`.
--
-- @table  arr
-- @return table
-- @api    private

function utils.flatten (arr, ret)
  local ret, length = ret or {}, #arr
  for i = 1, length do
    if 'table' == type(arr[i]) then
      utils.flatten(arr[i], ret)
    else
      insert(ret, arr[i])
    end
  end
  return ret
end


-- Normalize the given `_type`, for example "html" becomes "text/html".
--
-- @string _type
-- @return table
-- @api    private

local function normalize_type (_type)
  return find(_type, '/', 1, true) and 
    {value=utils.mime[_type]), params={}} or accept_params(_type)
end

-- Normalize `types`, for example "html" becomes "text/html".
--
-- @table  types
-- @return table
-- @api    private

local function normalize_types (types)
  local ret = {}
  for i=0, #types do
    insert(ret, normalize_type(types[i]))
  end
  return ret  
end

-- Generate Content-Disposition header appropriate for the filename.
-- non-ascii filenames are urlencoded and a filename* parameter is added
--
-- @string filename
-- @return string
-- @api    private

local function content_disposition (filename)
  if filename then
    filename = basename(filename)
    if match("[\040-\176]", filename) then
      return concat {
        'attachment; filename=', encode_uri(filename),
        "; filename*=UTF-8''", encode_uri(filename)
      }
    else
      return concat {'attachment; filename="', filename, '"'}
    end
  end
  return 'attachment'
end

-- Parse accept params `str` returning an
-- object with `.value`, `.quality` and `.params`.
-- also includes `.originalIndex` for stable sorting
--
-- @string str
-- @return table
-- @api    private

local function accept_params (str, index)
  local ret, p1, p2 = {value=nil, quality=1, params={}, original_index=index}
  
  for part in gmatch(str, '%s*([^;$]+)%s*') do
    if not ret.value then
      ret.value = part
    end
    p1, p2 = match(part, '%s*([^=]+)=([^%s$]+)%s*')
    if 'q' == p1 then
      ret.quality = tonumber(p1)
    else
      ret.params[p1] = p2
    end
  end

  return ret
end

-- Compile "etag" value to function.
--
-- @mixed  val
-- @return function
-- @api    private

local function compile_etag (val)
  if 'function' == type(val) then
    return val
  end
  --
  if true == val or 'weak' == val then
    return weak_etag
  elseif 'strong' == val then
    return etag
  elseif val == false then
    return
  else
    error(format('unknown value for etag function: %s', val))
  end
end

-- Compile "proxy trust" value to function.
--
-- @mixed  val
-- @return function
-- @api    private

local function compile_trust (val)
  local val_type = type(val)
  --
  if 'function' == val_type then return val end
  --
  if true == val then
    return function() return true end
  end
  --
  if 'number' == val_type then
    return function(a, i) return i < val end
  end
  --
  if 'string' == val_type then
    local parts = {}
    for key in gmatch(val, "([^,$]+)") do
      insert(parts, key)
    end
    val = parts
  end
  --
  return proxyaddr.compile(val or {})
end

-- Set the charset in a given Content-Type string.
--
-- @string _type
-- @string charset
-- @return string
-- @api    private

local function set_charset (_type, charset)
  if not _type or not charset then return _type end
  if find(_type, charset_reg_exp) then
    _type = gsub(_type, '(;%s*charset%s*=[^$]*)$', '')
  end
  return concat {_type, '; charset=', charset}
end

-- Check freshness of `req` and `res` headers.
--
-- When the cache is "fresh" __true__ is returned,
-- otherwise __false__ is returned to indicate that
-- the cache is now stale.
--
-- @Request  req
-- @Response res
-- @return   boolean
-- @api      public

function utils.fresh (req, res)

  local modified_since, none_match, last_modified, etag, cc, etag_matches, not_modified = 
    req['if-modified-since'], req['if-none-match'], res['last-modified'], 
    res.etag, req['cache-control'], true, true

  -- unconditional request
  if not (modified_since) and not (none_match) then
    return false
  end

  -- check for no-cache cache request directive
  if cc and (find(cc, 'no-cache', 1, true) or find(cc, 'max-age=0', 1, true)) then
    return false
  end

  if none_match then
    etag_matches = find(none_match, '*', 1, true) == 1 or 
      (etag ~= nil and find(none_match, ',?%s?'.. gsub(etag, '%+', '%%+') .. '%s?,?') ~= nil)
  end

  -- if-modified-since
  if modified_since then
    not_modified = utc2time(last_modified) <= utc2time(modified_since)
  end

  return etag_matches and not_modified
end

--
--
-- @number size
-- @string string
-- @return boolean string|table

function utils.parse_range (size, string)
  local object, _, last, unity, start, finish = {}, find(string, '([^=]+)=([^%-]*)%-?([^,$]+)')

  if not _ then
    return false, 'invalid string'
  end

  while _ do
    start, finish = tonumber(start), tonumber(finish)

    if not start then
      start, finish = size - finish, size - 1
    elseif not finish or finish > size - 1 then
      finish = size - 1
    end

    if start > finish or start < 0 then
      return false, 'invalid range'
    else
      insert(object, {start=start, finish=finish})
    end

    _, last, start, finish = find(string, ',([^%-]*)%-?([^,$]+)', last)
  end

  object.type = unity
  return true, object
end

utils.utc2time = utc2time

return utils