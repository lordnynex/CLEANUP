
import Model from require "lapis.db.model"

class Users extends Model
  @timestamp: true
  is_admin: => false
  name_for_display: => @username
