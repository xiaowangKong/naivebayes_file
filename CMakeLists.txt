cmake_minimum_required(VERSION 3.6)
project(naivebayes_file)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp NaiveBayesModel.cpp NaiveBayesModel.h)
add_executable(naivebayes_file ${SOURCE_FILES})