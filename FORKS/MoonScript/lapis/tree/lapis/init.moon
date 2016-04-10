
application = require "lapis.application"

import Application from application

app_cache = {}
setmetatable app_cache, __mode: "k"

local dispatcher

serve = (app_cls) ->
  app = app_cache[app_cls]

  unless app
    name = app_cls
    if type(name) == "string"
      app_cls = require(name)

    app = if app_cls.__base -- is a class
      app_cls!
    else
      app_cls\build_router!
      app_cls

    app_cache[name] = app


  unless dispatcher
    dispatcher = if __leda
      require "lapis.leda"
    else
      require "lapis.nginx"

  dispatcher.dispatch app

{
  :serve, :application, :Application, :app_cache
}
