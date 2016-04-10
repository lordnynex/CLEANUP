Name
======
lua-resty-s3 - Lua S3-Like API

Dependencies
======

[lua-resty-mongol](https://github.com/gooo000/lua-resty-mongol.git)

[lua-resty-upload](https://github.com/openresty/lua-resty-upload.git)

[lua-cjson-2.1.0](http://www.kyne.com.au/~mark/software/lua-cjson.php)

[luafilesystem](https://github.com/keplerproject/luafilesystem.git)

[lua-resty-string](https://github.com/openresty/lua-resty-string.git)

[lua-resty-uuid](https://github.com/bungle/lua-resty-uuid.git)

[magick](https://github.com/gooo000/magick.git)

Installation
======

		make install

Usage
======

Add package path into nginx.conf.

        lua_package_path '/usr/local/openresty/lualib/?.lua;/usr/local/openresty/lualib/?/init.lua;;';

        content_by_lua_file /usr/local/openresty/lualib/resty/s3/init.lua;
