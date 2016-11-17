
#utility information
CXX      := g++
CPPFLAGS := -std=c++11 -O0 -pedantic -Wall -Wextra -g
LDFLAGS  :=
AR 		 := ar -r
RM 		 := rm -f
MV 		 := mv

#build information
INC_DIRS := 
LIB_DIRS := 
LIBS 	 :=  -lpthread #-lrt #linux needs this

#how we build things
COMPILE := $(CXX) $(CPPFLAGS) $(INC_DIRS) -c
LINK    := $(CXX) $(CPPFLAGS) $(LIB_DIRS) -o

#files
HEADER := $(wildcard *.h)
SOURCE := $(wildcard *.cpp)
OBJECT := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

#what to build
TARGET := matrix_serial matrix_parallel

.PHONY: all clean

all: $(TARGET)

clean: 

	$(RM) $(TARGET) $(OBJECT)

matrix_serial: main_serial.o matrix.o getRealTime.o

	$(LINK) matrix_serial main_serial.o matrix.o getRealTime.o $(LIBS)

matrix_parallel: main_parallel.o matrix.o getRealTime.o

	$(LINK) matrix_parallel main_parallel.o matrix.o getRealTime.o $(LIBS)

main_serial.o: main_serial.cpp matrix.h getRealTime.h

	$(COMPILE) main_serial.cpp

main_parallel: main_parallel.cpp matrix.h getRealTime.h

	$(COMPILE) main_parallel.cpp

#generic rule for anything missed
%.o: %.cpp %.h

	$(COMPILE) $<

