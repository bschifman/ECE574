# Authors: Kevin Curtis, Ben Schifman
# NetIDs: kevincurtis, bschifman
# Date: 2 December 2016
# Assignment: 3


set( CMAKE_CXX_FLAGS "-std=c++11" )

file( GLOB SRCS *.cpp *.h )

add_executable( hlsyn ${SRCS} )

