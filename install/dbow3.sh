# Install DBow3
git clone https://github.com/rmsalinas/DBow3 DBow3
cd DBow3
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=OFF -DUSE_CONTRIB=ON .. && make
cd ../..