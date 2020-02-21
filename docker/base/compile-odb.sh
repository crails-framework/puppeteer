#!/bin/sh -ex

ODB_MAJOR_VERSION=2
ODB_MINOR_VERSION=5
ODB_PATCH_VERSION=0
ODB_SHORT_VERSION="$ODB_MAJOR_VERSION.$ODB_MINOR_VERSION"
ODB_VERSION="$ODB_MAJOR_VERSION.$ODB_MINOR_VERSION.$ODB_PATCH_VERSION-b.3"

CUTL_VERSION_MAJOR=1
CUTL_VERSION_MINOR=11
CUTL_VERSION_PATCH=0
CUTL_VERSION="$CUTL_VERSION_MAJOR.$CUTL_VERSION_MINOR.$CUTL_VERSION_PATCH-a1"

apt-get install -y libpq-dev

PRERELEASE_URL=https://codesynthesis.com/~boris/tmp/odb/pre-release/b.3/

wget "$PRERELEASE_URL/libcutl-$CUTL_VERSION.tar.gz"
#wget http://www.codesynthesis.com/download/libcutl/$CUTL_VERSION_MAJOR.$CUTL_VERSION_MINOR/libcutl-$CUTL_VERSION.tar.gz
tar -xvzf libcutl-$CUTL_VERSION.tar.gz
cd libcutl-$CUTL_VERSION
./configure
make
make install
cd ..

wget "$PRERELEASE_URL/odb-$ODB_VERSION.tar.gz"
#wget http://www.codesynthesis.com/download/odb/$ODB_SHORT_VERSION/odb-$ODB_VERSION.tar.gz
tar -xzvf odb-$ODB_VERSION.tar.gz
cd odb-$ODB_VERSION
./configure
make
make install
cd ..

wget "$PRERELEASE_URL/libodb-$ODB_VERSION.tar.gz"
#wget http://www.codesynthesis.com/download/odb/$ODB_SHORT_VERSION/libodb-$ODB_VERSION.tar.gz
tar -xzvf libodb-$ODB_VERSION.tar.gz
cd libodb-$ODB_VERSION
./configure
make
make install
cd ..

wget "$PRERELEASE_URL/libodb-pgsql-$ODB_VERSION.tar.gz"
#wget http://www.codesynthesis.com/download/odb/$ODB_SHORT_VERSION/libodb-pgsql-$ODB_VERSION.tar.gz
tar -xzvf libodb-pgsql-$ODB_VERSION.tar.gz
cd libodb-pgsql-$ODB_VERSION
./configure
make
make install
cd ..
