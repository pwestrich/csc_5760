AutomatedMakefile := am

# compiler information
CXX     := g++
CCFLAGS := -g -std=c++11 -O0 -pedantic -Wall -Wextra
AR 		:= ar -r

SYS := $(shell $(CC) -dumpmachine)

.PHONY: all run clean setup

run: all

	$(MAKE) -C ./test run

all: setup

	$(MAKE) -C ./src all
	$(MAKE) -C ./test all

cleanup: setup

	$(MAKE) -C ./src clean

setup:

ifneq (, $(findstring apple, $(SYS)))
# Do apple things
$(info System detected to be Mac OS X)

INC_DIRS := -I../include
LIB_DIRS := -L../lib
LIBS 	 := -lpthread

RM 		 := rm -f
MV 		 := mv
MAKE 	 := make

else ifneq (, $(findstring mingw, $(SYS)))
# Do mingw things
$(info System detected to be Windows MinGW)
$(error Cannot build this on Windows... don't even try.)

else ifneq (, $(findstring linux, $(SYS)))
# Do linux things
$(info System detected to be GNU/Linux)

INC_DIRS := -I../include
LIB_DIRS := -L../lib
LIBS 	 := -lpthread

RM 		 := rm -f
MV 		 := mv
MAKE 	 := make

$(error Set LIBS and INC_DIRS flags first before trying to build on Linux!)

else 
# Do other things
$(error Unrecongized OS)

endif

#how we build things
COMPILE := $(CXX) $(CCFLAGS) $(INC_DIRS) -c
LINK    := $(CXX) $(LIB_DIRS) -o

#export variables so that make chains can have them
export
