#!/usr/bin/env ruby

require 'json'

def random_login_attempt()
  success = [true, false].sample
  username = "fake_username"
  controller = "login"
  action = "login_submit"
  timestamp = Time.now.strftime("%Y-%m-%d %H:%M:%S.%L")

  return {timestamp: timestamp, controller: controller, action: action, username: username, success: success}
end


def random_pto_request
  requested_days = rand(5)
  requester = rand(100)
  requester_manager = rand(100)
  controller = "pto"
  action = "pto_request"
  timestamp = Time.now.strftime("%Y-%m-%d %H:%M:%S.%L")

  return {timestamp: timestamp, controller: controller, action: action, requested_days: requested_days, requester: requester, requester_manager: requester_manager}
end


EVENT_LOGGER_ROOT_PATH = '/var/log/event_logger'
login_attempt_name = "login_attempt"
pto_request_name = "pto_request"

while true
  events = {login_attempt_name => [], pto_request_name => []}

  if [true, false].sample
    events[login_attempt_name] << random_login_attempt
  end

  if [true, false].sample
    events[pto_request_name] << random_pto_request
  end


  events.each do |event_name, data|

    if data.size < 1
      next
    end

    log_directory = EVENT_LOGGER_ROOT_PATH + '/' + event_name
    Dir.mkdir(log_directory) unless File.directory?(log_directory)

    log_file_path = log_directory + '/' + event_name + "_" + Time.now.strftime('%Y%m%d') + '.log'
    file = File.new(log_file_path, 'a+')

    output = ""

    data.each do |log_line|
      output += log_line.to_json + "\n"
    end

    file.puts(output)
    file.close()

  end


  sleep rand(3)

end


