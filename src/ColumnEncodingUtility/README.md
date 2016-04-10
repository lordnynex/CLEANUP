# Amazon Redshift Column Encoding Utility

In order to get the best performance from your Redshift Database, you must ensure 
that database tables have the correct Column Encoding applied (http://docs.aws.amazon.com/redshift/latest/dg/t_Compressing_data_on_disk.html). 
Column Encoding specifies which algorithm is used to compress data within a column, 
and is chosen on the basis of the datatype, the unique number of discrete values 
in the column, and so on. When the COPY command (http://docs.aws.amazon.com/redshift/latest/dg/r_COPY.html) 
is used to load data into a table, column encoding will be analyzed and applied by default. 
Other tables may be loaded via Extract/Load/Transform/Load (ELT) processes, and 
these tables may require having the column encoding updated at some point.

The Redshift Column Encoding Utility gives you the ability to apply optimal Column 
Encoding to an established Schema with data already loaded. When run, it will analyze 
an entire schema or individual tables. The ANALYZE COMPRESSION (http://docs.aws.amazon.com/redshift/latest/dg/r_ANALYZE_COMPRESSION.html) 
command is used to determine if any of the columns in the table require updating, 
and if so a script is generated to convert to the optimal structure.

Because this utility can make changes to your database live (using the ```--do-execute true``` option, it is highly recommended that you thoroughly test the utility against a dev/test system, and ensure that you take a manual snapshot of Production systems prior to running the generated script. Also, as a large amount of data will be migrated, you should ensure that the migration will not adversely impact Cluster customers. AWS has thoroughly tested this software on a variety of systems, but cannot be responsible for the impact of running the utility against your database. 

## Data Migration

The script generated will take advantage of one of two data migration options. The default is to do an in place data migration within the schema being analyzed. This will create a new table with the same structure as your current table, including the distribution and sort keys, but with the correct column encoding. This table will be called ```<my_table>_$mig```. Your data will be migrated into the new table with an INSERT...SELECT... and then the existing table will be renamed to ```<my_table>_$old```, and the optimised ```<my_table>_$mig``` will be renamed to ```<my_table>```. In this way, all old data and structure are retained for future use if required. Alternatively, you can specify the ```--target-schema <my_new_schema>``` option, and the data migration will be done into the target schema with all tables retaining their original names. The source schema which has been analysed will not be modified in any way.

## Running the Column Encoding Utility

This utility was built and tested on Python 2.7x, but may work with other versions of Python. After cloning this Github project, you must ensure that you have installed the PyGresQL driver for Postgres (see http://www.pygresql.org or ```pip install pygresql```). You can then run the column encoding utility by typing ```python analyze-schema-compression.py``` or ```./analyze-schema-compression.py```. This will generate the following Usage instructions:

```
Usage: analyze-schema-compression.py
       Generates a script to optimise Redshift column encodings on all tables in a schema

Arguments: --db             - The Database to Use (or $PGDATABASE)
           --db-user        - The Database User to connect to (or $PGUSER)
           --db-host        - The Cluster endpoint (or $PGHOST)
           --db-port        - The Cluster endpoint port (default 5439 or $PGPORT)
           --analyze-schema - The Schema to be Analyzed (default public)
           --analyze-table  - A specific table to be Analyzed, if --analyze-schema is not desired
           --target-schema  - Name of a Schema into which the newly optimised tables and data should be created, rather than in place
           --threads        - The number of concurrent connections to use during analysis (default 2)
           --output-file    - The full path to the output file to be generated
           --debug          - Generate Debug Output including SQL Statements being run
           --do-execute     - Run the compression encoding optimisation
           --slot-count     - Modify the wlm_query_slot_count from the default of 1
           --ignore-errors  - Ignore errors raised in threads when running and continue processing
           --force          - Force table migration even if the table already has Column Encoding applied
           --drop-old-data  - Drop the old version of the data table, rather than renaming
           --comprows       - Set the number of rows to use for Compression Encoding Analysis
```

## Specific Usage Notes

There are a few runtime options that deserve a bit more explanation:

### Threads

The Column Encoding utility runs multiple threads to speed the generation of the output script. This means that if requested, it will consume all of the requested capacity for the Queue in which it runs. This may impact other users who are trying to run queries in this queue, and so care should be taken when running with a high number of threads. Please see http://docs.aws.amazon.com/redshift/latest/dg/cm-c-defining-query-queues.html for more information about WLM Query Queue Configuration.

### Slot Count

Within a given queue, each session will be given a single concurrency slot. In some cases, particularly when the ```--do-execute true``` option is used, you may want to use a low thread count and an increased slot count ```--slot-count N```. This will increase the amount of memory that a given session has access to, up to the limit imposed by the Query Queue. This will result in the INSERT...SELECT... used to migrate the data to the new table structure potentially running faster. However, as stated on the Threads option, you must consider the impact on other users. It is advised that Threads * SlotCount is less than the Queue Concurrency.

### Comprows

By default, the ANALYZE COMPRESSION command will attempt to analyze 100,000 rows across all Slices on the Cluster. For some types of data, you may wish to increase this value to get better coverage across all rows stored in the table.

### Do Execute

This option will cause the encoding utility to run the generated script as it goes. Changes will be made to your database LIVE and cannot be undone. It is not recommended that you use this option on Production systems. Furthermore, if the ```--drop-old-data true``` option is included with ```--do-execute true```, then you will be required to confirm that you wish to run this operation before the utility will proceed.

## Install Notes

To install PyGreSQL (Python PostgreSQL Driver) on Amazon Linux, please ensure that you follow the below steps as the ec2-user:

```
sudo easy_install pip
sudo yum install postgresql postgresql-devel gcc python-devel
sudo pip install PyGreSQL
```
