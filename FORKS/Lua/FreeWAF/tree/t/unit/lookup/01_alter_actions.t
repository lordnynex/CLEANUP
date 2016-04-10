use Test::Nginx::Socket::Lua;

repeat_each(3);
plan tests => repeat_each() * 3 * blocks();

no_shuffle();
run_tests();

__DATA__

=== TEST 1: ACCEPT is an alter action
--- config
	location /t {
		access_by_lua '
			local lookup = require "lib.lookup"
			local alter_actions = lookup.alter_actions
			for action in pairs(alter_actions) do
				ngx.say(action)
			end
		';

		content_by_lua 'ngx.exit(ngx.HTTP_OK)';
	}
--- request
GET /t
--- error_code: 200
--- response_body_like
ACCEPT
--- no_error_log
[error]

=== TEST 2: DENY is an alter action
--- request
GET /t
--- error_code: 200
--- response_body_like
DENY
--- no_error_log
[error]

=== TEST 3: IGNORE is not an alter action
--- request
GET /t
--- error_code: 200
--- response_body_unlike
IGNORE
--- no_error_log
[error]

=== TEST 4: CHAIN is not an alter action
--- request
GET /t
--- error_code: 200
--- response_body_unlike
CHAIN
--- no_error_log
[error]

=== TEST 5: SCORE is not an alter action
--- request
GET /t
--- error_code: 200
--- response_body_unlike
SCORE
--- no_error_log
[error]
