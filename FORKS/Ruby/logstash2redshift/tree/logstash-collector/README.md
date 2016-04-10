#logstash-collector
Examples used in [the post covering](http://engineering.justworks.com/2015/10/26/RailsLogstashRedshiftPart01.html) the generation and collecting of event logs on our local web app servers.

##Basic Commands

###Helper script to simulate event log generation.
```
$ ./log_generator.rb
```

###Helper script to manage example logstash docker containers.
```
$ cd /opt/logstash/logstash-collector/
$ ./logstash-collector restart
$ ./logstash-collector start
$ ./logstash-collector stop
$ ./logstash-collector rm
$ ./logstash-collector tail
```

