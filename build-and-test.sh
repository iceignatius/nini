#!/bin/sh

#
# Project build and test.
#

./gendoc.sh

test -d install && rm -r install
test -d build || mkdir build
cd build && rm -r ./*

cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../install -DBUILD_SHARED_LIBS=no .. && \
    make && \
    make test && \
    make install
