#! /bin/sh

home_libs=~/libs

if test -d /llj/libs ; then
   home_libs=/llj/libs
fi

home_ctr=${home_libs}/ctr

./configure --prefix=${home_ctr}

if test -d ${home_ctr}; then
    rm -rf ${home_ctr}
fi

make install

make distclean
