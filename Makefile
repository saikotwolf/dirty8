INCLUDES = -I include/ -I /usr/include/SDL2/
FLAGS = -g
SDLFLAG = -lSDL2
DIRS = bin build

all: directories
	gcc ${FLAGS} ${INCLUDES} src/main.c -o bin/main ${SDLFLAG}

directories:
	mkdir -p ${DIRS}

clean:
	rm -rf bin/ build/
