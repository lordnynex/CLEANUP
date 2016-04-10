use Test::Nginx::Socket::Lua;

repeat_each(3);
plan tests => repeat_each() * 3 * blocks();

no_shuffle();
run_tests();

__DATA__

=== TEST 1: Table with a single k/v pair
--- config
    location = /t {
        content_by_lua '
			local util = require "lib.util"
			local a = { foo = "bar" }
			local b = util.table_values(a)
			table.sort(b)
			for i in ipairs(b) do
				ngx.say(b[i])
			end
		';
	}
--- request
GET /t
--- error_code: 200
--- response_body
bar
--- no_error_log
[error]

=== TEST 2: Table with multiple k/v pairs
--- config
    location = /t {
        content_by_lua '
			local util = require "lib.util"
			local a = { foo = "bar", baz = "bat", qux = "frob" }
			local b = util.table_values(a)
			table.sort(b)
			for i in ipairs(b) do
				ngx.say(b[i])
			end
		';
	}
--- request
GET /t
--- error_code: 200
--- response_body
bar
bat
frob
--- no_error_log
[error]

=== TEST 3: Table with nested k/v pairs
--- config
    location = /t {
        content_by_lua '
			local util = require "lib.util"
			local a = { foo = { "bar", "baz", "bat" }, qux = { "frob" } }
			local b = util.table_values(a)
			table.sort(b)
			for i in ipairs(b) do
				ngx.say(b[i])
			end
		';
	}
--- request
GET /t
--- error_code: 200
--- response_body
bar
bat
baz
frob
--- no_error_log
[error]

=== TEST 4: Table with redundant keys
# n.b. the ngx API will not present data in this fashion
--- config
    location = /t {
        content_by_lua '
            local util = require "lib.util"
            local a = { foo = "bar", foo = "baz" }
            local b = util.table_values(a)
			table.sort(b)
            for i in ipairs(b) do
                ngx.say(b[i])
            end
        ';
    }
--- request
GET /t
--- error_code: 200
--- response_body
baz
--- no_error_log
[error]

=== TEST 5: Not a table
--- config
    location = /t {
        content_by_lua '
			local util = require "lib.util"
			local a = "foo, bar"
			local b = util.table_values(a)
			table.sort(b)
			for i in ipairs(b) do
				ngx.say(b[i])
			end
		';
	}
--- request
GET /t
--- error_code: 500
--- error_log
fatal_fail
was given to table_values!

