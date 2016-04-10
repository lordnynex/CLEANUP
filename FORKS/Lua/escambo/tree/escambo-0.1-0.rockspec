package = "escambo"
version = "0.1-0"

description = {
  summary  = "Escambo, an HTTP content negotiator, content-type parser and creator for Lua",
  homepage = "http://rocks.simbio.se/escambo",
  license  = "MIT"
}

source = {
  url    = "git://github.com/simbiose/escambo.git",
  branch = "v0.1"
}

dependencies = {
  "30log",
  "penlight"
}

build = {
  type    = "builtin",
  modules = {
    escambo = 'escambo.lua'
  }
}