#!/bin/bash

FORCE=1
PACKAGE_NAME="nginx"
BUILD_PREFIX="/usr/local/"
TARGET_PREFIX="/www/nginx"
#SOURCE_PACKAGE="ngx_openresty-1.5.8.1.tar.gz"
SOURCE_PACKAGE="ngx_openresty-1.5.12.1.tar.gz"
MAINTAINER_EMAIL="<lordnynex@gmail.com>"
# Overlay repo
OVERLAY_REPO="...."

SOURCE_PACKAGE_DIR=$(echo $SOURCE_PACKAGE |sed -e 's/\.tar\.gz$//g')
SOURCE_PACKAGE_VERSION=$(echo $SOURCE_PACKAGE_DIR | sed -e 's/^.*-//g') # This is ghetto
BUILD_DIRS=(bin build notes packages src logs)

# Build time deps
BUILD_DEPS=(
        build-essential
        make
        ruby1.9.1
        ruby1.9.1-dev
)

# Package install deps
PACKAGE_DEPS=(
        libperl-dev
        libregexp-grammars-perl
        libgd2-xpm-dev
        libimlib2
        libpcre3-dev
        libreadline-dev
        libncurses5-dev
        libssl-dev
        libxslt1-dev
        libgeoip-dev
        libpq-dev
)

PACKAGE_CONFIG_FILES=(
        /etc/nginx/nginx.conf
        /etc/nginx/conf.d/clojure.conf
        /etc/nginx/conf.d/fastcgi.conf
        /etc/nginx/conf.d/fastcgi.conf.default
        /etc/nginx/conf.d/fastcgi_params
        /etc/nginx/conf.d/fastcgi_params.default
        /etc/nginx/conf.d/koi-utf
        /etc/nginx/conf.d/koi-win
        /etc/nginx/conf.d/mime.types
        /etc/nginx/conf.d/mime.types.default
        /etc/nginx/conf.d/scgi_params
        /etc/nginx/conf.d/scgi_params.default
        /etc/nginx/conf.d/timezone.conf
        /etc/nginx/conf.d/uwsgi_params
        /etc/nginx/conf.d/uwsgi_params.default
        /etc/nginx/conf.d/win-utf
)

ADDITIONAL_MODULES=(
                git://github.com/yzprofile/ngx_http_dyups_module.git
)

#PACKAGE_BUILD_DIR=/home/ubuntu/build/nginx
#FPM_DIRS=("${PACKAGE_BUILD_DIR}/build/*")
#FPM_DIRS_STRING=$(IFS=, ; echo "${FPM_DIRS[*]}")
#
#echo $FPM_DIRS_STRING;
#exit;

# Build package config file string (arg for fpm)
for conf in ${PACKAGE_CONFIG_FILES[@]}; do
        PACKAGE_CONFIG_FILES_STRING="$PACKAGE_CONFIG_FILES_STRING --config-files $conf"
done

# Detect number of cores for make
NUM_CORES=$(cat /proc/cpuinfo | grep -i processor | wc -l)

# Build string to pass to apt-get for build time deps
for package in ${BUILD_DEPS[@]}; do
        BUILD_DEP_STRING="$BUILD_DEP_STRING $package"
done

# Add package deps to build dep string
# Create FPM dependancy string
for package in ${PACKAGE_DEPS[@]}; do
        BUILD_DEP_STRING="$BUILD_DEP_STRING $package"
        PACKAGE_DEP_STRING="$PACKAGE_DEP_STRING -d $package"
done

# Detect home dir
if [[ -z "$SUDO_USER" ]]; then
        USER_HOME=$(getent passwd $LOGNAME |awk '{split($0,a,":"); print a[6]}')
else
        USER_HOME=$(getent passwd $SUDO_USER |awk '{split($0,a,":"); print a[6]}')
fi

# With Trailing slash!
BUILD_DIR=$USER_HOME/nginx/build/
PACKAGE_BUILD_DIR=$BUILD_DIR$PACKAGE_NAME

# Create Build Sub Dirs
for dir in ${BUILD_DIRS[@]}; do
        mkdir -p $PACKAGE_BUILD_DIR/$dir
done

# Fetch Package
wget -P $PACKAGE_BUILD_DIR/src http://openresty.org/download/$SOURCE_PACKAGE

# Extract source
cd $PACKAGE_BUILD_DIR/src
tar -zxvf $SOURCE_PACKAGE

sudo apt-get -y install $BUILD_DEP_STRING

if [ $? -eq 0 ] ; then
        echo "Build dependancies installed"
else
        echo "Build dependancies failed to install"
        exit 1;
fi

# More Deps
if [ ! -f /usr/local/bin/fpm ] ; then
        sudo gem install fpm

        if [ $? -eq 0 ] ; then
                echo "FPM installed"
        else
                echo "FPM failed to install"
                exit 1;
        fi
fi

# pull down modules
mkdir nginx_modules
cd nginx_modules
#git clone https://github.com/yzprofile/ngx_http_dyups_module.git
for module in ${ADDITIONAL_MODULES[@]}; do
        git clone $module
done

# Pull down the overlay
mkdir -p $BUILD_DIR/overlays
OVERLAY_TARGET="$BUILD_DIR/overlays/nginx"
git clone $OVERLAY_REPO $OVERLAY_TARGET

rsync -az --exclude=.git* --exclude=.placeholder $OVERLAY_TARGET/build/ $PACKAGE_BUILD_DIR

if [ $? -eq 0 ] ; then
        echo "Successfully pulled package overlay."
else
        echo "Failed to pull package overlay."
        exit 1;
fi

cd ../$SOURCE_PACKAGE_DIR

./configure \
        --prefix=$TARGET_PREFIX \
        --with-cc-opt="-O3" \
        --with-luajit \
        --sbin-path=/usr/sbin/nginx \
        --conf-path=/etc/nginx/nginx.conf \
        --error-log-path=$TARGET_PREFIX/log/error.log \
        --http-client-body-temp-path=$TARGET_PREFIX/lib/nginx/body \
        --http-fastcgi-temp-path=$TARGET_PREFIX/lib/nginx/fastcgi \
        --http-log-path=$TARGET_PREFIX/log/access.log \
        --http-proxy-temp-path=$TARGET_PREFIX/lib/nginx/proxy \
        --http-scgi-temp-path=$TARGET_PREFIX/lib/nginx/scgi \
        --http-uwsgi-temp-path=$TARGET_PREFIX/lib/nginx/uwsgi \
        --lock-path=$TARGET_PREFIX/lock/nginx.lock \
        --pid-path=$TARGET_PREFIX/run/nginx.pid \
        --with-http_dav_module \
        --with-http_flv_module \
        --with-http_geoip_module \
        --with-http_gzip_static_module \
        --with-http_image_filter_module \
        --with-http_realip_module \
        --with-http_stub_status_module \
        --with-http_ssl_module \
        --with-http_xslt_module \
        --with-sha1=/usr/include/openssl \
        --with-md5=/usr/include/openssl \
        --with-mail \
        --with-http_secure_link_module \
        --with-http_sub_module \
        --with-http_perl_module \
        --with-http_postgres_module \
        --add-module=$PACKAGE_BUILD_DIR/src/nginx_modules/ngx_http_dyups_module

if [ $? -eq 0 ] ; then
        echo "Package Configured"
else
        echo "Package failed to configure"
        exit 1;
fi

# Parallel Make
make -j$NUM_CORES

if [ $? -eq 0 ] ; then
        echo "Make Succeeded"
else
        echo "Make Failed"
        exit 1;
fi

make install DESTDIR=$PACKAGE_BUILD_DIR/build

# Overlay conf
rsync -az --exclude=.git* $OVERLAY_TARGET/conf/ $PACKAGE_BUILD_DIR/build/
rsync -avz --exclude=.git* --delete $OVERLAY_TARGET/conf/etc/nginx/ $PACKAGE_BUILD_DIR/build/etc/nginx/

# Assemble array of dirs in build staging dir
# GLOBS BRO
#FPM_DIRS=("${PACKAGE_BUILD_DIR}/build/*")
#FPM_DIRS_STRING=$(IFS=, ; echo "${FPM_DIRS[*]}")

i=0
while read line
do
        echo "FOUND $line"
        array[ $i ]="$line"
        (( i++ ))
done < <(ls ${PACKAGE_BUILD_DIR}/build/)

echo ${array[1]}

FPM_DIRS_STRING=$(IFS=" " ; echo ${array[*]})

echo "##################################################################################################################"
echo "#                                                                                                                #"
echo "# FORCE                  : ${FORCE}                                                                              #"
echo "# SOURCE_PACKAGE_VERSION : ${SOURCE_PACKAGE_VERSION}                                                             #"
echo "# PACKAGE_BUILD_DIR      : ${PACKAGE_BUILD_DIR}                                                                  #"
echo "# MAINTAINER_EMAIL       : ${MAINTAINER_EMAIL}                                                                   #"
echo "# PACKAGE_BUILD_DIR      : ${PACKAGE_BUILD_DIR}                                                                  #"
echo "# PACKAGE_DEP_STRING     : ${PACKAGE_DEP_STRING}                                                                 #"
echo "# FPM_DIRS_STRING        : ${FPM_DIRS_STRING}                                                                    #"
echo "#                                                                                                                #"
echo "##################################################################################################################"

/usr/local/bin/fpm \
        --force \
        -s dir \
        -t deb \
        -v $SOURCE_PACKAGE_VERSION \
        --iteration 1 \
        -C $PACKAGE_BUILD_DIR/build \
        -n nynex-resty \
        --description='resty build' \
        -m $MAINTAINER_EMAIL \
        -p $PACKAGE_BUILD_DIR/packages/nynex-resty-VERSION_ARCH.deb \
        $PACKAGE_DEP_STRING \
        $FPM_DIRS_STRING
