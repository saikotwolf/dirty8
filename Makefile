INCLUDES = -I include/ -I /usr/include/SDL2/
FLAGS = -g
SDLFLAG = -lSDL2
DIRS = bin build
OBJECTS = build/chip8.o build/stack.o build/memory.o build/screen.o

all: directories ${OBJECTS}
	gcc ${FLAGS} ${INCLUDES} src/main.c ${OBJECTS} -o bin/main ${SDLFLAG}

directories:
	mkdir -p ${DIRS}

build/chip8.o:src/chip8.c
	gcc ${FLAGS} ${INCLUDES} src/chip8.c -c -o build/chip8.o

build/stack.o:src/stack.c
	gcc ${FLAGS} ${INCLUDES} src/stack.c -c -o build/stack.o

build/memory.o:src/memory.c
	gcc ${FLAGS} ${INCLUDES} src/memory.c -c -o build/memory.o

build/screen.o:src/screen.c
	gcc ${FLAGS} ${INCLUDES} src/screen.c -c -o build/screen.o

clean:
	rm -rf bin/ build/
