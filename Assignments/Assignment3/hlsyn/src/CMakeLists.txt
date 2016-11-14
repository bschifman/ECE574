# Authors: Kevin Curtis, Ben Schifman
# NetIDs: kevincurtis, bschifman
# Date: 14 November 2016
# Assignment: 3


set( CMAKE_CXX_FLAGS "-Wall -g" )

file( GLOB SRCS *.cpp *.h )

add_executable( hlsyn ${SRCS} )

