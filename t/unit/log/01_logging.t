use Test::Nginx::Socket::Lua;

repeat_each(3);
plan tests => repeat_each() * 3 * blocks();

no_shuffle();
run_tests();

__DATA__

=== TEST 1: Log a string
--- http_config
	init_by_lua '
		lua_resty_waf = require "waf"
		logger  = require "lib.log"
	';
--- config
	location /t {
		access_by_lua '
			local waf      = lua_resty_waf:new()
			waf:set_option("debug", true)
			logger.log(waf, "We have logged a string!")
		';

		content_by_lua 'ngx.exit(ngx.HTTP_OK)';
	}
--- request
GET /t
--- reponse_body
--- error_log
We have logged a string!
--- no_error_log
[error]

=== TEST 2: Log a string at ngx.INFO log level
--- http_config
	init_by_lua '
		lua_resty_waf = require "waf"
		logger  = require "lib.log"
	';
--- config
	location /t {
		access_by_lua '
			local waf      = lua_resty_waf:new()
			waf:set_option("debug", true)
			waf:set_option("debug_log_level", ngx.INFO)
			logger.log(waf, "We have logged a string!")
		';

		content_by_lua 'ngx.exit(ngx.HTTP_OK)';
	}
--- log_level
info
--- request
GET /t
--- reponse_body
--- error_log
We have logged a string!
--- no_error_log
[error]

=== TEST 3: Log a string at ngx.WARN log level
--- http_config
	init_by_lua '
		lua_resty_waf = require "waf"
		logger  = require "lib.log"
	';
--- config
	location /t {
		access_by_lua '
			local waf      = lua_resty_waf:new()
			waf:set_option("debug", true)
			waf:set_option("debug_log_level", ngx.WARN)
			logger.log(waf, "We have logged a string!")
		';

		content_by_lua 'ngx.exit(ngx.HTTP_OK)';
	}
--- request
GET /t
--- reponse_body
--- error_log eval
qr/\[warn\].*We have logged a string!/
--- no_error_log
[error]

=== TEST 4: Log a string at ngx.WARN log level
--- http_config
	init_by_lua '
		lua_resty_waf = require "waf"
		logger  = require "lib.log"
	';
--- config
	location /t {
		access_by_lua '
			local waf      = lua_resty_waf:new()
			waf:set_option("debug", true)
			waf:set_option("debug_log_level", ngx.DEBUG)
			logger.log(waf, "We have logged a string!")
		';

		content_by_lua 'ngx.exit(ngx.HTTP_OK)';
	}
--- request
GET /t
--- reponse_body
--- error_log eval
qr/\[debug\].*We have logged a string!/
--- no_error_log
[error]

=== TEST 5: Do not log a string if debug disabled
--- http_config
	init_by_lua '
		lua_resty_waf = require "waf"
		logger  = require "lib.log"
	';
--- config
	location /t {
		access_by_lua '
			local waf      = lua_resty_waf:new()
			waf:set_option("debug", false)
			logger.log(waf, "We have logged a string!")
		';

		content_by_lua 'ngx.exit(ngx.HTTP_OK)';
	}
--- request
GET /t
--- reponse_body
--- no_error_log
We have logged a string!
[error]

=== TEST 6: Do not log a string if insufficient log level
--- http_config
	init_by_lua '
		lua_resty_waf = require "waf"
		logger  = require "lib.log"
	';
--- config
	location /t {
		access_by_lua '
			local waf      = lua_resty_waf:new()
			waf:set_option("debug", true)
			logger.log(waf, "We have logged a string!")
		';

		content_by_lua 'ngx.exit(ngx.HTTP_OK)';
	}
--- log_level
notice
--- request
GET /t
--- reponse_body
--- no_error_log
We have logged a string!
[error]

