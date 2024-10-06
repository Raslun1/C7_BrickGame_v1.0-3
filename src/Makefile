CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

TET_LIB_C = ./brick_game/tetris/*.c
TET_LIB_H = ./brick_game/tetris/*.h
TET_LIB_O = backend.o fsm.o tetris_lib.o

CLI_C = ./gui/cli/*.c
CLI_H = ./gui/cli/*.h
CLI_O = frontend.o

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	OPEN = xdq-open
	CFLAGS += -D OS_LINUX
	TESTFLAGS = -lcheck  -lrt -lm -lsubunit
endif

ifeq ($(OS), Darwin)
	OPEN = open
	CFLAGS += -D OS_MAC -Wno-missing-braces
	TESTFLAGS = -lcheck
endif

all: clean install

install: tetris_lib.a
	$(CC) $(CFLAGS) -Wpedantic main.c ./gui/cli/frontend.c tetris_lib.a -lncurses -o tetris_game

uninstall:
	rm -rf tetris_game

tetris_lib.a:
	$(CC) -c $(CFLAGS) $(TET_LIB_C)
	ar rc tetris_lib.a $(TET_LIB_O)
	ranlib tetris_lib.a

test: clean tetris_lib.a
	gcc -g $(CFLAGS) test.c -o test tetris_lib.a $(TESTFLAGS)
	./test

dvi:
	doxygen Doxyfile
	open ./dvi/html/index.html

dist:
	tar -czf tetris.install.tar.gz ./* 

gcov_report: test
	gcc --coverage $(TET_LIB_C) test.c -o greport $(TESTFLAGS)
	./greport
	lcov -t "test" -o tetris.info -c -d .
	genhtml -o ./report tetris.info
	open ./report/index.html

valgrind: test
	valgrind --tool=memcheck --leak-check=yes ./test

clang:
	@cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.c *.h ./gui/cli/*.h ./gui/cli/*.c ./brick_game/tetris/*.c ./brick_game/tetris/*.h
	@rm .clang-format


clean:
	@rm -rf ./a.out ./*.o ./*.a ./test ./GcovReport ./report ./*.info ./*.gcno ./*gcda ./*.gcov ./report gcov_report* greport ./tetris_game
	-rm -rf html latex
	-rm -f tetris.install.tar.gz 
