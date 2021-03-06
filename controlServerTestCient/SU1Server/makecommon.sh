#!/bin/sh

libtoolize --automake --copy --force
aclocal
autoconf
autoheader
automake --add-missing

cd build
../configure --enable-debug
make

cd ../
