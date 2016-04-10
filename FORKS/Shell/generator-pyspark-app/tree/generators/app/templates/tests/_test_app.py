# -*- coding: utf-8 -*-

import os
import unittest

from <%= pkgName %>.main import <%= className %>App


class <%= className %>AppTest(unittest.TestCase):

    def setUp(self):
        os.putenv('SPARK_MASTER', 'local')

    def test_run(self):
        <%= className %>App.run()
