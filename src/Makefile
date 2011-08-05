CC = gcc
CFLAGS = -g -Wall `pkg-config --cflags --libs gtk+-2.0` -export-dynamic
LDFLAGS =
SOURCES = ltrgui.c statusbar_main.c menubar_main.c project_wizard.c error.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = ltrgui

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
