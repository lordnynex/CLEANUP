/**********************************************************************************************
Purpose: Return the top 50 time consuming statements aggregated by it's text.

Columns:
DB:		Database where the query ran
n_qry:		Number of Queries with same SQL text
qrytext:	First 80 Characters of the query SQL
min/max/avg:	Min/Max/Avg Execution time
total:		Total execution time of all occurences
max_query_id:	Largest query id of the query occurence
last_run:	Last day the query ran
aborted:	0 if query ran to completion, 1 if it was canceled.
alert:      Alert event related to the query
 
Notes:
There is a commented filter of the query to filter for only Select statements (otherwise it includes all statements like insert, update, COPY)
There is a commented filter to narrow the query to a given database

History:
2015-02-09 ericfe created
2015-04-17 ericfe Added event name and event time filter
**********************************************************************************************/
-- query runtimes
select trim(database) as DB, count(query) as n_qry, max(substring (qrytext,1,80)) as qrytext, min(run_minutes) as "min" , max(run_minutes) as "max", avg(run_minutes) as "avg", sum(run_minutes) as total,  max(query) as max_query_id, 
max(starttime)::date as last_run, aborted, event
from (
select userid, label, stl_query.query, trim(database) as database, trim(querytxt) as qrytext, md5(trim(querytxt)) as qry_md5, starttime, endtime, datediff(seconds, starttime,endtime)::numeric(12,2) as run_minutes, 
       aborted, decode(alrt.event,'Very selective query filter','Filter','Scanned a large number of deleted rows','Deleted','Nested Loop Join in the query plan','Nested Loop','Distributed a large number of rows across the network','Distributed','Broadcasted a large number of rows across the network','Broadcast','Missing query planner statistics','Stats',alrt.event) as event
from stl_query 
left outer join ( select query, trim(split_part(event,':',1)) as event from STL_ALERT_EVENT_LOG where event_time >=  dateadd(day, -7, current_Date)  group by query, trim(split_part(event,':',1)) ) as alrt on alrt.query = stl_query.query
where userid <> 1 
-- and (querytxt like 'SELECT%' or querytxt like 'select%' ) 
-- and database = ''
and starttime >=  dateadd(day, -7, current_Date)
 ) 
group by database, label, qry_md5, aborted, event
order by total desc limit 50;
