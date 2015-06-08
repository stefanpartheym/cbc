################################################################################
# cbc -- Codeblock compiler
################################################################################

TARGET                 := cbc
TARGET_TEST            := cbc_test
ifeq ($(OS), Windows_NT)
TARGET                 := $(TARGET).exe
TARGET_TEST            := $(TARGET_TEST).exe
endif

SRC_DIR                := src
TEST_DIR               := test
OBJ_DIR                := obj
OBJ_DIR_TEST           := obj/$(TEST_DIR)
MAIN                   := main.c
SOURCES                := utils.c cb_utils.c error_handling.c \
                          variant.c vector.c
OBJECTS                := $(SOURCES:%.c=%.o)
OBJ                    := $(MAIN:%.c=$(OBJ_DIR)/%.o) $(OBJECTS:%=$(OBJ_DIR)/%)
SOURCES_TEST           := test.c test_utils.c \
                          vector_test.c variant_test.c error_handling_test.c
OBJ_TEST               := $(SOURCES_TEST:%.c=$(OBJ_DIR_TEST)/%.o) \
                          $(OBJECTS:%=$(OBJ_DIR_TEST)/%)

CFLAGS_COMMON          := -Wall -pedantic-errors -ansi
CFLAGS                 := -g $(CFLAGS_COMMON) -D DEBUG
CFLAGS_RELEASE         := $(CFLAGS_COMMON)
LDFLAGS                := 
CFLAGS_TEST            := -I$(TEST_DIR) $(CFLAGS) -D inline=__inline
LDFLAGS_TEST           := -lcmocka $(LDFLAGS)

MKDIR                  := mkdir -p
GDB                    := gdb
VALGRIND               := valgrind
VALGRIND_OPTIONS       := --tool=memcheck
VALGRIND_OPTIONS_EXTRA := --leak-check=full --show-leak-kinds=all


# ------------------------------------------------------------------------------
# BUILD (main target)

# default target
default: debug

debug: build

release: CFLAGS := $(CFLAGS_RELEASE)
release: build

build: object_dir $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

# create object file directory
object_dir: $(OBJ_DIR)/
$(OBJ_DIR)/:
	$(MKDIR) $@
.PHONY: default debug release build object_dir


# ------------------------------------------------------------------------------
# EXECUTION

# execution of main target
run: build
	@./$(TARGET)

rund:     rundebug
rundebug: build
	$(GDB) ./$(TARGET)

.PHONY: run rund rundebug


# ------------------------------------------------------------------------------
# MEMORY-CHECK

# memory-check for main target
mc:       memcheck
memcheck: build
	$(VALGRIND) $(VALGRIND_OPTIONS) ./$(TARGET)

mcf:           memcheck-full
memcheck-full: VALGRIND_OPTIONS := $(VALGRIND_OPTIONS) $(VALGRIND_OPTIONS_EXTRA)
memcheck-full: build
	$(VALGRIND) $(VALGRIND_OPTIONS) ./$(TARGET)

.PHONY: mc memcheck mcf memcheck-full


# ------------------------------------------------------------------------------
# CLEANUP

# cleanup main target
clean:
	$(RM) $(TARGET) $(OBJ)

rebuild: clean default

.PHONY: clean rebuild


# ------------------------------------------------------------------------------
# TEST

# test target
test: build_test
build_test: $(OBJ_DIR_TEST)/ $(TARGET_TEST)

$(TARGET_TEST): $(OBJ_TEST)
	$(CC) -o $@ $^ $(LDFLAGS_TEST)

# build test object files
$(OBJ_DIR_TEST)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS_TEST) -o $@ -c $<

# build regular object files for test
$(OBJ_DIR_TEST)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS_TEST) -o $@ -c $<

# create object file directory
$(OBJ_DIR_TEST)/:
	$(MKDIR) $@

.PHONY: test build_test


# execution of test target
trun:     test-run
test-run: $(TARGET_TEST)
	@./$<

trund:         test-rundebug
test-rundebug: $(TARGET_TEST)
	$(GDB) ./$<

.PHONY: trun test-run trund test-rundebug


# memory-check for test target
tmc:           test-memcheck
test-memcheck: $(TARGET_TEST)
	$(VALGRIND) $(VALGRIND_OPTIONS) ./$<

tmcf:               test-memcheck-full
test-memcheck-full: VALGRIND_OPTIONS := $(VALGRIND_OPTIONS) --leak-check=full
test-memcheck-full: $(TARGET_TEST)
	$(VALGRIND) $(VALGRIND_OPTIONS) ./$<

.PHONY: tmc test-memcheck tmcf test-memcheck-full


# cleanup test target
tclean: test-clean
test-clean:
	$(RM) $(TARGET_TEST) $(OBJ_TEST)

# cleanup all targets
aclean: clean tclean

.PHONY: tclean test-clean
.PHONY: aclean
