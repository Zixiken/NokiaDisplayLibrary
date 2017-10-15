# Set the variable below to your avr toolchain's bin directory
toolchainBin :=
AR := ${toolchainBin}/avr-gcc-ar
CC := ${toolchainBin}/avr-gcc

CFLAGS := -I include -mmcu=atmega2560 -DF_CPU=16000000 -Os -Wall -Werror -flto \
	-fno-fat-lto-objects -ffunction-sections -fdata-sections

libname := libnokiadisplay.a
objs := $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
nokiaDisplayHDeps := libnokiadisplay

${libname} : ${objs}
	${AR} rcs $@ ${objs}

${objs} : obj/%.o : src/%.c
	${CC} -c ${CFLAGS} $< -o $@

$(patsubst %, obj/%.o, ${nokiaDisplayHDeps}) : include/libnokiadisplay.h

.PHONY : clean

clean :
	rm -rf ${objs} ${libname}
