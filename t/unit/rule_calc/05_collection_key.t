use Test::Nginx::Socket::Lua;

repeat_each(3);
plan tests => repeat_each() * 3 * blocks();

no_shuffle();
run_tests();

__DATA__

=== TEST 1: No parse, no transform, no length
--- config
	location /t {
		content_by_lua '
			local rule_calc  = require "lib.rule_calc"
			local mock_rules = {
				{ id = 1, vars = { { type = "FOO" } }, action = "DENY" },
			}

			rule_calc.calculate(mock_rules)

			ngx.say(mock_rules[1].vars[1].collection_key)
		';
	}
--- request
GET /t
--- response_body
FOO|nil|nil
--- error_code: 200
--- no_error_log
[error]

=== TEST 2: Truthy key, no transform, no length
--- config
	location /t {
		content_by_lua '
			local rule_calc  = require "lib.rule_calc"
			local mock_rules = {
				{ id = 1, vars = { { type = "FOO", parse = { keys = 1 } } }, action = "DENY" },
			}

			rule_calc.calculate(mock_rules)

			ngx.say(mock_rules[1].vars[1].collection_key)
		';
	}
--- request
GET /t
--- response_body
FOO|keys|1|nil|nil
--- error_code: 200
--- no_error_log
[error]

=== TEST 3: String key, no transform, no length
--- config
	location /t {
		content_by_lua '
			local rule_calc  = require "lib.rule_calc"
			local mock_rules = {
				{ id = 1, vars = { { type = "FOO", parse = { specific = "bar" } } }, action = "DENY" },
			}

			rule_calc.calculate(mock_rules)

			ngx.say(mock_rules[1].vars[1].collection_key)
		';
	}
--- request
GET /t
--- response_body
FOO|specific|bar|nil|nil
--- error_code: 200
--- no_error_log
[error]

=== TEST 4: No parse, single transform, no length
--- config
	location /t {
		content_by_lua '
			local rule_calc  = require "lib.rule_calc"
			local mock_rules = {
				{ id = 1, vars = { { type = "FOO" } }, opts = { transform = "bar" }, action = "DENY" },
			}

			rule_calc.calculate(mock_rules)

			ngx.say(mock_rules[1].vars[1].collection_key)
		';
	}
--- request
GET /t
--- response_body
FOO|bar|nil
--- error_code: 200
--- no_error_log
[error]

=== TEST 5: No parse, multiple transforms, no length
--- config
	location /t {
		content_by_lua '
			local rule_calc  = require "lib.rule_calc"
			local mock_rules = {
				{ id = 1, vars = { { type = "FOO" } }, opts = { transform = { "bar", "bat" } }, action = "DENY" },
			}

			rule_calc.calculate(mock_rules)

			ngx.say(mock_rules[1].vars[1].collection_key)
		';
	}
--- request
GET /t
--- response_body
FOO|bar,bat|nil
--- error_code: 200
--- no_error_log
[error]

=== TEST 6: No parse, no transform, length
--- config
	location /t {
		content_by_lua '
			local rule_calc  = require "lib.rule_calc"
			local mock_rules = {
				{ id = 1, vars = { { type = "FOO", length = 1 } }, action = "DENY" },
			}

			rule_calc.calculate(mock_rules)

			ngx.say(mock_rules[1].vars[1].collection_key)
		';
	}
--- request
GET /t
--- response_body
FOO|nil|1
--- error_code: 200
--- no_error_log
[error]

