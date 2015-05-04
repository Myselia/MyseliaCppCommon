#!/bin/sh

g++ -o MyseliaCppCommon -std=c++11 \
main.cpp \
src/cpp/Generic.cpp src/cpp/Json.cpp src/cpp/IO.cpp \
-lboost_system -lboost_thread
 