# Set the variable below to your avr toolchain's bin directory
toolchainBin := 
AR := ${toolchainBin}/avr-ar
AS := ${toolchainBin}/avr-as
CC := ${toolchainBin}/avr-gcc
CXX := ${toolchainBin}/avr-g++
CFLAGS := -I include -mmcu=atmega2560

objs := $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
mainHDeps := main

libNokiaDisplay.a : ${objs}
	${AR} rcs $@ ${objs}

${objs} : obj/%.o : src/%.c
	${CC} -c ${CFLAGS} $< -o $@

$(patsubst %, obj/%.o, ${mainHDeps}) : include/main.h

.PHONY : clean

clean :
	rm -rf ${objs} libNokiaDisplay.a
