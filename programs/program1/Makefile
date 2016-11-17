
#utility information
CC      := g++
CCFLAGS := -std=c++11 -O0 -pedantic -Wall -Wextra
AR 		:= ar -r
RM 		:= rm -f
MV 		:= mv

#build information
INC_DIRS := 
LIB_DIRS := 
LIBS 	 :=  -lpthread -lrt

#how we build things
COMPILE := $(CC) $(CCFLAGS) $(INC_DIRS) -c
LINK := $(CC) $(LIB_DIRS) -o

#files
HEADER := $(wildcard *.h)
SOURCE := $(wildcard *.cpp)
OBJECT := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

#what to build
TARGET := pi1 pi2 pi3 pi4 pi5

.PHONY: all clean

all: $(TARGET)

clean: 

	$(RM) $(TARGET) $(OBJECT)

pi1: pi1.o getRealTime.o

	$(LINK) pi1 pi1.o getRealTime.o $(LIBS)

pi2: pi2.o getRealTime.o

	$(LINK) pi2 pi2.o getRealTime.o $(LIBS)

pi3: pi3.o getRealTime.o

	$(LINK) pi3 pi3.o getRealTime.o	$(LIBS)

pi4: pi4.o getRealTime.o

	$(LINK) pi4 pi4.o getRealTime.o $(LIBS)

pi5: pi5.o getRealTime.o

	$(LINK) pi5 pi5.o getRealTime.o $(LIBS)

#generic rule for anything missed
%.o: %.cpp

	$(COMPILE) $<

