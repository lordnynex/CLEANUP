# Example taken from the http://gunicorn.org/configure.html
# page.

import os

def numCPUs():
  if not hasattr(os, "sysconf"):
    raise RuntimeError("No sysconf detected.")
  return os.sysconf("SC_NPROCESSORS_ONLN")

bind = "127.0.0.1:8000"
workers = numCPUs() * 2 + 1

