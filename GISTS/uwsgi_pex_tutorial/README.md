Building a test pex
-------------------

setup.py

    yakuza:src kw$ cat testapp/setup.py
    #!/usr/bin/env python

    from distutils.core import setup

    setup(
      name='testapp',
      version='1.0',
      description='Test',
      author='Test Test',
      author_email='test@test.com',
      packages=['testapp']
    )

testapp (note the arbitrary pystachio import which exists as a dep only in this pex)

    yakuza:src kw$ cat testapp/testapp/__init__.py
    import sys

    from flask import Flask
    import pystachio

    app = Flask(__name__)

    @app.route('/')
    def hello_world():
      return 'Hello World!\n%s\n' % pystachio.__file__

    def main():
      sys.stdout.write('running flask dev server via main()\n')
      app.run()

create the pex

    yakuza:src kw$ pex -r flask -r pystachio -s testapp -e testapp:main -o testapp.pex

test the pex, invoking main()

    yakuza:src kw$ ./testapp.pex
    running flask dev server via main()
     * Running on http://127.0.0.1:5000/
    ^C

test imports from the pex. note the print statement is not called as we don't go through main(); this level of wsgi-callable import from the pex is essentially what we want to achieve in uwsgi.

    yakuza:src kw$ PEX_MODULE='' ./testapp.pex
    Python 2.7.5 (default, Aug 25 2013, 00:04:04)
    [GCC 4.2.1 Compatible Apple LLVM 5.0 (clang-500.0.68)] on darwin
    Type "help", "copyright", "credits" or "license" for more information.
    (InteractiveConsole)
    >>> from testapp import app
    >>> app.run()
     * Running on http://127.0.0.1:5000/
    ^D

Bootstrapping uWSGI
-------------------

revised bootstrap.py

    yakuza:src kw$ cat bootstrap.py
    import os
    import sys

    def activate_pex():
      entry_point = os.environ.get('UWSGI_PEX')
      if not entry_point:
        sys.stderr.write('couldnt determine pex from UWSGI_PEX environment variable, bailing!\n')
        sys.exit(1)

      sys.stderr.write('entry_point=%s\n' % entry_point)

      sys.path[0] = os.path.abspath(sys.path[0])
      sys.path.insert(0, entry_point)
      sys.path.insert(0, os.path.abspath(os.path.join(entry_point, '.bootstrap')))

      from _twitter_common_python import pex_bootstrapper
      from _twitter_common_python.environment import PEXEnvironment
      from _twitter_common_python.finders import register_finders
      from _twitter_common_python.pex_info import PexInfo

      pex_bootstrapper.monkeypatch_build_zipmanifest()
      register_finders()

      pex_info = PexInfo.from_pex(entry_point)
      env = PEXEnvironment(entry_point, pex_info)
      working_set = env.activate()

      sys.stderr.write('sys.path=%s\n\n' % sys.path)

      return entry_point, pex_info, env, working_set

    activate_pex()

running the POC. the --import bootstrap.py will cause uWSGI workers to run bootstrap.py (and thus bootstrap their PEX environment) before the main wsgi module is loaded.

    yakuza:src kw$ ~/bin/uwsgi --http :9090 -p 4 --import bootstrap.py --env UWSGI_PEX=testapp.pex --module 'testapp:app'
    *** Starting uWSGI 2.0.4 (64bit) on [Sun May  4 04:28:42 2014] ***
    ...
    entry_point=testapp.pex
    ...
    WSGI app 0 (mountpoint='') ready in 0 seconds on interpreter 0x7fbec9406fb0 pid: 4021 (default app)
    *** uWSGI is running in multiple interpreter mode ***
    spawned uWSGI worker 1 (pid: 4021, cores: 1)
    spawned uWSGI worker 2 (pid: 4025, cores: 1)
    spawned uWSGI worker 3 (pid: 4026, cores: 1)
    spawned uWSGI worker 4 (pid: 4027, cores: 1)

Embed bootstrap.py into a custom uWSGI bin
-------------------------------------------------

on linux, you can take this one step further by embedding the bootstrap.py  (and the --import argument to invoke this) into the uwsgi binary for single-file deployability:

    [kwilson@nest uwsgi-2.0.4]$ cat buildconf/pex.ini
    [uwsgi]
    main_plugin = python,gevent
    inherit = base
    bin_name = pex_uwsgi
    embed_files = bootstrap.py
    embed_config = uwsgipex.ini

    [kwilson@nest uwsgi-2.0.4]$ cat uwsgipex.ini
    [uwsgi]
    import = sym://bootstrap_py

embedding bootstrap.py and our default --import parameter into the uwsgi build

    [kwilson@nest uwsgi-2.0.4]$ python2.6 uwsgiconfig.py --build pex
    using profile: buildconf/pex.ini
    detected include path: ['/usr/local/include', '/usr/lib/gcc/x86_64-redhat-linux/4.1.2/include', '/usr/include']
    ld -r -b binary -o uwsgipex_ini.o uwsgipex.ini
    ld -r -b binary -o bootstrap_py.o bootstrap.py
    ...
    *** uWSGI compiling server core ***
    ...
    *** uWSGI linking ***
    ...
    ############## end of uWSGI configuration #############
    total build time: 3 seconds
    *** uWSGI is ready, launch it with ./pex_uwsgi ***

    [kwilson@nest uwsgi-2.0.4]$ cp pex_uwsgi ~/bin/

and voila, a generic pex-bootstrapped uwsgi binary that can run any app/module in a given pex:

    [kwilson@nest src]$ ~/bin/pex_uwsgi --http :8585 --env UWSGI_PEX=testapp.pex --module 'testapp:app'
    *** Starting uWSGI 2.0.4 (64bit) on [Sun May  4 20:37:42 2014] ***
    ...
    entry_point=testapp.pex
    sys.path=['/home/kwilson/uwsgipex/src/testapp.pex/.bootstrap', '/home/kwilson/uwsgipex/src/testapp.pex', '.', '', '/usr/lib64/python26.zip', '/usr/lib64/python2.6', '/usr/lib64/python2.6/plat-linux2', '/usr/lib64/python2.6/lib-tk', '/usr/lib64/python2.6/lib-old', '/usr/lib64/python2.6/lib-dynload', '/usr/lib64/python2.6/site-packages', '/usr/lib64/python2.6/site-packages/PIL', '/usr/lib/python2.6/site-packages', '/usr/lib/python2.6/site-packages/setuptools-0.6c11-py2.6.egg-info', u'/home/kwilson/.pex/install/testapp-1.0-py2-none-any.whl.e4fb1d76d0b5acc7469b9131599df348d001e485/testapp-1.0-py2-none-any.whl', u'/home/kwilson/.pex/install/MarkupSafe-0.21-py2-none-any.whl.e893c031b3264a1b2bf4a181ec3c37c5c29c79d4/MarkupSafe-0.21-py2-none-any.whl', u'/home/kwilson/.pex/install/pystachio-0.7.2-py2-none-any.whl.e5dddb66aee457ea98cc4dc757c760eb19718766/pystachio-0.7.2-py2-none-any.whl', u'/home/kwilson/.pex/install/itsdangerous-0.24-py2-none-any.whl.b6cea9c5e263ffc66647552106a1880f3500447d/itsdangerous-0.24-py2-none-any.whl', u'/home/kwilson/.pex/install/Flask-0.10.1-py2-none-any.whl.e414a0ef2f868c6cb7428e8f5bef385fb00f3ca3/Flask-0.10.1-py2-none-any.whl', u'/home/kwilson/.pex/install/Jinja2-2.7.2-py2-none-any.whl.d141c60e06b1c05b0d31901fde68a50305ff6ed0/Jinja2-2.7.2-py2-none-any.whl', u'/home/kwilson/.pex/install/Werkzeug-0.9.4-py2-none-any.whl.83a9a2dd0dfe3eb61ffc972f2dda175e7f45b667/Werkzeug-0.9.4-py2-none-any.whl']
    ...
    WSGI app 0 (mountpoint='') ready in 1 seconds on interpreter 0x1abf220 pid: 48862 (default app)
    *** uWSGI is running in multiple interpreter mode ***
    spawned uWSGI worker 1 (and the only) (pid: 48862, cores: 1)


    [kwilson@nest ~]$ curl http://localhost:8585
    Hello World!
    /home/kwilson/.pex/install/pystachio-0.7.2-py2-none-any.whl.e5dddb66aee457ea98cc4dc757c760eb19718766/pystachio-0.7.2-py2-none-any.whl/pystachio/__init__.py

note that the uWSGI embedding process will silently fail on OSX tho due to lack of support for embedding on non-linux platforms.
