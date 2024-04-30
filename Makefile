PROJECT_NAME = quadratic_equation
# tools
OS := $(shell uname -s)
GCOVR_TOOL = $(shell which gcovr)
LCOV_TOOL = $(shell which lcov)
# flag and parameters
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
DOXY_SRC_DIR = docs_src
DOXY_DIR = documentation
# src files
TEST_FILES = $(TEST_DIR)/include.file $(TEST_DIR)/*.check
C_FILES = $(SOURCES_DIR)/*.c $(HELPERS_DIR)/*.c
H_FILES = $(SOURCES_DIR)/*.h $(HELPERS_DIR)/*.h
MAIN_C_FILE = $(SOURCES_DIR)/quadratic_equation.c
HELP_C_FILE = $(HELPERS_DIR)/quadeq_helpers.c
DOXY_BRIEF = doxygen_brief.conf
DOXY_FULL = doxygen_full.conf
ICON = favicon.ico
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
REPORT_TARGET = $(REPORTS_DIR)/index.html
DOXY_TARGET = $(DOXY_DIR)/index.html
# marco for generating doxygen documentation
define gen_docs
  @cp -r $(DOXY_SRC_DIR)/$(1) $(1)
	@doxygen $(1)
	@$(RM) $(1)
	@cp -r $(DOXY_SRC_DIR)/$(ICON) $(DOXY_DIR)/$(ICON)
	@echo "Documentation has been generated. You can access the documentation by opening $(DOXY_TARGET)"
endef

all: clean $(LIB_FILE) dvi

gcov_report: $(TEST_TARGET)
	@$(MKDIR) $(GCOV_DIR) $(REPORTS_DIR)
	@$(CC) -I $(GCOV_FLAGS) $(MAIN_C_FILE) $(HELP_C_FILE) $(TEST_O_FILE) -o $(GCOV_TARGET) $(LIBS_CHECK)
	$(GCOV_TARGET)
ifeq (,$(GCOVR_TOOL))
ifeq (,$(LCOV_TOOL))
	$(error "Neither gcovr nor lcov is installed. Please install one of them.")
else
	@-lcov -t "tests" -o $(GCOV_INFO) -c -d .
	@-genhtml -o $(REPORTS_DIR) $(GCOV_INFO)
	@echo "Using lcov for generating coverage reports. You can find it at $(REPORT_TARGET)"
endif
else
	@gcovr --html-details --html-self-contained -o $(REPORT_TARGET) $(GCOV_DIR)
	@echo "Using gcovr for generating coverage reports. You can find it at $(REPORT_TARGET)"
endif
	

check: $(TEST_TARGET)
	$(TEST_TARGET)

$(TEST_TARGET): $(LIB_FILE)
	@checkmk clean_mode=1 $(TEST_FILES) > $(TEST_C_FILE)
	@$(CC) -c $(TEST_C_FILE) -o $(TEST_O_FILE)
	@$(CC) $(GCOV_FLAGS) $(TEST_O_FILE) $(LIB_FILE) $(TEST_FLAGS) -o $(TEST_TARGET) $(LIBS_CHECK)
	@$(RM) $(TEST_C_FILE)

$(LIB_FILE): o_files
	@ar rc $@ $(MAIN_O_FILE) $(HELP_O_FILE)
	@ranlib $@

dvi:
	$(call gen_docs,$(DOXY_BRIEF))

dvi_full:
	$(call gen_docs,$(DOXY_FULL)) 

o_files: $(MAIN_C_FILE) $(HELP_C_FILE)
	@$(MKDIR) $(BUILD_DIR)
	@$(MKDIR) $(OBJ_DIR)
	@$(CC) -c $(MAIN_C_FILE) -o $(MAIN_O_FILE)
	@$(CC) -c $(HELP_C_FILE) -o $(HELP_O_FILE)

clean:
	rm -rf  *.a  $(BUILD_DIR) $(DOXY_DIR)

clang:
	clang-format -n -style=google $(C_FILES) $(H_FILES)

clangI:
	clang-format -i -style=google $(C_FILES) $(H_FILES)
	

rebuild: all

.PHONY: all rebuild o_files clean check gcov_report clang clangI dvi make_docs_dir