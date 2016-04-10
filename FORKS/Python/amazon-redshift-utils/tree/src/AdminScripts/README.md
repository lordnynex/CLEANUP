# Redshift Admin Scripts
Scripts objective is to help on tuning and troubleshooting.
If you are using psql, you can use ``` psql [option] -f &lt;script.sql&gt;``` to run.

| Script | Purpose |
| ------------- | ------------- |
| commit_stats.sql | Shows information on consumption of cluster resources through COMMIT statements |
| copy_performance.sql | Shows longest running copy for past 7 days |
| current_session_info.sql | Query showing information about sessions with currently running queries |
| filter_used.sql | Return filter applied to tables on scans. To aid on choosing sortkey |
| missing_table_stats.sql | Query shows EXPLAIN plans which flagged "missing statistics" on the underlying tables |
| perf_alert.sql | Return top occurrences of alerts, join with table scans |
| queuing_queries.sql | Query showing queries which are waiting on a WLM Query Slot |
| table_info.sql | Return Table storage information (size, skew, etc) |
| table_inspector.sql | Table Analysis based on content in [Analyzing Table Design](http://docs.aws.amazon.com/redshift/latest/dg/c_analyzing-table-design.html). Complements table_info.sql |
| top_queries.sql | Return the top 50 most time consuming statements in the last 7 days |
| unscanned_table_summary.sql | Summarizes storage consumed by unscanned tables |
| wlm_apex.sql | Returns overall high water-mark for WLM query queues and time queuing last occurred |
| wlm_apex_hourly.sql | Returns hourly high water-mark for WLM query queues |

