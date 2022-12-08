.PHONY: run
.PHONY: clean
.PHONY: build

FILES = main.c ecbp_file_format.c
FLAGS = -O2
C = gcc

build:
	${C} ${FLAGS} ${FILES} -o main.o

run:
	${C} ${FLAGS} ${FILES} -o main.o
	./main.o

clean:
	rm -rf *.0
