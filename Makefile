CC=cc
LFLAGS=-Wall -ansi -Wpedantic -Wextra -O $(DEBUG)
CFLAGS=-c $(LFLAGS) 
DEBUG=-g
SOURCES=dosexe.c
OBJECTS=$(SOURCES:.c=.o)
HEADERS=$(SOURCES:.c=.h)
EXECUTABLE=dosexe


all: $(EXECUTABLE) 

clean:
	rm *.o $(EXECUTABLE) *.exe *.exe.stackdump

.SUFFIXES: .o .c
.c.o:
	$(CC) $(CFLAGS) $<

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@
