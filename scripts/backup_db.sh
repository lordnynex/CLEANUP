#!/bin/bash
su biin -c "pg_dump -U biin smsgate" > "/var/db_backups/`/bin/date "+%Y-%m-%d"`.smsgate.dump"
gzip --force "/var/db_backups/`/bin/date "+%Y-%m-%d"`.smsgate.dump"
