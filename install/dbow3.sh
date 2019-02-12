# Install DBow3
CWD=`pwd`
git clone https://github.com/rmsalinas/DBow3 DBow3
cd DBow3
mkdir build
cd build

cmake -DOpenCV_DIR=$CWD/opencv3/build \
      -DBUILD_SHARED_LIBS=OFF \
      -DUSE_CONTRIB=ON \
      -DBUID_UTILS=OFF .. && make && make install
#      -DCMAKE_INSTALL_PREFIX=$CWD/DBow3 \
cd ../..
