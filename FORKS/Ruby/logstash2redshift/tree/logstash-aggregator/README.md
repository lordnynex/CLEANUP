#logstash-aggregator
Examples used in [the post covering](http://engineering.justworks.com/2015/10/27/RailsLogstashRedshiftPart02.html) the aggregation of our event logs via Redis and Logstash.


##Basic Commands

###Running Redis

```
$ docker run --name redis -p 6379:6379 -d redis
```

###Helper script to manage example logstash docker containers.
```
$ cd /opt/logstash/logstash-aggregator/
$ ./logstash-aggregator restart
$ ./logstash-aggregator start
$ ./logstash-aggregator stop
$ ./logstash-aggregator rm
$ ./logstash-aggregator tail
```

