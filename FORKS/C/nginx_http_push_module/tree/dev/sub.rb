#!/usr/bin/ruby
require 'securerandom'
require_relative 'pubsub.rb'
require "optparse"


server= "localhost:8082"
par=1

opt = {
  timeout:      60,
  quit_message: 'FIN',
  client:       :longpoll,
  extra_headers: nil,
  nostore:       true,
  http2:         false
}

no_message=false
print_content_type = false
show_id=false
origin = nil

url = nil
sub = nil

opt_parser=OptionParser.new do |opts|
  opts.on("-s", "--server SERVER (#{server})", "server and port."){|v| server=v}
  opts.on("-p", "--parallel NUM (#{par})", "number of parallel clients"){|v| par = v.to_i}
  opts.on("-t", "--timeout SEC (#{opt[:timeout]})", "Long-poll timeout"){|v| opt[:timeout] = v}
  opts.on("-q", "--quit STRING (#{opt[:quit_message]})", "Quit message"){|v| opt[:quit_message] = v}
  opts.on("-l", "--client STRING (#{opt[:client]})", "sub client (one of #{Subscriber::Client.unique_aliases.join ', '})") do |v|
    opt[:client] = v.to_sym
  end
  opts.on("-c", "--content-type", "show received content-type"){|v| print_content_type = true}
  opts.on("-i", "--id", "Print message id (last-modified and etag headers)."){|v| show_id = true}
  opts.on("-n", "--no-message", "Don't output retrieved message."){|v| no_message = true}
  opts.on("--origin STR", "Set Origin header if appplicable."){|v| origin = v}
  opts.on("--full-url URL", "full subscriber url") do |v|
    url = v
  end
  opts.on("--http2", "use HTTP/2"){opt[:http2] = true}
  opts.on("-v", "--verbose", "somewhat rather extraneously wordful output") do
    opt[:verbose] = true
    Typhoeus::Config.verbose=true
  end
end
opt_parser.banner="Usage: sub.rb [options] url"
opt_parser.parse!

url ||= "#{opt[:http2] ? 'h2' : 'http'}://#{server}#{ARGV.last}"

puts "Subscribing #{par} #{opt[:client]} client#{par!=1 ? "s":""} to #{url}."
puts "Timeout: #{opt[:timeout]}sec, quit msg: #{opt[:quit_message]}"

if origin
  opt[:extra_headers] ||= {}
  opt[:extra_headers]['Origin'] = origin
end

sub = Subscriber.new url, par, opt

nomsgmessage="\r"*30 + "Received message %i, len:%i"


msg_count=0
sub.on_message do |msg|
  if no_message
    msg_count+=1
    printf nomsgmessage, msg_count, msg.message.length
  else
    if msg.content_type
      out = "(#{msg.content_type}) #{msg}"
    else
      out = msg.to_s
    end
    if show_id
      out = "<#{msg.id}> #{out}"
    end
    puts out
  end
end

sub.on_failure do |err_msg|
  if Subscriber::IntervalPollClient === sub.client
    unless err_msg.match(/\(code 304\)/)
      false
    end 
  else
    false
  end
end

sub.run
sub.wait


if sub.errors.count > 0
  puts "Errors:"
  sub.errors.each do |err|
    puts err
  end
  exit 1
end
