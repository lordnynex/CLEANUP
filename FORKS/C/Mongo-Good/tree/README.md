## This repo is deprecated. Please check [https://bitbucket.org/cojennin/mongo-good](https://bitbucket.org/cojennin/mongo-good) for further updates.

# Mongo-Good
==========
An NGINX extension to do good.

## Installation
To intsall, download the NGINX source code: http://nginx.org/en/download.html.

Download and install the MongoDB C Driver: https://github.com/mongodb/mongo-c-driver

The MongoDB C Driver has a dependency on libbson, so you'll also want to download and install that: https://github.com/mongodb/libbson/

Once you've got and installed the above, note the following -I flags below.

Now clone this extension and put it somewhere you'll rememeber.

To install this extension with NGINX:
From within the NGINX source directory, run the following (an example is provided in this extensions config file for reference):
CFLAGS="-g -O0 -I/path/to/opt/libbson/src/bson -I/path/to/opt/mongo-c-driver-0.94.0/src/mongoc" ./configure --add-module=/path/to/module/Mongo-Good --with-ld-opt="-lbson-1.0 -lmongoc-1.0" --with-debug

(--with-debug optional).

To finish up run make, then make install.

You should be all set! (by default, I believe nginx is installed to /usr/local/share. Can change that at your lesiure).

Example URL: http://localhost/mongo?limit=1&offset=2&q=field:valueToSearch

Good luck!
