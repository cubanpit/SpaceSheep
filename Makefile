#
# SpaceSheep, ncurses game. Code is hosted on GitHub.
#
# File: Makefile
#
# Authors: 
#  Martina Crippa 				<martina.crippa2@studenti.unimi.it>
#  Pietro Francesco Fontana 	<pietrofrancesco.fontana@studenti.unimi.it>
#
################################################################################
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
################################################################################

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
