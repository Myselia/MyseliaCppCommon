#!/bin/sh

g++ -o MyseliaCppCommon -std=c++11 \
main.cpp \
src/cppcommon/Generic.cpp src/cppcommon/Json.cpp src/cppcommon/IO.cpp src/cppcommon/JavaIO.cpp \
src/common/generic/FrameInputStream.cpp src/common/generic/FrameOutputStream.cpp \
-lboost_system -lboost_thread
 
