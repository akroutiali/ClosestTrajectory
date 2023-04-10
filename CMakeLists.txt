cmake_minimum_required(VERSION 3.10)
project(ClosestTrajectory)
set(SOURCES
    src/closesttrajectory.cpp
)
add_library(ClosestTrajectory SHARED ${SOURCES})
include_directories(${GTEST_INCLUDE_DIRS} "${CMAKE_CURRENT_BINARY_DIR}/include")
set_target_properties(ClosestTrajectory PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
# Add the executable source files
add_executable(CTraj src/main.cpp)

# Link the library to the executable
target_link_libraries(CTraj ClosestTrajectory cpprest pthread crypto flann lz4)