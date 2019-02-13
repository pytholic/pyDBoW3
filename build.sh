#!/bin/bash

set -x

cd ${0%%$(basename $0)}
CWD=`pwd`

cd install

chmod +x boost.sh && ./boost.sh || exit 1

chmod +x opencv.sh && ./opencv.sh || exit 1

chmod +x dbow3.sh && ./dbow3.sh || exit 1

cd ..

mkdir build
cd build

if [[ "$OSTYPE" == "linux-gnu" || "$OSTYPE" == "linux" ]]; then
    cmake -DBUILD_PYTHON3=OFF \
          -DBUILD_STATICALLY_LINKED=OFF \
          -DOpenCV_DIR=$CWD/install/opencv3/build \
          -DDBoW3_DIR=$CWD/install/DBow3/build \
          -DDBoW3_INCLUDE_DIRS=$CWD/install/DBow3/src \
          -DCMAKE_BUILD_TYPE=Release ../src && make
elif [[ "$OSTYPE" == "darwin"* ]]; then
    PYTHON_VERSION=`python -c "import sys;t='{v[0]}.{v[1]}'.format(v=list(sys.version_info[:2]));sys.stdout.write(t)";`
    PYTHON_LIBRARY=/usr/local/Frameworks/Python.framework/Versions/$PYTHON_VERSION/lib/libpython$PYTHON_VERSION.dylib
    PYTHON_INCLUDE_DIR=/usr/local/Frameworks/Python.framework/Versions/$PYTHON_VERSION/Headers/
    cmake -DPYTHON_LIBRARY=$PYTHON_LIBRARY -DPYTHON_INCLUDE_DIR=$PYTHON_INCLUDE_DIR -DCMAKE_BUILD_TYPE=RELEASE ../src && make
elif [[ "$OSTYPE" == "cygwin" ]]; then
    : # POSIX compatibility layer and Linux environment emulation for Windows
elif [[ "$OSTYPE" == "msys" ]]; then
    : # shell and GNU utilities compiled for Windows as part of MinGW
elif [[ "$OSTYPE" == "win32" ]]; then
    : # good luck
elif [[ "$OSTYPE" == "freebsd"* ]]; then
    : # ...
else
    : # Unknown.
fi
