CfCmd.new do
  
  nchan_channel_id [:srv, :loc, :if],
      :nchan_set_pubsub_channel_id,
      :loc_conf,
      args: 1..7,
      alt: [ :nchan_pubsub_channel_id ],
      
      group: "pubsub",
      default: "(none)",
      info: "Channel id for a publisher or subscriber location. Can have up to 4 values to subscribe to up to 4 channels."
  
  nchan_publisher_channel_id [:srv, :loc, :if],
      :nchan_set_pub_channel_id,
      :loc_conf,
      args: 1..7,
      alt: [ :nchan_pub_channel_id ],
      
      group: "pubsub",
      default: "(none)",
      info: "Channel id for publisher location."
  
  nchan_publisher_upstream_request [:srv, :loc, :if],
      :ngx_http_set_complex_value_slot,
      [:loc_conf, :publisher_upstream_request_url],
      
      group: "pubsub",
      value: "<url>",
      info: <<-EOS.gsub(/^ {8}/, '')
        Send POST request to internal location (which may proxy to an upstream server) with published message in the request body. Useful for bridging websocket publishers with HTTP applications, or for transforming message via upstream application before publishing to a channel.  
        The upstream response code determine how publishing will proceed. A `200 OK` will publish the message from the upstream response's body. A `304 Not Modified` will publish the message as it was received from the publisher. A `204 No Content` will result in the message not being published.
      EOS
  
  nchan_channel_id_split_delimiter [:srv, :loc, :if],
      :ngx_conf_set_str_slot,
      [:loc_conf, :channel_id_split_delimiter],
      
      group: "pubsub",
      default: "(none)",
      info: "Split the channel id into several ids for multiplexing using the delimiter string provided."
      
  
  nchan_subscriber_channel_id [:srv, :loc, :if],
      :nchan_set_sub_channel_id,
      :loc_conf,
      args: 1..7,
      alt: [ :nchan_sub_channel_id ],
      group: "pubsub",
      
      default: "(none)",
      info: "Channel id for subscriber location. Can have up to 4 values to subscribe to up to 4 channels."
  
  nchan_pubsub [:srv, :loc, :if],
      :nchan_pubsub_directive,
      :loc_conf,
      args: 0..6,
      
      group: "pubsub",
      value: ["http", "websocket", "eventsource", "longpoll", "intervalpoll", "chunked", "multipart-mixed"],
      default: ["http", "websocket", "eventsource", "longpoll", "chunked", "multipart-mixed"],
      info: "Defines a server or location as a pubsub endpoint. For long-polling, GETs subscribe. and POSTs publish. For Websockets, publishing data on a connection does not yield a channel metadata response. Without additional configuration, this turns a location into an echo server."
  
  nchan_longpoll_multipart_response [:srv, :loc, :if],
      :ngx_conf_set_flag_slot,
      [:loc_conf, :longpoll_multimsg],
      args: 1,
      
      group: "pubsub",
      default: "off",
      info: "Enable sending multiple messages in a single longpoll response, separated using the multipart/mixed content-type scheme. If there is only one available message in response to a long-poll request, it is sent unmodified. This is useful for high-latency long-polling connections as a way to minimize round-trips to the server."
  
  nchan_eventsource_event [:srv, :loc, :if],
      :ngx_conf_set_str_slot,
      [:loc_conf, :eventsource_event],
      args: 1,
      
      group: "pubsub",
      default: "(none)",
      info: "Set the EventSource `event:` line to this value. When used in a publisher location, overrides the published message's `X-EventSource-Event` header and associates the message with the given value. When used in a subscriber location, overrides all messages' associated `event:` string with the given value."
      
  
  nchan_subscriber [:srv, :loc, :if],
      :nchan_subscriber_directive,
      :loc_conf,
      args: 0..5,
      legacy: "push_subscriber",
      
      group: "pubsub",
      value: ["websocket", "eventsource", "longpoll", "intervalpoll", "chunked", "multipart-mixed"],
      default: ["websocket", "eventsource", "longpoll", "chunked", "multipart-mixed"],
      info: "Defines a server or location as a channel subscriber endpoint. This location represents a subscriber's interface to a channel's message queue. The queue is traversed automatically, starting at the position defined by the `nchan_subscriber_first_message` setting.  \n The value is a list of permitted subscriber types." 
  
  nchan_subscriber_compound_etag_message_id [:srv, :loc, :if], 
      :ngx_conf_set_flag_slot,
      [:loc_conf, :msg_in_etag_only],
      args: 1,
      
      group: "pubsub",
      default: "off",
      info: <<-EOS.gsub(/^ {8}/, '')
        Override the default behavior of using both `Last-Modified` and `Etag` headers for the message id.  
        Enabling this option packs the entire message id into the `Etag` header, and discards
        `Last-Modified` and `If-Modified-Since` headers.
      EOS
      
  nchan_subscriber_message_id_custom_etag_header [:srv, :loc, :if], 
      :ngx_conf_set_str_slot, 
      [:loc_conf, :custom_msgtag_header],
      args: 1,
      
      group: "pubsub",
      default: "(none)",
      info: <<-EOS.gsub(/^ {8}/, '')
        Use a custom header instead of the Etag header for message ID in subscriber responses. This setting is a hack, useful when behind a caching proxy such as Cloudflare that under some conditions (like using gzip encoding) swallow the Etag header.
      EOS
  
  nchan_subscriber_last_message_id [:srv, :loc, :if], 
      :nchan_subscriber_last_message_id,
      :loc_conf,
      args: 1..5,
      
      group: "pubsub",
      default: ["$http_last_event_id", "$arg_last_event_id"],
      info: "If `If-Modified-Since` and `If-None-Match` headers are absent, set the message id to the first non-empty of these values. Used primarily as a workaround for the inability to set the first `Last-Message-Id` of a web browser's EventSource object. "
  
  nchan_subscriber_first_message [:srv, :loc, :if],
      :nchan_subscriber_first_message_directive,
      :loc_conf,
      args: 1,
      
      group: "pubsub",
      value: ["oldest", "newest"],
      default: "oldest",
      info: "Controls the first message received by a new subscriber. 'oldest' returns the oldest available message in a channel's message queue, 'newest' waits until a message arrives."
  
  #nchan_subscriber_concurrency [:main, :srv, :loc, :if],
  #    :nchan_set_subscriber_concurrency,
  #    [:loc_conf, :subscriber_concurrency],
  #    legacy: "push_subscriber_concurrency",
  #    
  #    group: "pubsub",
  #    value: [ :last, :first, :broadcast ],
  #    info: "Controls how multiple subscriber requests to a channel (identified by some common ID) are handled.The values work as follows:
  #    - broadcast: any number of concurrent subscriber requests may be held.
  #    - last: only the most recent subscriber request is kept, all others get a 409 Conflict response.
  #    - first: only the oldest subscriber request is kept, all others get a 409 Conflict response."
  
  nchan_websocket_ping_interval [:srv, :loc, :if],
      :ngx_conf_set_sec_slot,
      [:loc_conf, :websocket_ping_interval],
      
      group: "pubsub",
      value: "<number> (seconds)",
      default: "0 (none)",
      info: "Interval for sending websocket ping frames. Disabled by default."
  
  nchan_publisher [:srv, :loc, :if],
      :nchan_publisher_directive,
      :loc_conf,
      args: 0..2,
      legacy: "push_publisher",
      
      group: "pubsub",
      value: ["http", "websocket"],
      default: ["http", "websocket"],
      info: "Defines a server or location as a publisher endpoint. Requests to a publisher location are treated as messages to be sent to subscribers. See the protocol documentation for a detailed description."
  
  nchan_subscriber_timeout [:main, :srv, :loc, :if],
      :ngx_conf_set_sec_slot,
      [:loc_conf, :subscriber_timeout],
      legacy: "push_subscriber_timeout",
      
      group: "pubsub",
      value: "<number> (seconds)",
      default: "0 (none)",
      info: "Maximum time a subscriber may wait for a message before being disconnected. If you don't want a subscriber's connection to timeout, set this to 0. When possible, the subscriber will get a response with a `408 Request Timeout` status; otherwise the subscriber will simply be disconnected."
  
  nchan_authorize_request [:srv, :loc, :if], 
      :ngx_http_set_complex_value_slot,
      [:loc_conf, :authorize_request_url],
      
      group: "security",
      value: "<url>",
      info: "Send GET request to internal location (which may proxy to an upstream server) for authorization of a publisher or subscriber request. A 200 response authorizes the request, a 403 response forbids it."
  
  nchan_store_messages [:main, :srv, :loc, :if],
      :nchan_store_messages_directive,
      :loc_conf,
      legacy: "push_store_messages",
      
      group: "storage",
      value: [:on, :off],
      default: :on,
      info: "Publisher configuration. \"`off`\" is equivalent to setting `nchan_channel_buffer_length 0`"
    
  nchan_max_reserved_memory [:main],
      :ngx_conf_set_size_slot,
      [:main_conf, :shm_size],
      legacy: "push_max_reserved_memory",
      
      group: "storage",
      value: "<size>",
      default: "32M",
      info: "The size of the shared memory chunk this module will use for message queuing and buffering."
    
  nchan_redis_url [:main],
      :ngx_conf_set_str_slot,
      [:main_conf, :redis_url],
      
      group: "storage",
      default: "127.0.0.1:6379",
      info: "The path to a redis server, of the form 'redis://:password@hostname:6379/0'. Shorthand of the form 'host:port' or just 'host' is also accepted."
  
  nchan_use_redis [:main, :srv, :loc],
      :ngx_conf_enable_redis,
      [:loc_conf, :use_redis],
      
      group: "storage",
      value: [ :on, :off ],
      default: :off,
      info: "Use redis for message storage at this location."
  
  nchan_message_timeout [:main, :srv, :loc], 
      :ngx_conf_set_sec_slot, 
      [:loc_conf, :buffer_timeout],
      legacy: "push_message_timeout",
      
      group: "storage",
      value: "<time>",
      default: "1h",
      info: "Publisher configuration setting the length of time a message may be queued before it is considered expired. If you do not want messages to expire, set this to 0. Applicable only if a nchan_publisher is present in this or a child context."
  
  nchan_message_buffer_length [:main, :srv, :loc],
      :ngx_conf_set_num_slot,
      [:loc_conf, :max_messages],
      legacy: [ "push_max_message_buffer_length", "push_message_buffer_length" ],
      alt: ["nchan_message_max_buffer_length"],
      
      group: "storage",
      value: "<number>",
      default: 10,
      info: "Publisher configuration setting the maximum number of messages to store per channel. A channel's message buffer will retain a maximum of this many most recent messages."
  
  nchan_subscribe_existing_channels_only [:main, :srv, :loc],
      :ngx_conf_set_flag_slot, 
      [:loc_conf, :subscribe_only_existing_channel],
      legacy: "push_authorized_channels_only",
      
      group: "security",
      value: [:on, :off],
      default: :off,
      info: "Whether or not a subscriber may create a channel by sending a request to a push_subscriber location. If set to on, a publisher must send a POST or PUT request before a subscriber can request messages on the channel. Otherwise, all subscriber requests to nonexistent channels will get a 403 Forbidden response."
  
  nchan_access_control_allow_origin [:main, :srv, :loc], 
      :ngx_conf_set_str_slot,
      [:loc_conf, :allow_origin],
      args: 1,
      
      group: "security",
      value: "<string>",
      default: "*",
      info: "Set the [Cross-Origin Resource Sharing (CORS)](https://developer.mozilla.org/en-US/docs/Web/HTTP/Access_control_CORS) `Access-Control-Allow-Origin` header to this value. If the publisher or subscriber request's `Origin` header does not match this value, respond with a `403 Forbidden`."
  
  nchan_channel_group [:srv, :loc, :if], 
      :ngx_conf_set_str_slot, 
      [:loc_conf, :channel_group],
      legacy: "push_channel_group",
      
      group: "security",
      value: "<string>",
      default: "(none)",
      info: "Because settings are bound to locations and not individual channels, it is useful to be able to have channels that can be reached only from some locations and never others. That's where this setting comes in. Think of it as a prefix string for the channel id."
  
  nchan_channel_events_channel_id [:srv, :loc, :if],
      :nchan_set_channel_events_channel_id,
      :loc_conf,
      args: 1,
      
      group: "meta",
      info: "Channel id where `nchan_channel_id`'s events should be sent. Events like subscriber enqueue/dequeue, publishing messages, etc. Useful for application debugging. The channel event message is configurable via nchan_channel_event_string. The channel group for events is hardcoded to 'meta'."
  
  nchan_channel_event_string [:srv, :loc, :if], 
      :ngx_http_set_complex_value_slot,
      [:loc_conf, :channel_event_string],
      
      group: "meta",
      value: "<string>",
      default: "$nchan_channel_event $nchan_channel_id",
      info: "Contents of channel event message"
  
  nchan_max_channel_id_length [:main, :srv, :loc],
      :ngx_conf_set_num_slot,
      [:loc_conf, :max_channel_id_length],
      legacy: "push_max_channel_id_length",
      
      group: "security",
      value: "<number>",
      default: 512,
      info: "Maximum permissible channel id length (number of characters). Longer ids will be truncated."
  
  nchan_max_channel_subscribers [:main, :srv, :loc],
      :ngx_conf_set_num_slot,
      [:loc_conf, :max_channel_subscribers],
      legacy: "push_max_channel_subscribers",
      
      group: "security",
      value: "<number>",
      default: "0 (unlimited)",
      info: "Maximum concurrent subscribers."
  
  nchan_channel_timeout [:main, :srv, :loc],
      :ngx_conf_set_sec_slot,
      [:loc_conf, :channel_timeout],
      legacy: "push_channel_timeout",
      
      group: "development",
      info: "Amount of time an empty channel hangs around. Don't mess with this setting unless you know what you are doing!"
  
  nchan_storage_engine [:main, :srv, :loc],
      :nchan_set_storage_engine, 
      [:loc_conf, :storage_engine],
      
      group: "development",
      value: ["memory", "redis"],
      default: "memory",
      info: "Development directive to completely replace default storage engine. Don't use unless you are an Nchan developer."
  
  push_min_message_buffer_length [:srv, :loc, :if],
      :nchan_ignore_obsolete_setting,
      :loc_conf,
      undocumented: true,
      group: "obsolete"
  
  push_subscriber_concurrency [:srv, :loc, :if],
      :nchan_ignore_subscriber_concurrency,
      :loc_conf,
      undocumented: true,
      group: "obsolete"
  
end
