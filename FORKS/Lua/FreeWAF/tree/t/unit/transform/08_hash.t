use Test::Nginx::Socket::Lua;

repeat_each(3);
plan tests => repeat_each() * 3 * blocks();

no_shuffle();
run_tests();

__DATA__

=== TEST 1: md5
--- config
	location /t {
		content_by_lua '
			local lookup    = require "lib.lookup"
			local util      = require "lib.util"
			local value     = "hello world"
			local transform = lookup.transform["md5"]({}, value)
			ngx.say(util.hex_encode(transform))
		';
	}
--- request
GET /t
--- error_code: 200
--- response_body
5eb63bbbe01eeed093cb22bb8f5acdc3
--- no_error_log
[error]

=== TEST 2: hex-encoded md5 matches ngx.md5
--- config
	location /t {
		content_by_lua '
			local lookup    = require "lib.lookup"
			local util      = require "lib.util"
			local value     = "hello world"
			local transform = lookup.transform["md5"]({}, value)
			ngx.say(util.hex_encode(transform) == ngx.md5(value))
		';
	}
--- request
GET /t
--- error_code: 200
--- response_body
true
--- no_error_log
[error]

=== TEST 3: sha1
--- config
	location /t {
		content_by_lua '
			local lookup    = require "lib.lookup"
			local util      = require "lib.util"
			local value     = "hello world"
			local transform = lookup.transform["sha1"]({}, value)
			ngx.say(util.hex_encode(transform))
		';
	}
--- request
GET /t
--- error_code: 200
--- response_body
2aae6c35c94fcfb415dbe95f408b9ce91ee846ed
--- no_error_log
[error]

