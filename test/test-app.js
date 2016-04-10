'use strict';

var path = require('path');
var assert = require('yeoman-generator').assert;
var helpers = require('yeoman-generator').test;
var os = require('os');

describe('pyspark-app:app', function () {
  before(function (done) {
    helpers.run(path.join(__dirname, '../generators/app'))
      .withOptions({ skipInstall: true })
      .withPrompts({ appName: 'Word Count' })
      .on('end', done);
  });

  it('creates files', function () {
    assert.file([
      '.gitignore',
      'README.md',
      'bin/run',
      'bin/test',
      'samples/.gitkeep',
      'setup.py',
      'spark.json',
      'tests/__init__.py',
      'tests/test_app.py',
      'wordcount/__init__.py',
      'wordcount/main.py'
    ]);
  });
});
