ARCH		:= arm
APP_OUT		:= neon



all: default

default:
	g++ -std=c++11 x64.cpp -o x64.out
	$(CROSS_COMPILE)g++ -std=c++11 -mfpu=neon -c main.c
	$(CROSS_COMPILE)gcc -c c_keccak.c
	$(CROSS_COMPILE)gcc main.o -o $(APP_OUT).out
