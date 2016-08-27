########## 
#
#  File: Makefile
#
#  Authors: 
#	Martina Crippa 				<martina.crippa2@studenti.unimi.it>
#	Pietro Francesco Fontana 	<pietrofrancesco.fontana@studenti.unimi.it>
#

OS := $(shell uname)
ifeq ($(OS), Linux)
	export MAKEFLAGS="-j $(grep -c ^processor /proc/cpuinfo)" \
else($(OS), Darwin)
	export MAKEFLAGS="-j $(sysctl-n hw.ncpu)" \
else
	#Windows? 
endif

VPATH=./source/
OBJPATH=./obj/

TARGET := game
OBJ:= obstacle.o hitbox.o sketcher.o engine.o

DEBUG := -g

CXXFLAGS := $(CXXFLAGS) -std=c++11 -lncurses

all: $(TARGET)

game: main.o $(OBJ)
	$(CXX) $(OBJPATH)*.o -o $@ $(CXXFLAGS) $(DEBUG)

%.o: %.cpp
ifeq ($(wildcard $(OBJPATH)*),) #search for obj path, create it if it doesn't exist
	@mkdir -p $(OBJPATH)
endif
	$(CXX) -c $< -o $(OBJPATH)$@ $(CXXFLAGS) $(DEBUG)

.PHONY: run clean

run:
	./$(TARGET)

clean:
	/bin/rm -f $(OBJPATH)*.o
	/bin/rm -f $(TARGET)
