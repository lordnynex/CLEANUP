#!/bin/bash

LOG="/var/log/smsgate/restarts.log"

PATH="$PATH:/bin:/usr/bin:/usr/local/bin:/sbin:/usr/sbin:/usr/local/sbin"

function send_sms {
wget -O /tmp/wget --timeout 1 --retry-connrefused "http://127.0.0.1:8080/mt.cgi?from=GREENSMS&utf=1&user=mohtep&pass=SMSOnline&to=79852970920&txt=$1"
}

function send_email {
/home/biinilya/smsgate/msg_gen.sh $2 $1 | sendmail -v "biinilya@yandex.ru"
}

function notify {
LEVEL=$1
TXT=$2
echo $LEVEL 
echo $TXT
send_sms $TXT
send_email $LEVEL $TXT
}

echo -n "Checking /stat.cgi response "
wget -O - --timeout 10 --tries 6 --retry-connrefused http://127.0.0.1:8080/stat.cgi 2> /dev/null 1>/dev/null;
I=$?
if [ $I -eq 0 ]; then
wget -O - --timeout 1 --tries 6 --retry-connrefused http://127.0.0.1:8080/stat.cgi 2> /dev/null 1>/dev/null;
i=$?
fi
if [ $I -eq 0 ]; then
echo "[DONE]";
else
echo "[FAILURE]";
killall -ABRT smsgate
sleep 10
notify "ERROR" "Checking_/stat.cgi_response_is_negative";
fi

echo -n "Checking /mt.cgi response "
wget -O - --timeout 10 --tries 6 --retry-connrefused http://127.0.0.1:8080/stat.cgi 2> /dev/null 1>/dev/null;
I=$?
if [ $I -eq 0 ]; then
wget -O - --timeout 1 --tries 6 --retry-connrefused http://127.0.0.1:8080/stat.cgi 2> /dev/null 1>/dev/null;
I=$?
fi
if [ $I -eq 0 ]; then
echo "[DONE]";
else
echo "[FAILURE]";
killall -ABRT smsgate
sleep 10
notify "ERROR" "Checking_/mt.cgi_response_is_negative";
fi

echo -n "Checking duplicates response "
I=`/home/biin/smsgate/check_repeat.sh`                                           
if [ $I -eq 0 ]; then
echo "[DONE]";
else
echo "[FAILURE]";
/home/biin/smsgate/check_repeat.sh -v | xargs -L 2 /home/biin/smsgate/remove_id_from_db.sh
killall smsgate
sleep 10
notify "ERROR" "DUPLICATES_FOUND_FROM_DB...Fixing";
fi

echo -n "Checking duplicates response from log "
I=`/home/biin/smsgate/check_repeat_log.sh`                                           
if [ $I -eq 0 ]; then
echo "[DONE]";
else
echo "[FAILURE]";
/home/biin/smsgate/check_repeat_log.sh -v | sed 's/\./ /' | xargs -L 1 /home/biin/smsgate/remove_id_from_db.sh
killall smsgate
sleep 10
notify "ERROR" "DUPLICATES_FOUND_FROM_LOG...Fixing";
fi



