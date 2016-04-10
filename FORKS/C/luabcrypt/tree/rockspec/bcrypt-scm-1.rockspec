package = "bcrypt"
version = "scm-1"

source = {
	url = "git://github.com/leafo/luabcrypt.git",
}

description = {
	summary = "A Lua wrapper for bcrypt",
	homepage = "http://github.com/mikejsavage/luabcrypt",
	license = "MIT",
	maintainer = "Mike Savage",
}

dependencies = {
	"lua ~> 5.1",
}

build = {
	type = "builtin",
	modules = {
		bcrypt = {
			sources = {"src/luabcrypt.c", "src/bcrypt.c", "src/blowfish.c"},
		}
	}
}
