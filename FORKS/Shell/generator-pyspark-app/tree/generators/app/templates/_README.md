# <%= appName %> Spark Application

This is an [Apache Spark](https://spark.apache.org) application using Python API.


## Bootstrap

You need install Apache Spark, [pip](https://pip.pypa.io) and [nose](http://readthedocs.org/docs/nose) first.

Then set `SPARK_HOME` environment variable, e.g.

```bash
$ export SPARK_HOME=/usr/local/Cellar/apache-spark/1.4.1/libexec
```


## Running Spark application in local mode

```bash
$ bin/run local
```


## Running Spark application in YARN cluster mode

In production environment, we use [Azkaban](http://azkaban.github.io) as
Spark workflow manager. You should install [Azkaban CLI tool](https://github.com/mtth/azkaban) first.

```bash
$ pip install azkaban
```

Then edit `~/.azkabanrc` file, replace `USERNAME` and `PASSWORD` with yours.

```bash
$ cat ~/.azkabanrc
[alias]
azkaban = https://USERNAME:PASSWORD@azkaban.example.com:443

[azkaban]
default.alias = azkaban
```

Ensure correctly set configurations in `spark.json` file, e.g. `hdfsUsername`, `failureEmails`.
For detail about `spark.json`, please see below
[spark.json](#spark-json) setion.

Finally, ship the application.

```bash
$ bin/run yarn
```

### Schedule

You can schedule an application, just set `schedule.startDate`,
`schedule.startTime` and `schedule.period` in `spark.json`, for example:

```
{
  "appName": "xxx",
  "appEntryPoint": "xxx/main.py",
  "hdfsUsername": "xxx",
  "failureEmails": "xxx@example.com",
  "successEmails": "xxx@example.com",
  "schedule": {
    "startDate": "06/23/2015",
    "startTime": "03,15,AM,UTC",
    "period": "1d"
  }
}
```

Then add `--schedule` option when run the application.

```bash
$ bin/run yarn --schedule
```

If you wanna remove schedule, go to the `/schedule` page in Azkaban UI.


## Running test

```
$ bin/test
```


## `spark.json`

`spark.json` defines several useful configurations for running Spark
application, here is the list of all supported configurations.

| Name | Description |
| ---- | ----------- |
| `appName` | Application name, no space allowed |
| `appEntryPoint` | Python file as the main program, a.k.a. in Spark world it be called driver program |
| `extraSparkSubmitOptions` | Extra `spark-submit` options |
| `hdfsUsername` (YARN only) | Your username on HDFS, it is used for HDFS authentication |
| `failureEmails` (YARN only) | Comma delimited list of emails to notify during a failure |
| `successEmails` (YARN only) | Comma delimited list of emails to notify during a success |
| `schedule.startDate` (YARN only) | Date used for first run of a schedule. It must be in the format `MM/DD/YYYY`. |
| `schedule.startTime` (YARN only) | Time when a schedule should be run. Must be of the format `hh,mm,(AM or PM),(PDT or UTC)`. |
| `schedule.period` (YARN only) | Period to repeat the scheduled flow. Must be in format `1d`, a combination of magnitude and unit of repetition. Possible values: `M` (month), `w` (week), `d` (day), `h` (hour), `m` (minute), `s` (second). |
