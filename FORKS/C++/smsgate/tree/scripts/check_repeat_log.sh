#!/bin/bash
if [ $1 ]; then
tail -n 200000 /var/log/smsgate/msg.log-`date "+%Y-%b-%d"` | grep -E "(mt_[a-z]{2,10};?){5,}" | grep -o -E "[0-9]{10,}\.[0-9]{1,}" | sort -u
else
tail -n 200000 /var/log/smsgate/msg.log-`date "+%Y-%b-%d"` | grep -E "(mt_[a-z]{2,10};?){5,}" | grep -o -E "[0-9]{10,}\.[0-9]{1,}" | sort -u | wc -l
fi
