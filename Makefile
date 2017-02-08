# Z3 Headers and Libraries
Z3_HEADER_SOURCE = -Iz3/include/
Z3_LIB_SOURCE = -Lz3/bin/
Z3_LIB = -lz3

# Project source file
CPP_SOURCES = \
		main.cpp \
		IC3.cpp \
		model.cpp \
		solver.cpp \
		utils.cpp

# Output object file
OBJ_FILE = SimplePDR.o

# Definitions
DEBUG = -DDEBUG

all: SimplePDR

SimplePDR: $(CPP_SOURCES)
	g++ -Wall -Wextra $(DEBUG) $(Z3_HEADER_SOURCE) $(Z3_LIB_SOURCE) -std=c++11 $(CPP_SOURCES) -o $(OBJ_FILE) $(Z3_LIB)
#	./$(OBJ_FILE)
	

clean: $(OBJ_FILE)
	rm $(OBJ_FILE)
