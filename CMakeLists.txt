cmake_minimum_required(VERSION 2.8.9)
project(MagicPurse)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(Main ${SOURCES})
