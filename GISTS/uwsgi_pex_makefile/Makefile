PYTHON := python2.6
BUILD_DIR := buildtmp

AURORA_CLUSTER := test
AURORA_ROLE := $(shell whoami)

UWSGI_VER = 2.0.4
UWSGI_DIR = uwsgi-$(UWSGI_VER)
UWSGI_FILE = $(UWSGI_DIR).tar.gz
UWSGI_URL = http://projects.unbit.it/downloads/$(UWSGI_FILE)

UWSGI_BUILD_NAME = pex_uwsgi
UWSGI_BUILD_INI_TARGET = buildconf/$(UWSGI_BUILD_NAME).ini
UWSGI_OPTS_INI_TARGET = $(UWSGI_BUILD_NAME)_opts.ini
BOOTSTRAP_PY_TARGET = bootstrap.py

define UWSGI_BUILD_INI
[uwsgi]
main_plugin = python,gevent
inherit = base
bin_name = pex_uwsgi
embed_files = bootstrap.py
embed_config = pex_uwsgi_opts.ini
endef

define UWSGI_OPTS_INI
[uwsgi]
import = sym://bootstrap_py
endef

define BOOTSTRAP_PY
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
endef

export UWSGI_BUILD_INI
export UWSGI_OPTS_INI
export BOOTSTRAP_PY

build: clean $(BUILD_DIR) $(UWSGI_FILE)
	cd $(BUILD_DIR) && tar zxvf $(UWSGI_FILE)
	echo "$$UWSGI_BUILD_INI" > $(BUILD_DIR)/$(UWSGI_DIR)/$(UWSGI_BUILD_INI_TARGET)
	echo "$$UWSGI_OPTS_INI" > $(BUILD_DIR)/$(UWSGI_DIR)/$(UWSGI_OPTS_INI_TARGET)
	echo "$$BOOTSTRAP_PY" > $(BUILD_DIR)/$(UWSGI_DIR)/$(BOOTSTRAP_PY_TARGET)
	cd $(BUILD_DIR)/$(UWSGI_DIR) && $(PYTHON) uwsgiconfig.py --build $(UWSGI_BUILD_NAME)
	@echo
	@echo ========================================================
	@echo
	@echo built: $(BUILD_DIR)/$(UWSGI_DIR)/$(UWSGI_BUILD_NAME)
	@echo
	@echo ========================================================
	@echo

upload: build
	aurora package_add_version --cluster=$(AURORA_CLUSTER) $(AURORA_ROLE) uwsgi $(BUILD_DIR)/$(UWSGI_DIR)/$(UWSGI_BUILD_NAME)

clean:
	rm -rvf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(UWSGI_FILE):
	cd $(BUILD_DIR) && wget $(UWSGI_URL)
