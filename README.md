#Rails->Logstash->Redshift

This is a repo of examples to accompany [a post on our engineering blog](http://engineering.justworks.com/2015/10/25/RailsLogstashRedshiftIntro.html).

## High Level Architecture
![Arch](http://engineering.justworks.com/assets/2015-10-26-RailsLogstashRedshift/LogstashOverall01.png)

##Repo Structure

###logstash-collector
Collects logs and sends to Redis.

###logstash-indexer
Deals with logs in redis and passes on to various outputs.

###redshift-importer
Takes outputs from logstash hourly files and imports them into redshift.

##Usage
See README.md's in directories for usage.
