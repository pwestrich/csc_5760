
#utility information
CXX      := mpic++
CPPFLAGS := -std=c++11 -O0 -pedantic -Wall -Wextra -g
LDFLAGS  := $(CPPFLAGS)
AR 		 := ar -r
RM 		 := rm -f
MV 		 := mv

#build information
INC_DIRS := 
LIB_DIRS := 
LIBS 	 := -lstdc++ #-lrt #linux needs this

#how we build things
COMPILE := $(CXX) $(CPPFLAGS) $(INC_DIRS) -c
LINK    := $(CXX) $(LDFLAGS) $(LIB_DIRS) -o

#what to build
TARGET := main_mpi

#files
HEADER := $(wildcard *.h)
SOURCE := $(wildcard *.cpp)
OBJECT := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

.PHONY: all clean

all: $(TARGET)

clean: 

	$(RM) $(TARGET) $(OBJECT)
	$(RM) -r *.dYSM


main_mpi: main_mpi.o getRealTime.o

	$(LINK) main_mpi main_mpi.o getRealTime.o $(LIBS)

#generic rule for anything missed
%.o: %.cpp %.h

	$(COMPILE) $<
