# Deploying pypy on uwsgi

Instructions are kind of sporadic around the internet, so I thought I would gather them all in one place.  The following example uses a Flask app as the thing deployed. You should only have to this once, and then you can pass the bin around.

## Requirements

* pypy 2.x (tested)
* uwsgi 1.9.11+ (trunk as of this gist)

## Build translated pypy binary and pypy lib.

Following the instructions on https://github.com/unbit/uwsgi/tree/master/plugins/pypy:

* get pypy source code (https://bitbucket.org/pypy/pypy) and checkout the proper prod branch
* install standard python binary for your system (http://doc.pypy.org/en/latest/getting-started.html#installing-pypy)
* run the following command (took 1.47 hours on AWS c1.xlarge, there must be a way to make it run faster):

```bash
./rpython/bin/rpython -Ojit --shared --gcrootfinder=shadowstack pypy/goal/targetpypystandalone
```

Once it's complete:

* copy the lib file (.so) and pypy-c into your pypy directory. 
* make sure the lib and bin are on your path (for ex, symlinks: libpypy.so -> /usr/lib; pypy-c -> /usr/local/bin)

## Build uwsgi

Get uwsgi.

```bash
git clone https://github.com/unbit/uwsgi.git
```

Create a core file that has what you want, or use a default.  I used ``buildconf/default.ini``. I also added a parameter to that file that specifies the uwsgi lib: ``plugin_dir = /usr/lib/uwsgi`` (file attached as uwsgi_pypy_config.ini).

```bash
python uwsgiconfig.py --build default
```

Then add the pypy plugin (You can also do this in the ini file).

```bash
python uwsgiconfig.py --plugin plugins/pypy default
```

This should place it into the ``plugin_dir`` folder.

## Run

Then, you can run ./uwsgi against an app to make certain it works.  Keep in mind, that the pypy plugin changes parameters.

The module/callable is combined in a parameter called ``pypy-uwsgi``.

You can also set uwsgi against a virtualenv home directory.  That parameter is now ``pypy-home``.

So the entire thing:

```bash
./uwsgi --plugin pypy --pypy-home /path/to/my/venv --http :8080 --pypy-wsgi module:callable
```

A couple of notes:

* Unlike normal python applications, this will say ``*** no app loaded. going in full dynamic mode ***``. It's lying, it actually has loaded the app.  I'm assuming this is a bug, will look into it at some point.
* You can tell that pypy is running because you will see something like this in your log: ``Initialized PyPy with Python 2.7.3 (blahblahbahl) \n [PyPy 2.1.0-alpha0 with GCC 4.6.3]``
* can't seem to get uwsgi to recognize ``uwsgimodifier1``, which allows you to pass a ``SCRIPT_NAME`` parameter from nginx that uwsgi drops before sending the request.  In fact, I can only seem to get it to run on an http socket.  Not sure if something I'm doing wrong or if is a bug. 
