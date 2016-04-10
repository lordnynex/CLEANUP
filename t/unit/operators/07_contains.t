use Test::Nginx::Socket::Lua;

repeat_each(3);
plan tests => repeat_each() * 3 * blocks();

no_shuffle();
run_tests();

__DATA__

=== TEST 1: Match (individual)
--- config
    location = /t {
        content_by_lua '
			local op = require "lib.operators"
			local match, value = op.contains({ "foo", "bar", "baz", "qux" }, "foo")
			ngx.say(match)
		';
	}
--- request
GET /t
--- error_code: 200
--- response_body
true
--- no_error_log
[error]

=== TEST 2: Match (table)
--- config
    location = /t {
        content_by_lua '
			local op = require "lib.operators"
			local match, value = op.contains({ "foo", "bar", "baz", "qux" }, { "bang", "bash", "qux" })
			ngx.say(match)
		';
	}
--- request
GET /t
--- error_code: 200
--- response_body
true
--- no_error_log
[error]

=== TEST 3: No match (individual)
--- config
    location = /t {
        content_by_lua '
			local op = require "lib.operators"
			local match, value = op.contains({ "foo", "bar", "baz", "qux" }, "far")
			ngx.say(match)
		';
	}
--- request
GET /t
--- error_code: 200
--- response_body
false
--- no_error_log
[error]

=== TEST 4: No match (table)
--- config
    location = /t {
        content_by_lua '
			local op = require "lib.operators"
			local match, value = op.contains({ "foo", "bar", "baz", "qux" }, { "bang", "bash", "quz" })
			ngx.say(match)
		';
	}
--- request
GET /t
--- error_code: 200
--- response_body
false
--- no_error_log
[error]

=== TEST 5: Return values
--- config
    location = /t {
        content_by_lua '
			local op = require "lib.operators"
			local match, value = op.contains({ "foo", "bar", "baz", "qux" }, "foo")
			ngx.say(match)
			ngx.say(value)
		';
	}
--- request
GET /t
--- error_code: 200
--- response_body
true
foo
--- no_error_log
[error]

=== TEST 6: Return value types
--- config
    location = /t {
        content_by_lua '
			local op = require "lib.operators"
			local match, value = op.contains({ "foo", "bar", "baz", "qux" }, "foo")
			ngx.say(type(match))
			ngx.say(type(value))
		';
	}
--- request
GET /t
--- error_code: 200
--- response_body
boolean
string
--- no_error_log
[error]

