# -*- coding: utf-8 -*-

import os

from pyspark import SparkConf, SparkContext

conf = SparkConf()


class <%= className %>App(object):

    @classmethod
    def init(cls):
        conf.setAppName('<%= appName %>')
        # Set master if spark.master isn't provided and SPARK_MASTER isn't null
        if not conf.contains('spark.master') and os.getenv('SPARK_MASTER'):
            conf.setMaster(os.getenv('SPARK_MASTER'))

        # If running Spark application in local mode
        cls.LOCAL_MODE = True if conf.get('spark.master').startswith('local') else False  # noqa

        # Set your own class attributes below, e.g.
        # cls.MAX_LEN = 1

        return cls

    @classmethod
    def run(cls):
        with SparkContext(conf=conf) as sc:
            if cls.LOCAL_MODE:
                # If in local mode, load data from local path, e.g.
                # input_file = sc.textFile('samples/data.csv')
                pass
            else:
                # Load data from HDFS
                # input_file = sc.textFile('hdfs:///user/xxx/data.csv')
                pass

            # Write your code here
            pass

# Do NOT move this line.
<%= className %>App.init()

if __name__ == "__main__":
    <%= className %>App.run()
