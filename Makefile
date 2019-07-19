

OBJS = src/main.o src/CMatrix.o src/CDenseMatrix.o src/CSparseMatrix.o src/CInterface.o

CC = g++

COMPILER_FLAGS = -w -Wall -std=c++11 -pedantic -Wno-long-long -Werror -O0 -ggdb -g

OBJ_NAME = matrixCalc

all : compile doc

compile : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME)

main.o : $($(CC) -MM src/main.cpp) src/main.cpp
	$(CC) $(COMPILER_FLAGS) -c src/main.cpp -o src/main.o

CMatrix.o : $($(CC) -MM src/CMatrix.cpp) src/CMatrix.cpp
	$(CC) $(COMPILER_FLAGS) -c src/CMatrix.cpp -o src/CMatrix.o

CDenseMatrix.o : $($(CC) -MM src/CDenseMatrix.cpp)
	$(CC) $(COMPILER_FLAGS) -c src/CDenseMatrix.cpp -o src/CDenseMatrix.o

CSparseMatrix.o : $($(CC) -MM src/CSparseMatrix.cpp)
	$(CC) $(COMPILER_FLAGS) -c src/CSparseMatrix.cpp -o src/CSparseMatrix.o

CInterface.o : $($(CC) -MM src/CInterface.cpp) src/CInterface.cpp
	$(CC) $(COMPILER_FLAGS) -c src/CInterface.cpp -o src/CInterface.o

doc :
	doxygen Doxyfile

.PHONY: doc

clean :
	rm -r ./src/*.o doc matrixCalc

.PHONY: clean

run :
	./matrixCalc
