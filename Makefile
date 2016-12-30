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

# configure multithreaded compilation
OS := $(shell uname)
ifeq ($(OS), Linux)
	export MAKEFLAGS="-j $(grep -c ^processor /proc/cpuinfo)"
else ifeq ($(OS), Darwin)
	export MAKEFLAGS="-j $(sysctl-n hw.ncpu)"
endif

# search in VPATH for source file
VPATH=./source/
# place object file in OBJPATH
OBJPATH=./obj/

GCC47_BINDIR :=
# configure compilator path in tolab
#GCC47_BINDIR := /opt/centos/devtoolset-1.1/root/usr/bin/

CXX := $(GCC47_BINDIR)$(CXX)

TARGET := game
OBJS := $(patsubst %.o,$(OBJPATH)%.o, obstacle.o hitbox.o sketcher.o engine.o \
	prizegive.o	UDPMcastSender.o UDPMcastReceiver.o UDPSSMcast.o)

DEBUG := -g
WARNING := -Wall -Wextra

CXXFLAGS := $(CXXFLAGS) -std=c++11
LDFLAGS := -lncurses -lpthread
# libraries for compilation on old system (tolab)
#LDFLAGS := -lncurses -ltinfo -lrt -lpthread

all: $(TARGET)

game: $(OBJS) $(OBJPATH)main.o
	$(CXX) -o $@ $(OBJS) $(OBJPATH)main.o $(CXXFLAGS) $(LDFLAGS)

$(OBJPATH)main.o: main.cpp $(OBJS)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJPATH)%.o: %.cpp %.h
ifeq ($(wildcard $(OBJPATH)*),) #search for obj path, create it if it doesn't exist
	@mkdir -p $(OBJPATH)
endif
	$(CXX) -c $< -o $@ $(CXXFLAGS)

.PHONY: run clean

run:
	./$(TARGET)

clean:
	/bin/rm -f $(OBJPATH)*.o
	/bin/rm -f $(TARGET)
