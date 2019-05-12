# Different versions of Open CV (2.x vs 3.x) define functions in different libraries.
# if library files /usr/lib/x86_64-linux-gnu/libopencv_*.so.* are 2.x version, use this line:
# LDFLAGS=-lopencv_core -lopencv_highgui -lopencv_imgproc
# if they are 3.x version, use this line:
LDFLAGS=-lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs
CFLAGS=-g -Wall -std=c++11
CC=g++
EXE1=fc
EXE2=ft
SOURCES=fibertest.cpp fibergrid.cpp fibercal.cpp
HEADERS=$(SOURCES:.cpp=.h)
OBJECTS=$(SOURCES:.cpp=.o)

all: $(EXE1) $(EXE2)

$(EXE1): $(OBJECTS)
	$(CC) $(LDFLAGS) fibercal.o -o $(EXE1)

$(EXE2): $(OBJECTS)
	$(CC) $(LDFLAGS) fibertest.o fibergrid.o -o $(EXE2)

.cpp.o: $(SOURCES) $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm *.o
