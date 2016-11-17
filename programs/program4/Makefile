
#utility information
CXX      := g++
CPPFLAGS := -std=c++11 -O0 -pedantic -Wall -Wextra -g -fopenmp
LDFLAGS  := $(CPPFLAGS)
AR 		 := ar -r
RM 		 := rm -f
MV 		 := mv

#build information
INC_DIRS := 
LIB_DIRS := 
LIBS 	 := -lpthread -lstdc++ -lrt #linux needs this

#how we build things
COMPILE := $(CXX) $(CPPFLAGS) $(INC_DIRS) -c
LINK    := $(CXX) $(LDFLAGS) $(LIB_DIRS) -o

#what to build
TARGET := main_serial main_omp main_serial_text

#files
HEADER := $(wildcard *.h)
SOURCE := $(wildcard *.cpp)
OBJECT := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

.PHONY: all clean

all: $(TARGET)

clean: 

	$(RM) $(TARGET) $(OBJECT)
	$(RM) -r *.dYSM

main_serial: main_serial.o getRealTime.o

	$(LINK) main_serial main_serial.o getRealTime.o $(LIBS)

main_omp: main_omp.o getRealTime.o

	$(LINK) main_omp main_omp.o getRealTime.o $(LIBS)


main_serial_text: main_serial_text.o getRealTime.o

	$(LINK) main_serial_text main_serial_text.o getRealTime.o $(LIBS)

main_serial.o: main_serial.cpp getRealTime.h

	$(COMPILE) main_serial.cpp

main_omp.o: main_omp.cpp getRealTime.h

	$(COMPILE) main_omp.cpp

main_serial_text.o: main_serial_text.cpp getRealTime.h

	$(COMPILE) main_serial_text.cpp

#generic rule for anything missed
%.o: %.cpp %.h

	$(COMPILE) $<
