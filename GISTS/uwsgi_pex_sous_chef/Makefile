# VERSION is the cookbook/deployment version.
# RELEASE is the version of the sous-chef package.
NAME=sous-chef
VERSION?=$(shell cat VERSION)
RELEASE?=$(shell cat RELEASE)
PACKAGE=$(NAME)-$(VERSION)-$(RELEASE).noarch.rpm
OWNER=$(shell stat -c "%u:%g" ./Makefile)

# Builds a single-file Python environment that can be used to run Gunicorn.
# The script in `src/sous-chef.sh` then wraps it to start sous-chef correctly.
sous-chef-gunicorn.pex: Makefile
	pex -o "$@" -c "gunicorn" "sous-chef>=$(RELEASE)" "gunicorn>=18.0"
	@chown $(OWNER) $@

# Generates the site with middleman and then builds a package using fpm
# The package depends on 'openssl-devel' becuase a) Python and b) Chef.
$(PACKAGE): sous-chef-gunicorn.pex
	@rm -f $(PACKAGE)
	fpm -t rpm -s dir --log warn --package "$@" \
	--name "$(NAME)" \
	--maintainer "sam@datasift.com" \
	--vendor "DataSift" \
	--url "https://github.com/datasift/sous-chef" \
	--description "A web frontend for the Chef server index." \
	--epoch 0 \
	--version "$(VERSION)" \
	--iteration "$(RELEASE)" \
	--architecture all \
	--depends python27-python \
	--depends openssl-devel \
	--rpm-auto-add-directories /etc/sous-chef \
	sous-chef-gunicorn.pex=/opt/rh/python27/root/usr/bin/sous-chef-gunicorn \
	src/sous-chef.sh=/usr/bin/sous-chef
	@chown $(OWNER) $@

# Runs this makefile inside a docker container
gocd:
	docker build -t $(NAME)-build .
	docker run \
	-v $(shell pwd):/opt/src \
	-v /etc/sous-chef:/etc/sous-chef \
	-e VERSION=$(VERSION) \
	-e RELEASE=$(RELEASE) \
	$(NAME)-build 'make $(PACKAGE)'

clean:
	rm -f *.rpm *.pex

.PHONY: gocd clean
