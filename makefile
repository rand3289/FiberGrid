# Different versions of Open CV (2.x vs 3.x) define functions in different libraries.
# if library files /usr/lib/x86_64-linux-gnu/libopencv_*.so.* are 2.x version, use this line:
# LDFLAGS=-lopencv_core -lopencv_highgui -lopencv_imgproc
# if they are 3.x version, use this line:
LDFLAGS=-lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs
CFLAGS=-g -Wall -std=c++11 -I /usr/include/opencv4
CC=g++
EXE1=fc
EXE2=ft
SOURCES=fibertest.cpp fibergrid.cpp fibercal.cpp
HEADERS=$(SOURCES:.cpp=.h)
OBJECTS=$(SOURCES:.cpp=.o)

all: $(EXE1) $(EXE2)

$(EXE1): $(OBJECTS)
	$(CC) fibercal.o -o $(EXE1) $(LDFLAGS)

$(EXE2): $(OBJECTS)
	$(CC) fibertest.o fibergrid.o -o $(EXE2) $(LDFLAGS)

.cpp.o: $(SOURCES) $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	/bin/rm -f *.o $(EXE1) $(EXE2)
