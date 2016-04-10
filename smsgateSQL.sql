CREATE TABLE message_status (
    "REQUESTID" BIGINT NOT NULL,
    "MESSAGEID" INTEGER NOT NULL,
    "STATUS" INTEGER NOT NULL,
    "TO" TEXT NOT NULL,
    "PARTS" INTEGER NOT NULL,
    "PARTNERPRICE" REAL DEFAULT 0,
    "OURPRICE" REAL DEFAULT 0,
    "GATEWAY" TEXT NOT NULL,
    "COUNTRY" TEXT,
    "COUNTRYCODE" TEXT,
    "OPERATOR" TEXT,
    "OPERATORCODE" TEXT,
    "REGION" TEXT,
    "WHEN" INTEGER
);
CREATE INDEX message_status_REQUESTID on message_status( "REQUESTID" );
CREATE INDEX message_status_WHEN on message_status( "WHEN" );
CREATE INDEX message_status_TO on message_status( "TO" );
CREATE INDEX message_status_COUNTRY on message_status( "COUNTRY" );
CREATE INDEX message_status_COUNTRYCODE on message_status( "COUNTRYCODE" );
CREATE INDEX message_status_OPERATOR on message_status( "OPERATOR" );
CREATE INDEX message_status_OPERATORCODE on message_status( "OPERATORCODE" );
CREATE INDEX message_status_REGION on message_status( "REGION" );

CREATE TABLE message_history (
    "REQUESTID" BIGINT NOT NULL,
    "MESSAGEID" INTEGER NOT NULL,
    "OP_CODE" INTEGER NOT NULL,
    "OP_DIRECTION" INTEGER NOT NULL,
    "OP_RESULT" INTEGER NOT NULL,
    "GATEWAY" TEXT NOT NULL,
    "WHEN" INTEGER
);
CREATE INDEX message_history_REQUESTID on message_history( "REQUESTID" );
CREATE INDEX message_history_WHEN on message_history( "WHEN" );
CREATE INDEX message_history_GATEWAY on message_history( "GATEWAY" );
CREATE INDEX message_history_WHEN_GATEWAY on message_history( "WHEN", "GATEWAY" );

CREATE TABLE smsrequest (
    "REQUESTID" BIGINT PRIMARY KEY NOT NULL,
    "USER" TEXT NOT NULL,
    "PASS" TEXT NOT NULL,
    "TO" TEXT NOT NULL,
    "TXT" TEXT NOT NULL,
    "TID" TEXT,
    "FROM" TEXT,
    "UTF" INTEGER,
    "SUBPREF" TEXT,
    "HEX" INTEGER,
    "UDH" TEXT,
    "DELAY" INTEGER,
    "DLR" INTEGER,
    "PID" TEXT,
    "PRIORITY" INTEGER,
    "GARANT" INTEGER,
    "WHEN" INTEGER
);
CREATE INDEX smsrequest_REQUESTID on smsrequest( "REQUESTID" );
CREATE INDEX smsrequest_WHEN on smsrequest( "WHEN" );
CREATE INDEX smsrequest_TXT on smsrequest( "TXT" );
CREATE INDEX smsrequest_FROM on smsrequest( "FROM" );
CREATE INDEX smsrequest_TO on smsrequest( "TO" );
CREATE INDEX smsrequest_PID on smsrequest( "PID" );

CREATE TABLE gateways (
    "gName" TEXT PRIMARY KEY NOT NULL,
    "uName" TEXT NOT NULL,
    "uPass" TEXT NOT NULL,
    "gPort" INTEGER NOT NULL,
    "gPriority" INTEGER NOT NULL,
    "gEnabled" INTEGER NOT NULL,
    "gRule" TEXT,
    "gOptions" TEXT
);
CREATE INDEX gateways_gName on gateways( "gName" );

CREATE TABLE dlrs (
    oid INTEGER PRIMARY KEY,
    smsc TEXT,
    ts TEXT,
    source TEXT,
    destination TEXT,
    service TEXT,
    url TEXT,
    mask TEXT,
    status TEXT,
    boxc TEXT
);
CREATE INDEX dlrs_oid on dlrs( oid );
CREATE INDEX dlrs_smsc on dlrs( smsc );
CREATE INDEX dlrs_ts on dlrs( ts );
CREATE INDEX dlrs_source on dlrs( source );
CREATE INDEX dlrs_destination on dlrs( destination );
CREATE INDEX dlrs_service on dlrs( service );
CREATE INDEX dlrs_url on dlrs( url );
CREATE INDEX dlrs_mask on dlrs( mask );
CREATE INDEX dlrs_status on dlrs( status );
CREATE INDEX dlrs_boxc on dlrs( boxc );

CREATE LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION insert_dlr() RETURNS trigger AS $$
BEGIN
  new.added = NOW();
  RETURN new;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER insert_price_change BEFORE INSERT ON dlrs FOR EACH ROW EXECUTE PROCEDURE insert_dlr();

CREATE TABLE partners (
    pid         TEXT PRIMARY KEY,
    uname       TEXT NOT NULL,
    pass        TEXT NOT NULL,
    adminpass   TEXT DEFAULT '',
    cname       TEXT NOT NULL,
    manager     TEXT NOT NULL,
    phone       TEXT NOT NULL DEFAULT '',
    contact     TEXT NOT NULL DEFAULT '',
    tariff	TEXT NOT NULL DEFAULT '',
    balance     REAL DEFAULT 20,
    credit      REAL DEFAULT 0,
    plimit      INTEGER DEFAULT 60,
    postplay    BOOL DEFAULT FALSE,
    trial       BOOL DEFAULT FALSE,
    priority    INTEGER DEFAULT 0,
    ts          INTEGER DEFAULT 4,
    fname	TEXT NOT NULL DEFAULT '',
    lname	TEXT NOT NULL DEFAULT '',
    mname	TEXT NOT NULL DEFAULT '',
    companyname	TEXT NOT NULL DEFAULT '',
    caddress	TEXT NOT NULL DEFAULT '',
    email	TEXT NOT NULL DEFAULT '',
    ownerid	TEXT DEFAULT 'system'
);

CREATE TABLE mccmnc (
    mcc         INTEGER NOT NULL,
    mnc         INTEGER NOT NULL,
    company     TEXT DEFAULT '',
    name        TEXT DEFAULT ''
);
CREATE INDEX mccmnc_pk on mccmnc( mcc, mnc);

CREATE TABLE countries (
    mcc         INTEGER PRIMARY KEY,
    code        TEXT NOT NULL,
    name        TEXT NOT NULL,
    preffix     INTEGER NOT NULL DEFAULT 0
);

CREATE TABLE preffix_map (
    preffix     INTEGER PRIMARY KEY,
    mcc         INTEGER NOT NULL,
    mnc         INTEGER NOT NULL,
    region      TEXT DEFAULT ''
);

CREATE TABLE tariffs (
    name        TEXT PRIMARY KEY,
    description TEXT DEFAULT '',
    ownerid	TEXT DEFAULT 'system'
);

CREATE TABLE routes (
    name        TEXT PRIMARY KEY,
    description TEXT DEFAULT '',
    ownerid	TEXT DEFAULT 'system'
);

CREATE TABLE matviews (
  mv_name NAME NOT NULL PRIMARY KEY
  , v_name NAME NOT NULL
  , last_refresh TIMESTAMP WITH TIME ZONE
);

CREATE OR REPLACE FUNCTION create_matview(NAME, NAME)
RETURNS VOID
SECURITY DEFINER
LANGUAGE plpgsql AS '
DECLARE
    matview ALIAS FOR $1;
    view_name ALIAS FOR $2;
    entry matviews%ROWTYPE;
BEGIN
    SELECT * INTO entry FROM matviews WHERE mv_name = matview;

    IF FOUND THEN
        RAISE EXCEPTION ''Materialized view ''''%'''' already exists.'',
          matview;
    END IF;

    EXECUTE ''REVOKE ALL ON '' || view_name || '' FROM PUBLIC''; 

    EXECUTE ''GRANT SELECT ON '' || view_name || '' TO PUBLIC'';

    EXECUTE ''CREATE TABLE '' || matview || '' AS SELECT * FROM '' || view_name;

    EXECUTE ''REVOKE ALL ON '' || matview || '' FROM PUBLIC'';

    EXECUTE ''GRANT SELECT ON '' || matview || '' TO PUBLIC'';

    INSERT INTO matviews (mv_name, v_name, last_refresh)
      VALUES (matview, view_name, CURRENT_TIMESTAMP); 
    
    RETURN;
END
';

CREATE OR REPLACE FUNCTION drop_matview(NAME) RETURNS VOID
SECURITY DEFINER
LANGUAGE plpgsql AS '
DECLARE
    matview ALIAS FOR $1;
    entry matviews%ROWTYPE;
BEGIN

    SELECT * INTO entry FROM matviews WHERE mv_name = matview;

    IF NOT FOUND THEN
        RAISE EXCEPTION ''Materialized view % does not exist.'', matview;
    END IF;

    EXECUTE ''DROP TABLE '' || matview;
    DELETE FROM matviews WHERE mv_name=matview;

    RETURN;
END
';

CREATE OR REPLACE FUNCTION ts2int(timestamp without time zone) RETURNS int AS
$$
select extract('epoch' from $1)::integer;
$$ LANGUAGE SQL STRICT STABLE;

CREATE OR REPLACE FUNCTION int2ts(integer) RETURNS timestamp AS
$$
SELECT ( TIMESTAMP WITH TIME ZONE 'epoch' + $1 * INTERVAL '1second')::timestamp without time zone;
$$ LANGUAGE SQL STRICT STABLE;

CREATE TABLE public.table_partitions
(
  master_table text NOT NULL,
  partition_table text NOT NULL,
  range_check text NOT NULL,
  time_added TIMESTAMP DEFAULT now() NOT NULL,
  CONSTRAINT table_partitions_primary_key PRIMARY KEY (master_table, partition_table)
);

CREATE OR REPLACE FUNCTION public.pg_add_range_partition(IN p_master_table text, 
  IN p_partition_table text, IN p_range_check text, IN p_trigger_function text,
  OUT status_code text)
RETURNS text AS
$$
DECLARE
  v_table_ddl text := 'CREATE TABLE [PARTITION_TABLE] ( CHECK ( [RANGE_CHECK] ) ) INHERITS ([MASTER_TABLE]);';
  v_trigger_ddl text := 'CREATE OR REPLACE FUNCTION [TRIGGER_FUNCTION]() RETURNS TRIGGER AS $body$ ' ||
             'BEGIN [RANGE_CHECKS] ELSE RAISE EXCEPTION ' ||
             '''Inserted data is out of range. Fix [TRIGGER_FUNCTION].''; ' ||
             'END IF; RETURN NULL; END; $body$ LANGUAGE plpgsql;';
  v_range_checks text := '';
  rec record;
BEGIN
  IF EXISTS (SELECT 1 FROM public.table_partitions
        WHERE master_table = p_master_table
         AND partition_table = p_partition_table) THEN
    status_code := 'Partition ' || p_partition_table || ' already exists';
    RETURN;
  END IF;

  v_table_ddl := replace(v_table_ddl, '[PARTITION_TABLE]', p_partition_table);
  v_table_ddl := replace(v_table_ddl, '[RANGE_CHECK]', p_range_check);
  v_table_ddl := replace(v_table_ddl, '[MASTER_TABLE]', p_master_table);
  
  FOR rec IN (SELECT 'ELSIF (' || tp.range_check || ') THEN INSERT INTO ' ||
            tp.partition_table || ' VALUES (NEW.*); ' AS range_check
         FROM public.table_partitions tp
         WHERE tp.master_table = p_master_table
        ORDER BY tp.time_added DESC) LOOP
    v_range_checks := _pg_check_to_trigger(p_master_table, rec.range_check) || v_range_checks;
  END LOOP;
  
  v_range_checks := 'IF (' || _pg_check_to_trigger(p_master_table, p_range_check) || 
           ') THEN INSERT INTO ' || p_partition_table || 
           ' VALUES (NEW.*); ' || v_range_checks;
           
  v_trigger_ddl := replace(v_trigger_ddl, '[TRIGGER_FUNCTION]', p_trigger_function);
  v_trigger_ddl := replace(v_trigger_ddl, '[RANGE_CHECKS]', v_range_checks);
  
  RAISE NOTICE 'Partition script: %', v_table_ddl;
  RAISE NOTICE 'Trigger script: %', v_trigger_ddl;
  
  EXECUTE v_table_ddl;
  EXECUTE v_trigger_ddl;
  
  INSERT INTO public.table_partitions (master_table, partition_table, range_check)
     VALUES (p_master_table, p_partition_table, p_range_check);
  
  status_code := 'OK';
  RETURN;
EXCEPTION
WHEN OTHERS THEN
  status_code := 'Unexpected error: ' || SQLERRM;
END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION public._pg_check_to_trigger(IN master_table text, IN range_check text)
RETURNS text AS
$$                                                                                                                          
DECLARE                                         
  v_schema text := COALESCE(SUBSTRING(master_table FROM E'(.*)\\.'),'public');
  v_tablename text := replace(master_table, v_schema || '.', '');
  v_range_check text := range_check;
  rec record;
BEGIN
  RAISE NOTICE '%', v_schema;
  RAISE NOTICE '%', v_tablename;
  FOR rec IN (SELECT column_name
         FROM information_schema.columns
         WHERE table_schema = v_schema
          AND table_name = v_tablename) LOOP
    v_range_check := replace(v_range_check, '"' || rec.column_name || '"', 'NEW.' || '"' || rec.column_name || '"' );
  END LOOP;
  
  RETURN v_range_check;
END;
$$ LANGUAGE 'plpgsql';


CREATE OR REPLACE FUNCTION message_status_partitioning_helper()
RETURNS TRIGGER AS 
$body$ 
BEGIN 
RETURN NULL; 
END; $body$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION message_history_partitioning_helper()
RETURNS TRIGGER AS 
$body$ 
BEGIN 
RETURN NULL; 
END; $body$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION smsrequest_partitioning_helper()
RETURNS TRIGGER AS 
$body$ 
BEGIN 
RETURN NULL; 
END; $body$ LANGUAGE plpgsql;

-- creating partitions for 2011 Jan
SELECT *                                                                                                                 
FROM pg_add_range_partition('message_status',
               'message_status_2011_01',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-01-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-02-01'' )',
               'message_status_partitioning_helper');

SELECT *                                                                                                                 
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2011_01',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-01-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-02-01'' )',
               'smsrequest_partitioning_helper');

SELECT * 
FROM pg_add_range_partition('message_history',
               'message_history_2011_01',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-01-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-02-01'' )',
               'message_history_partitioning_helper');

-- creating partitions for 2011 Feb
SELECT *                                                                                                                 
FROM pg_add_range_partition('message_status',
               'message_status_2011_02',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-02-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-03-01'' )',
               'message_status_partitioning_helper');

SELECT *                                                                                                                 
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2011_02',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-02-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-03-01'' )',
               'smsrequest_partitioning_helper');

SELECT * 
FROM pg_add_range_partition('message_history',
               'message_history_2011_02',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-02-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-03-01'' )',
               'message_history_partitioning_helper');

-- creating partitions for 2011 Mar
SELECT *                                                                                                                 
FROM pg_add_range_partition('message_status',
               'message_status_2011_03',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-03-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-04-01'' )',
               'message_status_partitioning_helper');

SELECT *                                                                                                                 
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2011_03',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-03-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-04-01'' )',
               'smsrequest_partitioning_helper');

SELECT * 
FROM pg_add_range_partition('message_history',
               'message_history_2011_03',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-03-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-04-01'' )',
               'message_history_partitioning_helper');
-- creating partitions for 2011 Apr
SELECT *                                                                                                                 
FROM pg_add_range_partition('message_status',
               'message_status_2011_04',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-04-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-05-01'' )',
               'message_status_partitioning_helper');

SELECT *                                                                                                                 
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2011_04',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-04-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-05-01'' )',
               'smsrequest_partitioning_helper');

SELECT * 
FROM pg_add_range_partition('message_history',
               'message_history_2011_04',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-04-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-05-01'' )',
               'message_history_partitioning_helper');
-- creating partitions for 2011 May
SELECT *                                                                                                                 
FROM pg_add_range_partition('message_status',
               'message_status_2011_05',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-05-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-06-01'' )',
               'message_status_partitioning_helper');

SELECT *                                                                                                                 
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2011_05',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-05-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-06-01'' )',
               'smsrequest_partitioning_helper');

SELECT * 
FROM pg_add_range_partition('message_history',
               'message_history_2011_05',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-05-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-06-01'' )',
               'message_history_partitioning_helper');
-- creating partitions for 2011 Jun
SELECT *                                                                                                                 
FROM pg_add_range_partition('message_status',
               'message_status_2011_06',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-06-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-07-01'' )',
               'message_status_partitioning_helper');

SELECT *                                                                                                                 
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2011_06',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-06-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-07-01'' )',
               'smsrequest_partitioning_helper');

SELECT * 
FROM pg_add_range_partition('message_history',
               'message_history_2011_06',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-06-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-07-01'' )',
               'message_history_partitioning_helper');
-- creating partitions for 2011 Jul
SELECT *                                                                                                                 
FROM pg_add_range_partition('message_status',
               'message_status_2011_07',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-07-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-08-01'' )',
               'message_status_partitioning_helper');

SELECT *                                                                                                                 
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2011_07',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-07-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-08-01'' )',
               'smsrequest_partitioning_helper');

SELECT * 
FROM pg_add_range_partition('message_history',
               'message_history_2011_07',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-07-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-08-01'' )',
               'message_history_partitioning_helper');


--2012
-- creating partitions for 2012 Jan
SELECT *
FROM pg_add_range_partition('message_status',
               'message_status_2012_01',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-01-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-02-01'' )',
               'message_status_partitioning_helper');

SELECT *
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2012_01',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-01-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-02-01'' )',
               'smsrequest_partitioning_helper');

SELECT *
FROM pg_add_range_partition('message_history',
               'message_history_2012_01',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-01-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-02-01'' )',
               'message_history_partitioning_helper');
CREATE INDEX message_status_REQUESTID_2012_01 on message_status_2012_01( "REQUESTID" );
CREATE INDEX message_status_WHEN_2012_01 on message_status_2012_01( "WHEN" );
CREATE INDEX message_status_TO_2012_01 on message_status_2012_01( "TO" );
CREATE INDEX message_status_COUNTRY_2012_01 on message_status_2012_01( "COUNTRY" );
CREATE INDEX message_status_COUNTRYCODE_2012_01 on message_status_2012_01( "COUNTRYCODE" );
CREATE INDEX message_status_OPERATOR_2012_01 on message_status_2012_01( "OPERATOR" );
CREATE INDEX message_status_OPERATORCODE_2012_01 on message_status_2012_01( "OPERATORCODE" );
CREATE INDEX message_status_REGION_2012_01 on message_status_2012_01( "REGION" );
CREATE INDEX message_history_REQUESTID_2012_01 on message_history_2012_01( "REQUESTID" );
CREATE INDEX message_history_WHEN_2012_01 on message_history_2012_01( "WHEN" );
CREATE INDEX message_history_GATEWAY_2012_01 on message_history_2012_01( "GATEWAY" );
CREATE INDEX message_history_WHEN_GATEWAY_2012_01 on message_history_2012_01( "WHEN", "GATEWAY" );
CREATE INDEX smsrequest_REQUESTID_2012_01 on smsrequest_2012_01( "REQUESTID" );
CREATE INDEX smsrequest_WHEN_2012_01 on smsrequest_2012_01( "WHEN" );
CREATE INDEX smsrequest_TXT_2012_01 on smsrequest_2012_01( "TXT" );
CREATE INDEX smsrequest_FROM_2012_01 on smsrequest_2012_01( "FROM" );
CREATE INDEX smsrequest_TO_2012_01 on smsrequest_2012_01( "TO" );
CREATE INDEX smsrequest_PID_2012_01 on smsrequest_2012_01( "PID" );
-- creating partitions for 2012 Feb
SELECT *
FROM pg_add_range_partition('message_status',
               'message_status_2012_02',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-02-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-03-01'' )',
               'message_status_partitioning_helper');

SELECT *
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2012_02',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-02-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-03-01'' )',
               'smsrequest_partitioning_helper');

SELECT *
FROM pg_add_range_partition('message_history',
               'message_history_2012_02',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-02-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-03-01'' )',
               'message_history_partitioning_helper');
CREATE INDEX message_status_REQUESTID_2012_01 on message_status_2012_02( "REQUESTID" );
CREATE INDEX message_status_WHEN_2012_02 on message_status_2012_02( "WHEN" );
CREATE INDEX message_status_TO_2012_02 on message_status_2012_02( "TO" );
CREATE INDEX message_status_COUNTRY_2012_02 on message_status_2012_02( "COUNTRY" );
CREATE INDEX message_status_COUNTRYCODE_2012_02 on message_status_2012_02( "COUNTRYCODE" );
CREATE INDEX message_status_OPERATOR_2012_02 on message_status_2012_02( "OPERATOR" );
CREATE INDEX message_status_OPERATORCODE_2012_02 on message_status_2012_02( "OPERATORCODE" );
CREATE INDEX message_status_REGION_2012_02 on message_status_2012_02( "REGION" );
CREATE INDEX message_history_REQUESTID_2012_02 on message_history_2012_02( "REQUESTID" );
CREATE INDEX message_history_WHEN_2012_02 on message_history_2012_02( "WHEN" );
CREATE INDEX message_history_GATEWAY_2012_02 on message_history_2012_02( "GATEWAY" );
CREATE INDEX message_history_WHEN_GATEWAY_2012_02 on message_history_2012_02( "WHEN", "GATEWAY" );
CREATE INDEX smsrequest_REQUESTID_2012_02 on smsrequest_2012_02( "REQUESTID" );
CREATE INDEX smsrequest_WHEN_2012_02 on smsrequest_2012_02( "WHEN" );
CREATE INDEX smsrequest_TXT_2012_02 on smsrequest_2012_02( "TXT" );
CREATE INDEX smsrequest_FROM_2012_02 on smsrequest_2012_02( "FROM" );
CREATE INDEX smsrequest_TO_2012_02 on smsrequest_2012_02( "TO" );
CREATE INDEX smsrequest_PID_2012_02 on smsrequest_2012_02( "PID" );
-- creating partitions for 2011 Mar
SELECT *
FROM pg_add_range_partition('message_status',
               'message_status_2011_03',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-03-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-04-01'' )',
               'message_status_partitioning_helper');

SELECT *
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2011_03',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-03-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-04-01'' )',
               'smsrequest_partitioning_helper');

SELECT *
FROM pg_add_range_partition('message_history',
               'message_history_2011_03',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-03-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-04-01'' )',
               'message_history_partitioning_helper');
CREATE INDEX message_status_REQUESTID_2012_03 on message_status_2012_03( "REQUESTID" );
CREATE INDEX message_status_WHEN_2012_03 on message_status_2012_03( "WHEN" );
CREATE INDEX message_status_TO_2012_03 on message_status_2012_03( "TO" );
CREATE INDEX message_status_COUNTRY_2012_03 on message_status_2012_03( "COUNTRY" );
CREATE INDEX message_status_COUNTRYCODE_2012_03 on message_status_2012_03( "COUNTRYCODE" );
CREATE INDEX message_status_OPERATOR_2012_03 on message_status_2012_03( "OPERATOR" );
CREATE INDEX message_status_OPERATORCODE_2012_03 on message_status_2012_03( "OPERATORCODE" );
CREATE INDEX message_status_REGION_2012_03 on message_status_2012_03( "REGION" );
CREATE INDEX message_history_REQUESTID_2012_03 on message_history_2012_03( "REQUESTID" );
CREATE INDEX message_history_WHEN_2012_03 on message_history_2012_03( "WHEN" );
CREATE INDEX message_history_GATEWAY_2012_03 on message_history_2012_03( "GATEWAY" );
CREATE INDEX message_history_WHEN_GATEWAY_2012_03 on message_history_2012_03( "WHEN", "GATEWAY" );
CREATE INDEX smsrequest_REQUESTID_2012_03 on smsrequest_2012_03( "REQUESTID" );
CREATE INDEX smsrequest_WHEN_2012_03 on smsrequest_2012_03( "WHEN" );
CREATE INDEX smsrequest_TXT_2012_03 on smsrequest_2012_03( "TXT" );
CREATE INDEX smsrequest_FROM_2012_03 on smsrequest_2012_03( "FROM" );
CREATE INDEX smsrequest_TO_2012_03 on smsrequest_2012_03( "TO" );
CREATE INDEX smsrequest_PID_2012_03 on smsrequest_2012_03( "PID" );
-- creating partitions for 2011 Apr
SELECT *
FROM pg_add_range_partition('message_status',
               'message_status_2011_04',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-04-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-05-01'' )',
               'message_status_partitioning_helper');

SELECT *
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2011_04',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-04-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-05-01'' )',
               'smsrequest_partitioning_helper');

SELECT *
FROM pg_add_range_partition('message_history',
               'message_history_2011_04',
               '"WHEN" >= ts2int( TIMESTAMP ''2011-04-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2011-05-01'' )',
               'message_history_partitioning_helper');
CREATE INDEX message_status_REQUESTID_2012_04 on message_status_2012_04( "REQUESTID" );
CREATE INDEX message_status_WHEN_2012_04 on message_status_2012_04( "WHEN" );
CREATE INDEX message_status_TO_2012_04 on message_status_2012_04( "TO" );
CREATE INDEX message_status_COUNTRY_2012_04 on message_status_2012_04( "COUNTRY" );
CREATE INDEX message_status_COUNTRYCODE_2012_04 on message_status_2012_04( "COUNTRYCODE" );
CREATE INDEX message_status_OPERATOR_2012_04 on message_status_2012_04( "OPERATOR" );
CREATE INDEX message_status_OPERATORCODE_2012_04 on message_status_2012_04( "OPERATORCODE" );
CREATE INDEX message_status_REGION_2012_04 on message_status_2012_04( "REGION" );
CREATE INDEX message_history_REQUESTID_2012_04 on message_history_2012_04( "REQUESTID" );
CREATE INDEX message_history_WHEN_2012_04 on message_history_2012_04( "WHEN" );
CREATE INDEX message_history_GATEWAY_2012_04 on message_history_2012_04( "GATEWAY" );
CREATE INDEX message_history_WHEN_GATEWAY_2012_04 on message_history_2012_04( "WHEN", "GATEWAY" );
CREATE INDEX smsrequest_REQUESTID_2012_04 on smsrequest_2012_04( "REQUESTID" );
CREATE INDEX smsrequest_WHEN_2012_04 on smsrequest_2012_04( "WHEN" );
CREATE INDEX smsrequest_TXT_2012_04 on smsrequest_2012_04( "TXT" );
CREATE INDEX smsrequest_FROM_2012_04 on smsrequest_2012_04( "FROM" );
CREATE INDEX smsrequest_TO_2012_04 on smsrequest_2012_04( "TO" );
CREATE INDEX smsrequest_PID_2012_04 on smsrequest_2012_04( "PID" );

-- creating partitions for 2012 May
SELECT *
FROM pg_add_range_partition('message_status',
               'message_status_2012_05',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-05-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-06-01'' )',
               'message_status_partitioning_helper');

SELECT *
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2012_05',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-05-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-06-01'' )',
               'smsrequest_partitioning_helper');

SELECT *
FROM pg_add_range_partition('message_history',
               'message_history_2012_05',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-05-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-06-01'' )',
               'message_history_partitioning_helper');
CREATE INDEX message_status_REQUESTID_2012_05 on message_status_2012_05( "REQUESTID" );
CREATE INDEX message_status_WHEN_2012_05 on message_status_2012_05( "WHEN" );
CREATE INDEX message_status_TO_2012_05 on message_status_2012_05( "TO" );
CREATE INDEX message_status_COUNTRY_2012_05 on message_status_2012_05( "COUNTRY" );
CREATE INDEX message_status_COUNTRYCODE_2012_05 on message_status_2012_05( "COUNTRYCODE" );
CREATE INDEX message_status_OPERATOR_2012_05 on message_status_2012_05( "OPERATOR" );
CREATE INDEX message_status_OPERATORCODE_2012_05 on message_status_2012_05( "OPERATORCODE" );
CREATE INDEX message_status_REGION_2012_05 on message_status_2012_05( "REGION" );
CREATE INDEX message_history_REQUESTID_2012_05 on message_history_2012_05( "REQUESTID" );
CREATE INDEX message_history_WHEN_2012_05 on message_history_2012_05( "WHEN" );
CREATE INDEX message_history_GATEWAY_2012_05 on message_history_2012_05( "GATEWAY" );
CREATE INDEX message_history_WHEN_GATEWAY_2012_05 on message_history_2012_05( "WHEN", "GATEWAY" );
CREATE INDEX smsrequest_REQUESTID_2012_05 on smsrequest_2012_05( "REQUESTID" );
CREATE INDEX smsrequest_WHEN_2012_05 on smsrequest_2012_05( "WHEN" );
CREATE INDEX smsrequest_TXT_2012_05 on smsrequest_2012_05( "TXT" );
CREATE INDEX smsrequest_FROM_2012_05 on smsrequest_2012_05( "FROM" );
CREATE INDEX smsrequest_TO_2012_05 on smsrequest_2012_05( "TO" );
CREATE INDEX smsrequest_PID_2012_05 on smsrequest_2012_05( "PID" );

-- creating partitions for 2012 Jun
SELECT *
FROM pg_add_range_partition('message_status',
               'message_status_2012_06',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-06-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-07-01'' )',
               'message_status_partitioning_helper');

SELECT *
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2012_06',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-06-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-07-01'' )',
               'smsrequest_partitioning_helper');

SELECT *
FROM pg_add_range_partition('message_history',
               'message_history_2012_06',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-06-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-07-01'' )',
               'message_history_partitioning_helper');
CREATE INDEX message_status_REQUESTID_2012_06 on message_status_2012_06( "REQUESTID" );
CREATE INDEX message_status_WHEN_2012_06 on message_status_2012_06( "WHEN" );
CREATE INDEX message_status_TO_2012_06 on message_status_2012_06( "TO" );
CREATE INDEX message_status_COUNTRY_2012_06 on message_status_2012_06( "COUNTRY" );
CREATE INDEX message_status_COUNTRYCODE_2012_06 on message_status_2012_06( "COUNTRYCODE" );
CREATE INDEX message_status_OPERATOR_2012_06 on message_status_2012_06( "OPERATOR" );
CREATE INDEX message_status_OPERATORCODE_2012_06 on message_status_2012_06( "OPERATORCODE" );
CREATE INDEX message_status_REGION_2012_06 on message_status_2012_06( "REGION" );
CREATE INDEX message_history_REQUESTID_2012_06 on message_history_2012_06( "REQUESTID" );
CREATE INDEX message_history_WHEN_2012_06 on message_history_2012_06( "WHEN" );
CREATE INDEX message_history_GATEWAY_2012_06 on message_history_2012_06( "GATEWAY" );
CREATE INDEX message_history_WHEN_GATEWAY_2012_06 on message_history_2012_06( "WHEN", "GATEWAY" );
CREATE INDEX smsrequest_REQUESTID_2012_06 on smsrequest_2012_06( "REQUESTID" );
CREATE INDEX smsrequest_WHEN_2012_06 on smsrequest_2012_06( "WHEN" );
CREATE INDEX smsrequest_TXT_2012_06 on smsrequest_2012_06( "TXT" );
CREATE INDEX smsrequest_FROM_2012_06 on smsrequest_2012_06( "FROM" );
CREATE INDEX smsrequest_TO_2012_06 on smsrequest_2012_06( "TO" );
CREATE INDEX smsrequest_PID_2012_06 on smsrequest_2012_06( "PID" );
-- creating partitions for 2012 Jul
SELECT *
FROM pg_add_range_partition('message_status',
               'message_status_2012_07',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-07-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-08-01'' )',
               'message_status_partitioning_helper');

SELECT *
FROM pg_add_range_partition('smsrequest',
               'smsrequest_2012_07',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-07-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-08-01'' )',
               'smsrequest_partitioning_helper');

SELECT *
FROM pg_add_range_partition('message_history',
               'message_history_2012_07',
               '"WHEN" >= ts2int( TIMESTAMP ''2012-07-01'' ) AND "WHEN" < ts2int( TIMESTAMP ''2012-08-01'' )',
               'message_history_partitioning_helper');

CREATE INDEX message_status_REQUESTID_2012_07 on message_status_2012_07( "REQUESTID" );
CREATE INDEX message_status_WHEN_2012_07 on message_status_2012_07( "WHEN" );
CREATE INDEX message_status_TO_2012_07 on message_status_2012_07( "TO" );
CREATE INDEX message_status_COUNTRY_2012_07 on message_status_2012_07( "COUNTRY" );
CREATE INDEX message_status_COUNTRYCODE_2012_07 on message_status_2012_07( "COUNTRYCODE" );
CREATE INDEX message_status_OPERATOR_2012_07 on message_status_2012_07( "OPERATOR" );
CREATE INDEX message_status_OPERATORCODE_2012_07 on message_status_2012_07( "OPERATORCODE" );
CREATE INDEX message_status_REGION_2012_07 on message_status_2012_07( "REGION" );
CREATE INDEX message_history_REQUESTID_2012_07 on message_history_2012_07( "REQUESTID" );
CREATE INDEX message_history_WHEN_2012_07 on message_history_2012_07( "WHEN" );
CREATE INDEX message_history_GATEWAY_2012_07 on message_history_2012_07( "GATEWAY" );
CREATE INDEX message_history_WHEN_GATEWAY_2012_07 on message_history_2012_07( "WHEN", "GATEWAY" );
CREATE INDEX smsrequest_REQUESTID_2012_07 on smsrequest_2012_07( "REQUESTID" );
CREATE INDEX smsrequest_WHEN_2012_07 on smsrequest_2012_07( "WHEN" );
CREATE INDEX smsrequest_TXT_2012_07 on smsrequest_2012_07( "TXT" );
CREATE INDEX smsrequest_FROM_2012_07 on smsrequest_2012_07( "FROM" );
CREATE INDEX smsrequest_TO_2012_07 on smsrequest_2012_07( "TO" );
CREATE INDEX smsrequest_PID_2012_07 on smsrequest_2012_07( "PID" );

-- enable partitioning
CREATE TRIGGER message_status_partitioning_trigger
 BEFORE INSERT ON message_status
 FOR EACH ROW EXECUTE PROCEDURE message_status_partitioning_helper();

CREATE TRIGGER message_history_partitioning_trigger
 BEFORE INSERT ON message_history
 FOR EACH ROW EXECUTE PROCEDURE message_history_partitioning_helper();

CREATE TRIGGER smsrequest_partitioning_trigger
 BEFORE INSERT ON smsrequest
 FOR EACH ROW EXECUTE PROCEDURE smsrequest_partitioning_helper();

--disable partitioning
/*
drop trigger message_status_partitioning_trigger on message_status;
drop trigger smsrequest_partitioning_trigger on smsrequest;
drop trigger message_history_partitioning_trigger on message_history;
*/


