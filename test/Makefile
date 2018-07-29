
CC=g++
CFLAGS=-c -Werror -ansi
LDFLAGS=-lpthread -g -ggdb  
OBJECTS=$(SOURCES:.cpp=.o)

SOURCES=../PointGroup.cpp ../Point.cpp ../Poly.cpp driver.cpp
EXECUTABLE=driver

###############################################################################

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf *o $(EXECUTABLE)
