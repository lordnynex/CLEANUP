# -*- coding: utf-8 -*-

from setuptools import setup, find_packages

setup(
    name='<%= pkgName %>-pyspark-app',
    version='0.0.1',
    description='<%= appName %> Spark application',
    packages=find_packages(exclude=['tests', '.packages']),
    tests_require=['nose'],
)
