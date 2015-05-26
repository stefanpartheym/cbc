################################################################################
# cbc -- Codeblock compiler
################################################################################

TARGET                 := cbc
ifeq ($(OS), Windows_NT)
TARGET                 := $(TARGET).exe
endif

SRC_DIR                := src
OBJ_DIR                := obj
SOURCES                := main.c utils.c cb_utils.c
OBJECTS                := $(SOURCES:%.c=%.o)
OBJ                    := $(OBJECTS:%=$(OBJ_DIR)/%)

CFLAGS_COMMON          := -Wall -pedantic-errors -ansi
CFLAGS                 := -g $(CFLAGS_COMMON) -D DEBUG
CFLAGS_RELEASE         := $(CFLAGS_COMMON)
LDFLAGS                := 

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
