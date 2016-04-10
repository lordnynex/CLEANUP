OPENRESTY_PREFIX=/usr/local/openresty

PREFIX ?= /usr/local/openresty
LUA_INCLUDE_DIR ?= $(PREFIX)/include
LUA_LIB_DIR ?= $(PREFIX)/lualib
INSTALL ?= install

.PHONY: all test install

all: ;

install: all
	$(INSTALL) -d $(LUA_LIB_DIR)/resty/s3
	$(INSTALL) lib/resty/s3/*.lua $(LUA_LIB_DIR)/resty/s3
