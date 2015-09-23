# Basic Make file (haven't tested it yet)
CC=g++
# Flags to be used 
CFLAGS=-w -s -Os -std=c++11
# Linked libraries
LDFLAGS=-lSDL2main -lSDL2 -lSDL2_image
# Path to .cpp files
SRCPATH = ./src/
# Name of the executable made
EXECUTABLE=cannon
# Include Flags to look in Src folder
INCLUDEFLAGS=-I. -I$(SRCPATH)

all: project

project: main.o cannonball.o config.o screen.o tick.o
    $(CC) $(CFLAGS) main.o cannonball.o config.o screen.o tick.o -o $(EXECUTABLE) $(LDFLAGS)

main.o: $(SRCPATH)main.cpp $(SRCPATH)version.h $(SRCPATH)global.h $(SRCPATH)screen.h $(SRCPATH)cannonball.h $(SRCPATH)tick.h $(SRCPATH)config.h
    $(CC) $(CFLAGS) $(INCLUDEFLAGS) $(SRCPATH)main.cpp -c main.o

cannonball.o: $(SRCPATH)cannonball.cpp $(SRCPATH)cannonball.h $(SRCPATH)screen.h
    $(CC) $(CFLAGS) $(INCLUDEFLAGS) $(SRCPATH)cannonball.cpp -c cannonball.o

config.o: $(SRCPATH)config.cpp $(SRCPATH)config.h $(SRCPATH)version.h
    $(CC) $(CFLAGS) $(INCLUDEFLAGS) $(SRCPATH)config.cpp -c config.o

screen.o: $(SRCPATH)screen.cpp $(SRCPATH)screen.h $(SRCPATH)image_ball.xpm $(SRCPATH)image_pixel.xpm
    $(CC) $(CFLAGS) $(INCLUDEFLAGS) $(SRCPATH)screen.cpp -c screen.o

tick.o: $(SRCPATH)tick.cpp $(SRCPATH)tick.h
    $(CC) $(CFLAGS) $(INCLUDEFLAGS) $(SRCPATH)tick.cpp -c tick.o

clean:
    rm *o $(EXECUTABLE)

