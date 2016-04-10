#!/usr/bin/env ruby

require 'logger'
require 'rufus-scheduler'

scheduler = Rufus::Scheduler.new


def spawn_proc(cmd)
  pid = spawn cmd
  puts "Spawned [#{cmd}] with pid '#{pid}'"
  Process.detach(pid)
end

scheduler.cron '5 * * * *' do
  spawn_proc("ruby /usr/src/app/redshift_import.rb")
end

scheduler.join
