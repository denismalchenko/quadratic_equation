PROJECT_NAME = quadratic_equation
# commands
CC = gcc
RM := rm -rf
MKDIR := mkdir -p
# flag and parameters
CFLAGS = -Wall #-Werror -Wextra
LINUX_FLAGS =-lrt -lpthread -lm
TEST_FLAGS = $(shell pkg-config --libs --cflags check)
LD_FLAGS = $(shell pkg-config --cflags check)
GCOV_FLAGS =--coverage

# paths and filenames
TEST_DIR = test
BUILD_DIR = build
SOURCES_DIR = src
HELPERS_DIR = $(SOURCES_DIR)/helpers
REPORTS = reports
TEST_FILES = $(TEST_DIR)/include.file $(TEST_DIR)/*.check
C_FILES = $(SOURCES_DIR)/*.c $(HELPERS_DIR)/*.c
H_FILES = $(SOURCES_DIR)/*.h $(HELPERS_DIR)/*.h


# CC = gcc
# CFLAGS = -Wall -Wextra -Werror -std=c11 -c
# OS := $(shell uname)
# TEST_FLAGS = $(shell pkg-config --cflags --libs check)
# C_FILES = quadratic_equation.c
# O_FILES = quadratic_equation.o

all: clean quadratic_equation.a test gcov_report

test: quadratic_equation.a
	@checkmk clean_mode=1 $(TEST_FILES) > test.c
	@clang-format -i --style=Google test.c
	@$(CC) -c $(CFLAGS) $(LD_FLAGS) $(TEST_FLAGS) $(GCOV_FLAGS) test.c -o test.o
	@$(CC) $(GCOV_FLAGS) test.o quadratic_equation.a $(TEST_FLAGS) -o testing
	./testing


quadratic_equation.a: o_file
	@ar rc $@ quadratic_equation.o helpers.o
	@ranlib $@


o_file: $(SOURCES_DIR)/quadratic_equation.c $(HELPERS_DIR)/quadeq_helpers.c
	@$(CC) $(CFLAGS) -c $(SOURCES_DIR)/quadratic_equation.c -o quadratic_equation.o
	@$(CC) $(CFLAGS) -c $(HELPERS_DIR)/quadeq_helpers.c -o helpers.o


clean:
	rm -rf *.o *.a *.out *.info *.html *.gc* report $(TEST_DIR)/test.c

gcov_report:
	$(CC) -fprofile-arcs -ftest-coverage test.c quadratic_equation.c -o gcov_report $(TEST_FLAGS)
	./gcov_report
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info

open:
ifeq ($(OS),Darwin)
	open -a "Google Chrome" report/src/index.html
else
	xdg-open report/src/index.html
endif

clang:
	clang-format -n -style=google *.c *.h

clangI:
	clang-format -i -style=google *.c *.h

leak:
ifeq ($(OS),Darwin)
	CK_FORK=no leaks -atExit -- ./test
else
	valgrind --leak-check=full -s --track-origins=yes ./test
endif
	

rebuild: all

.PHONY: all rebuild clean test report