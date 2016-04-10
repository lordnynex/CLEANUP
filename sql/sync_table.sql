DROP TABLE IF EXISTS `site_sync`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `site_sync` (
  `sync_id` int(11) NOT NULL auto_increment,
  `site_name` varchar(255) NOT NULL,
  `site_dev_name` varchar(255) NOT NULL,
  `site_excludes` text,
  `site_source` varchar(500) NOT NULL,
  `site_target` varchar(500) NOT NULL,
  `source_machine` varchar(25) NOT NULL,
  `last_sync` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  PRIMARY KEY  (`sync_id`),
  UNIQUE KEY `sync_id` (`sync_id`)
);
