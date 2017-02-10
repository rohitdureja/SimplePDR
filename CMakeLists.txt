cmake_minimum_required(VERSION 3.1)
project(SimplePDR CXX)
enable_language(CXX C)

# set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -DDEBUG")

# Bring in the header files
include_directories(include)

# Bring in the source file
file(GLOB SOURCES "src/*/*.cpp" "src/*.cpp")

# add sources to executable
add_executable(SimplePDR ${SOURCES})

# find and link the Z3 library
find_library(Z3_LIB z3)
target_link_libraries(SimplePDR ${Z3_LIB})