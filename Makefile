INCLUDES = -I include/ -I /usr/include/SDL2/
FLAGS = -g
SDLFLAG = -lSDL2
DIRS = bin build
OBJECTS = build/chip8.o

all: directories ${OBJECTS}
	gcc ${FLAGS} ${INCLUDES} src/main.c ${OBJECTS} -o bin/main ${SDLFLAG}

directories:
	mkdir -p ${DIRS}

build/chip8.o:src/chip8.c
	gcc ${FLAGS} ${INCLUDES} src/chip8.c -c -o build/chip8.o

clean:
	rm -rf bin/ build/
