# SpaceSheep

Simple game written in C++11 using `ncurses` library, it has a single player and
a two players mode.

The two players mode requires a network connection, by default it uses port
58432/UDP and it asks for users local IP addresses before starting.

The network is configured using Linux libraries, if you want to play this
game with other operating systems you have to modify the code.

## Building and Coding

### Requirements

To compile this program you need a modern compiler that supports C++11 standard
(a.e. `g++ >= 4.8.1`) and any version of `ncurses` library.

### Compiling

A simple `make` in the directory containing `Makefile`.

### Code Convention

The code is indented using 2 spaces.

## Authors

Martina Crippa <martina.crippa2@studenti.unimi.it>

Pietro F. Fontana <pietrofrancesco.fontana@studenti.unimi.it>

The code is distributed under the  GNU GENERAL PUBLIC LICENSE 3.0.
