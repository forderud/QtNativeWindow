cmake_minimum_required(VERSION 3.16)

project(QtNativeWindow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# enable unicode for all projects
add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory(MyOleControl)
add_subdirectory(QtNativeWindow)
