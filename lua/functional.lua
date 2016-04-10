-- Functional Library
--
-- @file    functional.lua
-- @author  Shimomura Ikkei
-- @date    2005/05/18
--
-- @brief    porting several convenience functional utilities form Haskell,Python etc..

-- Slightly edited pairs to ipairs (order not guaranteed in pairs)

-- map(function, table)
-- e.g: map(double, {1,2,3})    -> {2,4,6}
local f = {}

function f.map(func, tbl)
	local newtbl = {}
	for i,v in pairs(tbl) do
		newtbl[i] = func(v)
	end
	return newtbl
end

function f.each(func, tbl)
	local newtbl = {}
	for _,v in pairs(tbl) do
		 func(v)
	end
	return nil
end

-- filter(function, table)
-- e.g: filter(is_even, {1,2,3,4}) -> {2,4}
function f.filter(func, tbl)
	local newtbl= {}
	for i,v in pairs(tbl) do
		if func(v) then
			newtbl[i]=v
		end
	end
	return newtbl
end

-- head(table)
-- e.g: head({1,2,3}) -> 1
function f.head(tbl)
	return tbl[1]
end

-- tail(table)
-- e.g: tail({1,2,3}) -> {2,3}
--
-- XXX This is a BAD and ugly implementation.
-- should return the address to next porinter, like in C (arr+1)
function f.tail(tbl)
	if table.getn(tbl) < 1 then
		return nil
	else
		local newtbl = {}
		local tblsize = table.getn(tbl)
		local i = 2
		while (i <= tblsize) do
			table.insert(newtbl, i-1, tbl[i])
			i = i + 1
		end
		return newtbl
	end
end

-- foldr(function, default_value, table)
-- e.g: foldr(operator.mul, 1, {1,2,3,4,5}) -> 120
function f.foldr(func, val, tbl)
	for i,v in ipairs(tbl) do
		val = func(val, v)
	end
	return val
end

function f.all(func, tbl)
	for i,v in ipairs(tbl) do
		if not func(tbl[1]) then
			return false
		end
	end
	return true
end

f.not_all = function(...)
	return not f.all(...)
end

f.none = function(func, tbl)
	return f.all(function(...)
								 return not func(...) end,
							 tbl)
end

f.some = function(...)
	return not f.none(...)
end
f.any = f.some

-- reduce(function, table)
-- e.g: reduce(operator.add, {1,2,3,4}) -> 10
function f.reduce(func, tbl)
	return foldr(func, f.head(tbl), f.tail(tbl))
end

-- curry(f,g)
-- e.g: printf = curry(io.write, string.format)
--          -> function(...) return io.write(string.format(unpack(arg))) end
function f.curry(f,g)
	return function (...)
		return f(g(...))
	end
end

-- bind1(func, binding_value_for_1st)
-- bind2(func, binding_value_for_2nd)
-- @brief
--      Binding argument(s) and generate new function.
-- @see also STL's functional, Boost's Lambda, Combine, Bind.
-- @examples
--      local mul5 = bind1(operator.mul, 5) -- mul5(10) is 5 * 10
--      local sub2 = bind2(operator.sub, 2) -- sub2(5) is 5 -2
function bind1(func, val1)
	return function (val2)
		return func(val1, val2)
	end
end
function bind2(func, val2) -- bind second argument.
	return function (val1)
		return func(val1, val2)
	end
end

-- is(checker_function, expected_value)
-- @brief
--      check function generator. return the function to return boolean,
--      if the condition was expected then true, else false.
-- @example
--      local is_table = is(type, "table")
--      local is_even = is(bind2(math.mod, 2), 1)
--      local is_odd = is(bind2(math.mod, 2), 0)
function f.is(check, expected)
	return function (...)
		if (check(...) == expected) then
			return true
		else
			return false
		end
	end
end

-- operator table.
-- @see also python's operator module.
local operator = {
	mod = math.mod;
	pow = math.pow;
	add = function(n,m) return n + m end;
	sub = function(n,m) return n - m end;
	mul = function(n,m) return n * m end;
	div = function(n,m) return n / m end;
	gt  = function(n,m) return n > m end;
	lt  = function(n,m) return n < m end;
	eq  = function(n,m) return n == m end;
	le  = function(n,m) return n <= m end;
	ge  = function(n,m) return n >= m end;
	ne  = function(n,m) return n ~= m end;

}

-- enumFromTo(from, to)
-- e.g: enumFromTo(1, 10) -> {1,2,3,4,5,6,7,8,9}
-- TODO How to lazy evaluate in Lua? (thinking with coroutine)
f.enumFromTo = function (from,to)
	local newtbl = {}
	local step = bind2(operator[(from < to) and "add" or "sub"], 1)
	local val = from
	while val <= to do
		table.insert(newtbl, table.getn(newtbl)+1, val)
		val = step(val)
	end
	return newtbl
end

-- make function to take variant arguments, replace of a table.
-- this does not mean expand the arguments of function took,
-- it expand the function's spec: function(tbl) -> function(...)
function f.expand_args(func)
	return function(...) return func(...) end
end

return f
