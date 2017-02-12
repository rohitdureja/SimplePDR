cmake_minimum_required(VERSION 3.1)
project(SimplePDR CXX)
enable_language(CXX C)

# set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -DDEBUG")

# find packages
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# bring in the bison file
file(GLOB FLEX_SOURCE "src/parser/*/*.l")
file(GLOB BISON_SOURCE "src/parser/*/*.yy")

BISON_TARGET(parser
			 ${BISON_SOURCE}
			 ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cc)

FLEX_TARGET(lexer
			${FLEX_SOURCE}
			${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

# Bring in the source file
file(GLOB SOURCES "src/*/*.cpp" "src/*.cpp")
	
# add sources to executable
add_executable(SimplePDR
			   ${SOURCES}
			   ${FLEX_lexer_OUTPUTS}
			   ${BISON_parser_OUTPUTS})

# Bring in the header files
target_include_directories(SimplePDR
						   PRIVATE
						   include
						   .
						   ${CMAKE_CURRENT_BINARY_DIR})

# find and link the Z3 library
find_library(Z3_LIB z3)
target_link_libraries(SimplePDR ${Z3_LIB})