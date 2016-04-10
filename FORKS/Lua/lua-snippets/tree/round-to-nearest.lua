#!/usr/bin/env lua

require('helpers')

local round_to_nearest =
	function (n, mul)
		return math.floor(n / mul + 0.5) * mul
	end

println(
	'Rounding to nearest 15.',
	'Under: %d -> %d' % { 7, round_to_nearest(7, 15) },
	' Over: %d -> %d' % { 8, round_to_nearest(8, 15) }
)
