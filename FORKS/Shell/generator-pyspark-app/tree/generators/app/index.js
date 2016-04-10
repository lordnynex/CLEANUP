'use strict';
var yeoman = require('yeoman-generator');
var chalk = require('chalk');
var yosay = require('yosay');
var _s = require('underscore.string');

module.exports = yeoman.generators.Base.extend({
  prompting: function () {
    var done = this.async();

    // Have Yeoman greet the user.
    this.log(yosay(
      'Welcome to the awe-inspiring ' + chalk.red('pyspark-app') + ' generator!'
    ));

    var prompts = [{
      name: 'appName',
      message: 'What\'s the name of your Apache Spark application? (e.g. Word Count)',
      validate: function (appName) {
        if (appName.match(/[^a-zA-Z0-9 ]/) !== null) {
          return 'Only use letters, numbers and spaces in your application name';
        } else {
          return true;
        }
      }
    }];

    this.prompt(prompts, function (props) {
      this.appName = _s.capitalize(props.appName);
      this.pkgName = _s(props.appName).toLowerCase().replaceAll(' ', '').value();
      this.className = _s.classify(props.appName);
      done();
    }.bind(this));
  },

  writing: {
    projectfiles: function () {
      this.fs.copyTpl(
        this.templatePath('_README.md'),
        this.destinationPath('README.md'),
        this
      );
      this.fs.copyTpl(
        this.templatePath('_setup.py'),
        this.destinationPath('setup.py'),
        this
      );
      this.fs.copyTpl(
        this.templatePath('_spark.json'),
        this.destinationPath('spark.json'),
        this
      );
    },

    gitfiles: function () {
      this.fs.copy(
        this.templatePath('gitignore'),
        this.destinationPath('.gitignore')
      );
    },

    app: function () {
      this.fs.copy(
        this.templatePath('_/__init__.py'),
        this.destinationPath(this.pkgName, '__init__.py')
      );
      this.fs.copyTpl(
        this.templatePath('_/_main.py'),
        this.destinationPath(this.pkgName, 'main.py'),
        this
      );
      this.fs.copy(
        this.templatePath('bin/run'),
        this.destinationPath('bin/run')
      );
      this.fs.copy(
        this.templatePath('bin/test'),
        this.destinationPath('bin/test')
      );
      this.fs.copy(
        this.templatePath('samples/gitkeep'),
        this.destinationPath('samples/.gitkeep')
      );
      this.fs.copy(
        this.templatePath('tests/__init__.py'),
        this.destinationPath('tests/__init__.py')
      );
      this.fs.copyTpl(
        this.templatePath('tests/_test_app.py'),
        this.destinationPath('tests/test_app.py'),
        this
      );
    }
  },

  end: function () {
    this.log('Read useful information in README.md, then edit ' + this.pkgName + '/main.py to start your Spark journey!');
  }
});
