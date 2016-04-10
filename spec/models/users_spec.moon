import use_test_env from require "lapis.spec"
import truncate_tables from require "lapis.spec.db"

import Users from require "models"
import CommunityUsers from require "community.models"

factory = require "spec.factory"

describe "models.users", ->
  use_test_env!

  before_each ->
    truncate_tables Users, CommunityUsers

  it "should create a user", ->
    factory.Users!


