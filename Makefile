PROJECT_NAME = quadratic_equation
# flag and parameters
OS := $(shell uname -s)
CC_STANDART = -std=c11
CC_FLAGS = -Wall -Wextra -Werror
LIBS_CHECK = -lcheck
ifeq ($(OS), Linux)
	LIBS_CHECK += -lrt -lpthread -lsubunit -lm
endif
GCOV_FLAGS = -fprofile-arcs -ftest-coverage -coverage
# commands
CC = gcc $(CC_STANDART) $(CC_FLAGS)
RM := rm -rf
MKDIR := mkdir -p
# paths
TEST_DIR = test_src
BUILD_DIR = build
SOURCES_DIR = src
HELPERS_DIR = $(SOURCES_DIR)/helpers
OBJ_DIR = $(BUILD_DIR)/obj
GCOV_DIR = $(BUILD_DIR)/gcov
REPORTS_DIR = $(BUILD_DIR)/reports
# src files
TEST_FILES = $(TEST_DIR)/include.file $(TEST_DIR)/*.check
C_FILES = $(SOURCES_DIR)/*.c $(HELPERS_DIR)/*.c
H_FILES = $(SOURCES_DIR)/*.h $(HELPERS_DIR)/*.h
MAIN_C_FILE = $(SOURCES_DIR)/quadratic_equation.c
HELP_C_FILE = $(HELPERS_DIR)/quadeq_helpers.c
# build files
TEST_C_FILE = $(BUILD_DIR)/test.c
MAIN_O_FILE = $(OBJ_DIR)/$(patsubst %.c,%.o,$(notdir $(MAIN_C_FILE)))
HELP_O_FILE = $(OBJ_DIR)/$(patsubst %.c,%.o,$(notdir $(HELP_C_FILE)))
TEST_O_FILE = $(OBJ_DIR)/test.o
GCOV_INFO = $(GCOV_DIR)/gcov.info
# target files
LIB_FILE = lib$(PROJECT_NAME).a
TEST_TARGET = $(BUILD_DIR)/test.out
GCOV_TARGET = $(GCOV_DIR)/gcov_test.out
HTML_TARGET = $(REPORTS_DIR)/index.html

all: clean gcov_report

gcov_report: $(TEST_TARGET)
	@$(MKDIR) $(GCOV_DIR) $(REPORTS_DIR)
	@$(CC) -I $(GCOV_FLAGS) $(MAIN_C_FILE) $(HELP_C_FILE) $(TEST_O_FILE) -o $(GCOV_TARGET) $(LIBS_CHECK)
	$(GCOV_TARGET)
	@-lcov -t "tests" -o $(GCOV_INFO) -c -d .
	@-genhtml -o $(REPORTS_DIR) $(GCOV_INFO)
	@echo "You can open report in $(HTML_TARGET)"

check: $(TEST_TARGET)
	$(TEST_TARGET)

$(TEST_TARGET): $(LIB_FILE)
	@checkmk clean_mode=1 $(TEST_FILES) > $(TEST_C_FILE)
	@$(CC) -c $(TEST_C_FILE) -o $(TEST_O_FILE)
	@$(CC) $(GCOV_FLAGS) $(TEST_O_FILE) $(LIB_FILE) $(TEST_FLAGS) -o $(TEST_TARGET) $(LIBS_CHECK)
	$(RM) $(TEST_C_FILE)

$(LIB_FILE): o_files
	@ar rc $@ $(MAIN_O_FILE) $(HELP_O_FILE)
	@ranlib $@


o_files: $(MAIN_C_FILE) $(HELP_C_FILE)
	@$(MKDIR) $(BUILD_DIR)
	@$(MKDIR) $(OBJ_DIR)
	@$(CC) -c $(MAIN_C_FILE) -o $(MAIN_O_FILE)
	@$(CC) -c $(HELP_C_FILE) -o $(HELP_O_FILE)

clean:
	rm -rf  *.a  $(BUILD_DIR)

clang:
	clang-format -n -style=google $(C_FILES) $(H_FILES)

clangI:
	clang-format -i -style=google $(C_FILES) $(H_FILES)
	

rebuild: all

.PHONY: all rebuild o_files clean check gcov_report clang clangI