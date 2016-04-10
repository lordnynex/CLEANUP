#!/usr/local/bin/bash
/usr/local/bin/psql -U mohtep smsgate -c 'UPDATE message_status SET "PARTS"=msg_parts_num( "TXT" ) FROM smsrequest WHERE message_status."REQUESTID"=smsrequest."REQUESTID" 
AND 
smsrequest."REQUESTID" IN ( SELECT "REQUESTID" FROM message_status WHERE "PARTS" IS NULL LIMIT 10000 );'
