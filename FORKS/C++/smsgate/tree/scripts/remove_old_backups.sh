#!/bin/sh
rm -rf `find /var/db_backups/ -mtime +14`
