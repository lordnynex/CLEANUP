#!/bin/bash
DATE=`date +%s`
DATE=`echo $DATE-86400 | bc`
if [ $1 ]; then
psql -U biin smsgate -c "select \"REQUESTID\", \"MESSAGEID\" from message_status where ( \"REQUESTID\", \"MESSAGEID\" ) IN( select \"REQUESTID\",\"MESSAGEID\" from message_history where message_history.\"WHEN\">$DATE AND \"OP_CODE\"=0 AND \"OP_DIRECTION\"=0 GROUP BY \"REQUESTID\", \"MESSAGEID\" HAVING COUNT(*) > 10 );" | grep -o -E " [0-9]+"
else
FALSE_NUMS=`psql -U biin smsgate -c "select count(*) from message_status where ( \"REQUESTID\", \"MESSAGEID\" ) IN( select \"REQUESTID\",\"MESSAGEID\" from message_history where message_history.\"WHEN\">$DATE AND \"OP_CODE\"=0 AND \"OP_DIRECTION\"=0 GROUP BY \"REQUESTID\", \"MESSAGEID\" HAVING COUNT(*) > 10 );" | grep -o -E " [0-9]+"`
fi

echo $FALSE_NUMS;
