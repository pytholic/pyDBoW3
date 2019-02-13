# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

cmake_minimum_required(VERSION 2.8)
project(pyDBoW3)

# Project version
#------------------------------------------------------------------------------
set(pyDBOW3_MAJOR_VERSION 1)
set(pyDBOW3_MINOR_VERSION 1)
set(pyDBOW3_PATCH_VERSION 1)
set(pyDBOW3_VERSION ${pyDBOW3_MAJOR_VERSION}.${pyDBOW3_MINOR_VERSION}.${pyDBOW3_PATCH_VERSION})
set(pyDBOW3_VERSION_STR ${pyDBOW3_VERSION})
set(pyDBOW3_VERSION_ID ${pyDBOW3_MAJOR_VERSION}${pyDBOW3_MINOR_VERSION}${pyDBOW3_PATCH_VERSION})


# Building options
#-------------------------------------------------------------------------------
option(BUILD_PYTHON3 "Build DBow3 Python3 binding instead of default Python2." OFF)
OPTION(BUILD_STATICALLY_LINKED 	"Set to OFF to link with opencv and DBoW3 dynamically" ON)

#SET (BOOST_ROOT "c:/boost_1_67_0/")

SET(Boost_DEBUG OFF)

if(BUILD_PYTHON3)
	set(PYTHON_VERSION 3)
else()
	set(PYTHON_VERSION 2)
endif()

# CMake options
#-----------------------------------------------------------------------------------------------------------------------

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall  -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -Wall -O3")

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    add_definitions(-DCOMPILEDWITHC11)
    message(STATUS "Using flag -std=c++11.")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    add_definitions(-DCOMPILEDWITHC0X)
    message(STATUS "Using flag -std=c++0x.")
else()
    #message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

if (COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif (COMMAND cmake_policy)

# Add cmake_modules to module
set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")

# Set the release mode if not specified
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions("-fPIC")
endif ()


#-----------------------------------------------------------------------------------------------------------------------

#LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../../../cmake_modules)

IF(NOT UNIX)
  SET(OpenCV_USE_STATIC_LIBS ON)
ENDIF()
find_package(OpenCV REQUIRED)

SET(DBoW3_USE_STATIC_LIBS ON)
find_package(DBoW3 REQUIRED)

#-----------------------------------------------------------------------------------------------------------------------
# Python binding
#-----------------------------------------------------------------------------------------------------------------------

FIND_PACKAGE(Boost 1.54)

IF(Boost_FOUND)
 IF (NOT UNIX)
  SET(Boost_USE_STATIC_LIBS ON)
 ENDIF()
  SET(Boost_USE_MULTITHREADED ON)
  SET(Boost_USE_STATIC_RUNTIME OFF)
  SET(BOOST_ALL_NO_LIB ON)

   message(STATUS "Boost_ROOT = ${Boost_ROOT}")
   message(STATUS "Boost_INCLUDE_DIR = ${Boost_INCLUDE_DIR}")

ELSEIF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "Unable to find correct Boost version. Did you set BOOST_ROOT?")
ENDIF()

FIND_PACKAGE(PythonInterp ${PYTHON_VERSION})

if (PYTHONINTERP_FOUND)
  if (UNIX AND NOT APPLE)
    if (PYTHON_VERSION_MAJOR EQUAL 3)
        FIND_PACKAGE(Boost 1.54 COMPONENTS python${PYTHON_VERSION_SUFFIX} REQUIRED)
        FIND_PACKAGE(PythonInterp 3)
        FIND_PACKAGE(PythonLibs 3 REQUIRED)
    else()
        FIND_PACKAGE(Boost 1.54 COMPONENTS python REQUIRED)
        FIND_PACKAGE(PythonInterp 2.7)
        FIND_PACKAGE(PythonLibs 2.7 REQUIRED)
    endif()
  else()	
    if (PYTHON_VERSION_MAJOR EQUAL 3)
        FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR} REQUIRED)
        FIND_PACKAGE(PythonInterp 3)
        FIND_PACKAGE(PythonLibs 3 REQUIRED)
    else()
        FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR} REQUIRED)
        FIND_PACKAGE(PythonInterp 2.7)
        FIND_PACKAGE(PythonLibs 2.7 REQUIRED)
    endif()
  endif()
else()
    message("Python not found")
endif()

find_package(NumPy REQUIRED)

if (NOT NUMPY_FOUND)
    message("Numpy not found")
endif ()

# This is a hack, since DBoW3 report weird library path
LINK_DIRECTORIES(${DBoW3_INCLUDE_DIRS}/../lib)

INCLUDE_DIRECTORIES(
${OpenCV_INCLUDE_DIRS} 
${Boost_INCLUDE_DIRS} 
${PYTHON_INCLUDE_DIRS} 
${NUMPY_INCLUDES} 
${DBoW3_INCLUDE_DIRS}/DBoW3 
lib/)

if (BUILD_STATICALLY_LINKED)
    add_definitions(-DBOOST_PYTHON_STATIC_LIB)
endif ()

# Build np<=>opencv converter library
add_library(np_opencv_converter lib/conversion.cpp lib/np_opencv_converter.cpp)
target_compile_options(np_opencv_converter PRIVATE -DBOOST_ALL_NO_LIB)
target_link_libraries(np_opencv_converter ${OpenCV_LIBRARIES} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})


# Build python extension 
add_library(pyDBoW3 SHARED dbow3.cpp)
target_compile_options(pyDBoW3 PRIVATE -DBOOST_ALL_NO_LIB)

if (BUILD_STATICALLY_LINKED)
	target_link_libraries(pyDBoW3 np_opencv_converter ${DBoW3_LIBRARIES} "-static")
else ()
	target_link_libraries(pyDBoW3 np_opencv_converter ${DBoW3_LIBRARIES} ${DBoW3_INCLUDE_DIRS}/../lib/libDBoW3.a)
endif ()


if (NOT UNIX)
	set_target_properties(pyDBoW3 PROPERTIES PREFIX "" SUFFIX ".pyd")
else ()
	set_target_properties(pyDBoW3 PROPERTIES PREFIX "")
endif()

# ----------------------------------------------------------------------------
# display status message for important variables
# ----------------------------------------------------------------------------
message( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
message( STATUS "General configuration for ${PROJECT_NAME} ${PROJECT_VERSION}")
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
message("    Built as static libs?:" ${BUILD_STATICALLY_LINKED})
message("    Compiler:"                   "${CMAKE_COMPILER}"   "${CMAKE_CXX_COMPILER}")

message( STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message( STATUS "C++ flags (Release):       ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
message( STATUS "C++ flags (Debug):         ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
message( STATUS "C++ flags (Relase+Debug):         ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

message( STATUS "CMAKE_CXX_FLAGS:         ${CMAKE_CXX_FLAGS}")
message( STATUS "CMAKE_BINARY_DIR:         ${CMAKE_BINARY_DIR}")
message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
message(STATUS "NUMPY_LIBRARIES = ${NUMPY_LIBRARIES}")
message(STATUS "NUMPY_INCLUDE_DIRS = ${NUMPY_INCLUDES}")
message(STATUS "DBoW3_INCLUDE_DIRS = ${DBoW3_INCLUDE_DIRS}")
message(STATUS "DBoW3_LIB_DIR = ${DBoW3_LIB_DIR}")
message(STATUS "DBoW3_LIBRARIES = ${DBoW3_LIBRARIES}")
message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")
message(STATUS "PROJECT LIBRARIES = ${PROJECT_LIBRARIES}")
