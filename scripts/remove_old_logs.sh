#!/bin/sh
rm -rf `find /var/log/smsgate/ -mtime +30`
rm -rf `find /var/log/nginx/ -mtime +30`
rm -rf `find /var/log/kannel/ -mtime +30`
