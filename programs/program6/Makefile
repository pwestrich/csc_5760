
# compiler information
CXX      := nvcc
CCFLAGS  := -g -O0
AR 		 := ar -r
RM 		 := rm -f
MV 		 := mv
MAKE 	 := make

INC_DIRS :=
LIB_DIRS :=
LIBS 	 :=

#how we build things
COMPILE := $(CXX) $(CCFLAGS) $(INC_DIRS) -c
LINK    := $(CXX) $(LIB_DIRS) -o

HEADER := $(wildcard *.h)
FILES  := $(wildcard *.cu)
OBJECT := $(patsubst %.cu,%.o,$(wildcard *.cu))

TARGET := main_cuda

.PHONY: all clean

all: $(TARGET)

clean:

	$(RM) $(OBJECT) $(TARGET)

$(TARGET): matrix_cuda.o getRealTime.o

	$(LINK) $(TARGET) $(OBJECT) $(LIBS)

matrix_cuda.o: matrix_cuda.cu getRealTime.h

	$(COMPILE) matrix_cuda.cu

getRealTime.o: getRealTime.cu getRealTime.h

	$(COMPILE) getRealTime.cu
