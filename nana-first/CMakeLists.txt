cmake_minimum_required(VERSION 2.8.9)
project (nana-first)

#######################################################
# Prerequisites:
#   1. git clone https://github.com/cnjinhao/nana.git
#   2. cd nana
#   3. mv build build-original
#   4. mkdir build && cd build
#   5. cmake ..
#   6. make
#   7. sudo make install
#######################################################

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14" )
#add_definitions(-DDEBUG)

add_executable (nana-first src/main.cc)
target_include_directories(nana-first PUBLIC ./include)
target_link_libraries(nana-first nana X11 Xft fontconfig pthread)
