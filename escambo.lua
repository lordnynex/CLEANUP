--
-- Escambo, an HTTP content negotiator, content-type parser and creator for Lua
--
-- @author    leite (xico@simbio.se)
-- @license   MIT
-- @copyright Simbiose 2015

local string, table, tbx, class =
  require [[string]], require [[table]], require [[pl.tablex]], require [[30log]]

local Escambo, find, sub, gmatch, format, len, lower, gsub, match, concat, insert, remove, t_sort,
  tonumber, type =
  class(), string.find, string.sub, string.gmatch, string.format, string.len, string.lower,
  string.gsub, string.match, table.concat, table.insert, table.remove, tbx.sort, tonumber, type

local EMPTY, STAR, COMA, EQUAL, SSS, Q, SLASH, DASH, CONCAT_PARAMS, MIME_PATTERN, LANGUAGE_PATTERN,
  CHARSET_PATTERN, PARAM, QUOTE_ESCAPE, QUOTED_PARAM, SUBTYPE_NAME, TYPE_NAME, TYPE, TYPE_SPACE,
  TOKEN, TEXT, QUOTE =
  '', '*', ',', '=', '*/*', 'q', '/', '-', '%s;%s=%s', '%s*([^/=]+)([/=])([^;,%s]*)[;,]?%s*',
  '%s*([^%s-=;,]+)%s*([-=,]?)%s*([^%s;,]*)%s*', '%s*([^%s=;,]+)%s*([=,]?)%s*([^%s;,]*)%s*',
  ";%s*([%w!#%$%%&'%*%+%-%.%^_`|~]+)%s*=%s*([%w!#%$%%&'%*%+%-%.%^_`|~]+)%s*;", '\\([%z\1-\127])',
  ';%s*([%w!#%$%%&\'%*%+%-%.%^_`|~]+)%s*=?%s*"(.-[^\\])";?', '^[%w][%w!#%$&%^_%.%+%-]+$',
  '^[%w][%w!#%$&%^_%-]+$', '^([%w][%w!#%$&%^_%-]+)/([%w][%w!#%$&%^_%.%+%-]+)$',
  '^%s*([%w][%w!#%$&%^_%-]+)/([%w][%w!#%$&%^_%.%+%-]+)%s*;', '^[%w!#%$%%&\'%*%+%-%.%^_`|~]+$',
  '^[\32-\126\128-\255]+$', '(["])'

Escambo.__name, string, table, tbx, class = 'Escambo', nil, nil, nil, nil

-- order array with insertation sort and exclude nodes with quality below or equals zero
--
-- @table  list
-- @string join
-- @return ordered table

local function order_array(list, join)
  local i, result, key = 0, {}

  if list[1] and list[1][2] > 0 then
    result[1] = (not list[1][3] or list[1][3] == EMPTY) and list[1][1] or
      concat{list[1][1], join, list[1][3], list[1][4] or ''}
  end

  for j = 2, #list do
    if list[j][2] <= 0 then
      j = j - 1
      goto followup
    end
    key, i = list[j], j - 1
    while i > 0 and list[i][2] < key[2] do
      list[i + 1] = list[i]
      if list[i][2] > 0 then
        result[i + 1] = (not list[i][3] or list[i][3] == EMPTY) and list[i][1] or
        concat{list[i][1], join, list[i][3], list[i][4] or ''}
      end
      i = i - 1
    end

    i         = i + 1
    list[i]   = key
    result[i] = (not key[3] or key[3] == EMPTY) and key[1] or
      concat{key[1], join, key[3], key[4] or ''}

    ::followup::
  end

  return result
end

-- index multidimensional table
--
-- @table  index
-- @string prefix
-- @string suffix
-- @string params
-- @number value

local function index_it(index, prefix, suffix, params, value)
  if index[prefix] then
    if not index[ prefix ][ suffix ] then
      index[ prefix ][ suffix ] = {[ params ] = value}
    else
      index[ prefix ][ suffix ][ params ] = value
    end
  else
    index[ prefix ] = {[ suffix ] = {[ params ] = value}}
  end
end

-- get "response"/"request" object content type value
--
-- @table  object
-- @return string

local function get_content_type(object)
  if not object then
    return nil
  end
  return (object.get_header and object.get_header('content-type')) or
    (object.headers and object.headers['content-type']) or nil
end

-- escape quotes and quote string
--
-- @string string
-- @return string

local function quote(string)
  if find(string, TOKEN) then
    return true, string
  end
  if len(string) > 0 and not find(string, TEXT) then
    return false, 'invalid parameter value'
  end
  return true, concat {'"', gsub(string, QUOTE, '\\%1'), '"'}
end

-- helps language and mimetype methods to parse accept string in a loop
--
-- @string       prefix
-- @string       suffix
-- @table        selected
-- @table        provide
-- @table        index
-- @table        opts
-- @table        last
-- @boolean[opt] language

local function language_parser_helper(prefix, suffix, selected, provide, index, opts, last, language)
  if last then

    local provide_index = (opts.provided and provide[last[1]] and provide[last[1]][last[3]] and
      (language or provide[last[1]][last[3]][last[4]]))

    if language and opts.provided then
      index_it(index, last[1], STAR, last[4], last[2])
      opts.suffix_star = true
    end
    
    if not opts.provided or (last[1] == STAR or last[3] == STAR) or provide_index then
      if provide_index and not language then
        last[4] = provide[ provide[last[1]][last[3]][last[4]] ][3]
      end

      index_it(index, last[1], last[3], last[4], last[2])

      if last[1] == STAR or last[3] == STAR then
        opts.starred     = last[1] == STAR or opts.starred
        opts.suffix_star = last[3] == STAR or opts.suffix_star
      else
        insert(selected, last)
      end
    end
    last = nil
  end
  if EMPTY == prefix then return end
  opts.last = {prefix, 1, suffix, EMPTY}
end

-- parse provided media
--
-- @param  input
-- @table  result
-- @return boolean

local function parse_media(input, result)
  local acx = {}
  for i=1, #input do
    for a, b, c in gmatch(input[i] or SSS, MIME_PATTERN) do
      if SLASH == b then
        insert(result, acx)
        acx = {a, c, EMPTY}
        if result[a] then
          if not result[a][c] then result[a][c] = {} end
        else
          result[a] = {[c] = {}}
        end
      else
        if acx[3] ~= EMPTY then
          result[acx[1]][acx[2]][acx[3]] = i
        end
        acx[3] = format(CONCAT_PARAMS, acx[3], a, c)
      end
    end
    result[acx[1]][acx[2]][acx[3]] = i
    insert(result[acx[1]][acx[2]], acx[3])
  end

  if #acx > 0 then
    remove(result, 1)
    insert(result, acx)
  end

  return #result > 0
end

-- helps charset and encoding methods to parse accept string in a loop
--
-- @string value
-- @table  selected
-- @table  provide
-- @table  index
-- @table  opts

local function charset_parser_helper(value, selected, provide, index, opts)
  if STAR == value then
    opts.starred, opts.quality = true, 0
  else
    if not opts.provided or (opts.provided and provide[value]) then
      opts.last = {value, 1}
      insert(selected, opts.last)
    else
      opts.last = nil
    end
  end
  index[value] = true
end

-- parse accept charset and encoding
--
-- @string       accept
-- @table[opt]   provided
-- @boolean[opt] is_encoding
-- @return table with sanitized, filtered and ordered charset(s) or encoding(s)

local function parse_charset(accept, provided, is_encoding)

  local opts, selected, provide, index =
    {provided=false, starred=false, last=nil}, {}, {}, {}

  provided, accept = provided or {}, accept or (is_encoding and EMPTY or STAR)

  for i=1, #provided do
    provide[provided[i]], opts.provided = true, true
  end

  if is_encoding and (EMPTY == accept and not provide.identity) then
    provide.identity, opts.provided = true, true
  end

  for key, sep, val in gmatch(accept, CHARSET_PATTERN) do
    if COMA == sep then
      charset_parser_helper(key, selected, provide, index, opts)
      charset_parser_helper(val, selected, provide, index, opts)
    elseif EQUAL == sep and Q == key then
      if not opts.last then
        opts.quality = tonumber(val)
      else
        opts.last[2] = tonumber(val)
        if opts.last[2] == 0 then
          if opts.provided then provide[opts.last[1]] = nil end
          remove(selected, #selected)
        end
      end
    else
      charset_parser_helper(key, selected, provide, index, opts)
    end
  end

  if opts.provided and opts.starred then
    local x, k = opts.quality and (opts.quality + 0.01) or 0.01, ''
    for i = 1, #provided do
      k = provided[i]
      if provide[k] and not index[k] then
        x = x - 0.001
        index[k] = true
        insert(selected, {k, x})
      end
    end
  end

  if is_encoding and ((provide.identity and (not index.identity or not opts.provided)) or
    (not opts.provided and not index.identity and (not opts.quality or opts.quality > 0))) then
    insert(selected, {'identity', 0.0001})
  end

  return order_array(selected, DASH)
end

-- parse language(s)
--
-- @table[opt]  provided
-- @string[opt] accept
-- @return table with sanitized, filtered and ordered language(s)

function Escambo:languages(provided, accept)

  local opts, selected, provide, index, bar =
    {provided=false, starred=false, last=nil}, {}, {}, {}, nil

  provided, accept = provided or {}, accept or STAR

  for i=1, #provided do
    bar = find(provided[i], DASH, 0, false)
    provide[i] =
      bar and {sub(provided[i], 1, bar-1), sub(provided[i], bar+1)} or {provided[i], EMPTY}
    if provide[provide[i][1]] then
      provide[provide[i][1]][provide[i][2]] = i
    else
      provide[provide[i][1]] = {[provide[i][2]] = i}
    end
    opts.provided = true
  end

  for key, sep, val in gmatch(accept, LANGUAGE_PATTERN) do
    if COMA == sep then
      language_parser_helper(key, EMPTY, selected, provide, index, opts, opts.last, true)
      language_parser_helper(val, EMPTY, selected, provide, index, opts, opts.last, true)
    elseif DASH == sep then
      language_parser_helper(key, val, selected, provide, index, opts, opts.last, true)
    elseif EQUAL == sep and Q == key and opts.last then
      opts.last[2] = tonumber(val)
      if opts.last[2] == 0 then
        if opts.provided then
          provide[provide[opts.last[1]][opts.last[3]]] = false
          provide[opts.last[1]][opts.last[3]] = nil
        end
      end
    else
      language_parser_helper(key, EMPTY, selected, provide, index, opts, opts.last, true)
    end
  end

  language_parser_helper(EMPTY, EMPTY, selected, provide, index, opts, opts.last, true)

  if opts.provided then
    local x = 0.01
    for i=1, #provide do
      x = x - 0.001
      if provide[i] then
        if opts.starred and not index[provide[i][1]] then
          index[provide[i][1]] = {[provide[i][2]] = true}
          insert(selected, {provide[i][1], x, provide[i][2]})
        elseif index[provide[i][1]] and not index[provide[i][1]][provide[i][2]] then
          index[provide[i][1]][provide[i][2]] = true
          insert(selected, {provide[i][1], x, provide[i][2]})
        end
      end
    end
  end

  return order_array(selected, DASH)
end

-- parse mimetype
--
-- @table[opt]  provided
-- @string[opt] accept
-- @return table with sanitized, filtered and ordered mimetype(s)

function Escambo:media_types(provided, accept)

  local opts, selected, provide, index =
    {provided=false, starred=false, suffix_star=false, last=nil}, {}, {}, {}

  provided, accept = provided or {}, (self.accept or accept or SSS)
  opts.provided    = parse_media(provided, provide)

  for key, sep, val in gmatch(accept, MIME_PATTERN) do
    if SLASH == sep then
      language_parser_helper(key, val, selected, provide, index, opts, opts.last)
    elseif EQUAL == sep and opts.last then
      if Q == key then
        opts.last[2] = tonumber(val)
      else
        opts.last[4] = format(CONCAT_PARAMS, opts.last[4], key, val)
      end
    end
  end

  language_parser_helper(EMPTY, EMPTY, selected, provide, index, opts, opts.last)

  if opts.provided and (#selected > 0 or opts.starred or opts.suffix_star) then
    local already, x, prefix, suffix, params, nindex = false, 0.001, '', '', '', nil
    for i=1, #provide do
      if provide[i] then
        x, prefix, suffix, params = (x - 0.0001), provide[i][1], provide[i][2], provide[i][3]
        already = (index[prefix] and index[prefix][suffix] and index[prefix][suffix][params])

        if not already then
          if opts.starred and index[STAR][suffix] then
            nindex = (index[STAR][suffix][EMPTY] or index[STAR][suffix][params])
          elseif opts.suffix_star and index[prefix] and index[prefix][STAR] then
            nindex = (index[prefix][STAR][EMPTY] or index[prefix][STAR][params])
          elseif opts.suffix_star and opts.starred then
            nindex = (index[STAR][STAR][EMPTY] or index[STAR][STAR][params])
          end
        end

        if nindex and not already then
          insert(selected, {prefix, nindex and (nindex - x) or x, suffix, params})
          index_it(index, prefix, suffix, params, #selected)
          nindex = nil
        end
      end
    end
  end

  return order_array(selected, SLASH)
end

-- initialize "metaclass"
--
-- @string accept

function Escambo:__init(accept)
  self.accept = accept
end

-- parse content-type string
--
-- @string       string
-- @boolean[opt] expand
-- @return       boolean, string|table

function Escambo.parse(string, expand)
  if not string then
    return false, 'argument string is required'
  end
  if 'table' == type(string) then
    string = get_content_type(string)
  end
  if 'string' ~= type(string) then
    return false, 'argument string is required to be a string'
  end

  local opts, string, lenght, start, index = {parameters={}}, string .. ';', len(string), 0, 0
  start, index, opts.type, opts.subtype    = find(string, TYPE_SPACE)

  if not opts.type then
    return false, 'invalid media type'
  end

  if expand then
    opts.subtype, opts.suffix = match(lower(opts.subtype), '^([^%+$]+)%+?([^$]*)$')
    opts.type, opts.suffix    = lower(opts.type), opts.suffix ~= '' and opts.suffix or nil
  else
    opts.type    = lower(format('%s/%s', opts.type, opts.subtype))
    opts.subtype = nil
  end

  if (lenght - index) > 3 then
    local key, val, last = '', '', index

    while start do
      start, index, key, val = find(string, QUOTED_PARAM, last)
      if not start then
        start, index, key, val = find(string, PARAM, last)
      else
        val = gsub(val, QUOTE_ESCAPE, '%1')
      end

      if not start or (start - last) > 3 then
        break
      end
      opts.parameters[lower(key)], last = val, index
    end

    if last < 2 or (last and (lenght - last) > 3) or (start and (start - last) > 3) then
      return false, 'invalid parameter format'
    end
  else
    opts.parameters = nil
  end

  return true, opts
end

-- format content-type "object"
--
-- @table opts
-- @return boolean, string

function Escambo.format(opts)
  if not opts then
    return false, 'argument is required'
  end
  if not opts.type then
    return false, 'invalid type'
  end

  local string, _ = '', false

  if find(opts.type, '/', 1, true) then
    if not find(opts.type, TYPE) then
      return false, 'invalid type'
    end
    string = opts.type
  else
    if not find(opts.type, TYPE_NAME) then
      return false, 'invalid type'
    end
    if not opts.subtype or not find(opts.subtype, SUBTYPE_NAME) then
      return false, 'invalid subtype'
    end
    string = format('%s/%s', opts.type, opts.subtype)
  end

  if opts.suffix then
    if not find(opts.suffix, TYPE_NAME) then
      return false, 'invalid suffix'
    end
    string = format('%s+%s', string, opts.suffix)
  end

  if not opts.parameters then
    return true, string
  end

  for key, val in t_sort(opts.parameters) do
    if not find(key, TOKEN) then
      return false, 'invalid parameter name'
    end
    _, val = quote(val)
    if not _ then
      return _, val
    end
    string = format('%s; %s=%s', string, key, val)
  end

  return true, string
end

-- get single charset 
--
-- @param       available
-- @string[opt] accept
-- @return string

function Escambo:charset(available, accept)
  local set = parse_charset(accept or self.accept, available)
  return #set > 0 and set[1]
end

-- get list of charset(s)
--
-- @param       available
-- @string[opt] accept
-- @return table

function Escambo:charsets(available, accept)
  return parse_charset(accept or self.accept, available)
end

-- get single encoding
--
-- @param[opt]  available
-- @string[opt] accept
-- @return string

function Escambo:encoding(available, accept)
  local set = parse_charset(accept or self.accept, available, true)
  return #set > 0 and set[1]
end

-- get list of encoding(s)
--
-- @param[opt]  available
-- @string[opt] accept
-- @return table

function Escambo:encodings(available, accept)
  return parse_charset(accept or self.accept, available, true)
end

-- get single language
--
-- @param[opt]  available
-- @string[opt] accept
-- @return string

function Escambo:language(available, accept)
  local set = self:languages(accept or self.accept, available)
  return #set > 0 and set[1]
end

-- get single media type
--
-- @param[opt]  available
-- @string[opt] accept
-- @return string

function Escambo:media_type(available, accept)
  local set = self:media_types(accept or self.accept, available)
  return #set > 0 and set[1]
end

return Escambo