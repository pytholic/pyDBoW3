# pyDBoW3
*Ultra-fast Boost.Python interface for [DBoW3](https://github.com/rmsalinas/DBow3.git)*

This repo was created in order to interface DBoW algorithm from python in another project [EasyVision](https://github.com/foxis/EasyVision.git). It is being used for a simple topological SLAM implementation since OpenCV BowKMeansTrainer doesn't work with binary features.
If you wish you use it on your own it is as easy as:

```python
import pyDBoW3 as bow
voc = bow.Vocabulary()
voc.load("/slamdoom/libs/orbslam2/Vocabulary/ORBvoc.txt")
db = bow.Database()
db.setVocabulary(voc)
del voc
# extract features using OpenCV
...
# add features to database
for features in features_list:
  db.add(features)
  
# query features
feature_to_query = 1
results = db.query(features_list[feature_to_query])

del db
```

This repository was created based on [pyORBSLAM2](https://github.com/raulmur/ORB_SLAM2.git) and ndarray to cv::Mat conversion on [numpy-opencv-converter](https://github.com/GarrickLin/numpy-opencv-converter.git).

NOTE: Mainly tested on Windows using OpenCV 4.0.0 and Python 2.7.

# Get started

## Windows

Prerequisites:
* OpenCV
* Python 2.7 with Numpy and opencv-contrib-python
* Boost 1.67
* [DBoW3](https://github.com/rmsalinas/DBow3.git)
* cmake
* Microsoft Visual Studio (with python package)

To build Boost.Python, go to Boost root and run:

`bootstrap.bat --prefix=/dir/to/Boost.Build`

Then build Boost.Python like this:

`/dir/to/Boost.Build/b2 --with-python`

To build DBoW3, simply use cmake-gui to generate a MSVC solution. And then build using that solution.

Then you can use cmake-gui to create MSVC solution for pyDBoW3 and build it. Note that you will have to copy `DBoW3.dll`, `opencv*.dll` and `pyDBoW3.pyd` in the same directory and on `PYTHONPATH`. 

## Unix
Use run.sh to run nvidia-docker environment to build build/pyDBoW.so, which you should then put on your PYTHONPATH.
See tests for an example.

See below more detailed steps on how to set up the tutorial examples.

# Ubuntu 

Clone this repository (or even better: fork it!)

`git clone git@github.com:torrvision/pyORBSLAM2.git`

Install nvidia-docker plugin: see https://github.com/NVIDIA/nvidia-docker
Note: You require nvidia-docker plugin on host, as Pangolin2 requires OpenGL support

Now run the nvidia-docker container:

`sudo ./run.sh`

(It will take a while to compile OpenCV3)
(SSH password: source)

Now, inside the container SSH shell, simply do:
(ensure build and bin folders are deleted to force CMake to update properly)

`/orbslam/src/build.sh`

`export PYTHONPATH=/dbow3/src/build:$PYTHONPATH`

Now you should be able to try the test example inside the container:

`pytest`

(press key for GUI windows to open)
