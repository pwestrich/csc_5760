
HEADER := $(wildcard *.h)
FILES  := $(wildcard *.cpp)
OBJECT := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

TARGET := libpmwdispatch.a

.PHONY: all clean

all: $(TARGET)

	$(MV) $(TARGET) ../lib/

clean:

	$(RM) $(OBJECT) $(TARGET)

$(TARGET): $(OBJECT)

	$(AR) $(TARGET) $(OBJECT)

dispatch.o: dispatch.cpp ../include/pmw_dispatch/dispatch.h

	$(COMPILE) dispatch.cpp

internal.o: internal.cpp internal.h ../include/pmw_dispatch/dispatch.h

	$(COMPILE) internal.cpp
