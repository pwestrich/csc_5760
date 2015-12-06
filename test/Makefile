
HEADER := $(wildcard *.h)
FILES  := $(wildcard *.cpp)
OBJECT := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

TARGET := test_main

.PHONY: all clean run

all: $(TARGET)

clean:

	$(RM) $(OBJECT) $(TARGET)

run: all

	./$(TARGET)

$(TARGET): $(OBJECT) ../lib/libpmwdispatch.a

	$(LINK) $(TARGET) $(OBJECT) $(LIBS) -lpmwdispatch

test_main.o: test_main.cpp ../include/pmw_dispatch/dispatch.h matrix.h getRealTime.h

	$(COMPILE) test_main.cpp

matrix.o: matrix.cpp matrix.h

	$(COMPILE) matrix.cpp

getRealTime.o: getRealTime.cpp getRealTime.h

	$(COMPILE) getRealTime.cpp
