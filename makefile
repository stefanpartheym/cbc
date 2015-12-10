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

LEXER_NAME             := cbc_lexer
PARSER_NAME            := cbc_parser
LEXER                  := $(SRC_DIR)/$(LEXER_NAME).l
PARSER                 := $(SRC_DIR)/$(PARSER_NAME).y
LEXER_SRC              := $(LEXER_NAME).c
PARSER_SRC             := $(PARSER_NAME).c
LEXER_INC              := $(LEXER_NAME).h
PARSER_INC             := $(PARSER_NAME).h
LEXER_AND_PARSER_FILES := $(SRC_DIR)/$(LEXER_NAME).c $(SRC_DIR)/$(LEXER_NAME).h \
                          $(SRC_DIR)/$(PARSER_NAME).c $(SRC_DIR)/$(PARSER_NAME).h

MAIN                   := main.c
SOURCES                := $(LEXER_SRC) $(PARSER_SRC) \
                          utils.c cb_utils.c error_handling.c \
                          variant.c vector.c stack.c hash_table.c \
                          ast.c ast_binary.c ast_unary.c ast_variable.c \
                          ast_value.c ast_declaration.c ast_statement_list.c \
                          ast_declaration_block.c ast_assignment.c \
                          scope.c symbol.c symbol_variable.c symbol_function.c \
                          symbol_table.c \
                          codeblock.c
OBJECTS                := $(SOURCES:%.c=%.o)
OBJ                    := $(MAIN:%.c=$(OBJ_DIR)/%.o) $(OBJECTS:%=$(OBJ_DIR)/%)
SOURCES_TEST           := test.c test_utils.c \
                          vector_test.c variant_test.c error_handling_test.c \
                          stack_test.c hash_table_test.c symbol_table_test.c \
                          ast_test.c symbol_test.c codeblock_test.c
OBJ_TEST               := $(SOURCES_TEST:%.c=$(OBJ_DIR_TEST)/%.o) \
                          $(OBJECTS:%=$(OBJ_DIR_TEST)/%)

CFLAGS_COMMON          := -Wall -std=c99 -pedantic -pedantic-errors
CFLAGS                 := -g $(CFLAGS_COMMON) -D DEBUG
CFLAGS_RELEASE         := $(CFLAGS_COMMON)
LDFLAGS                := 
CFLAGS_TEST            := $(CFLAGS)
LDFLAGS_TEST           := -lcmocka $(LDFLAGS)

MKDIR                  := mkdir -p
GDB                    := gdb
LEX                    := flex
YACC                   := bison
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

build: object_dir build_lexer_and_parser $(TARGET)

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
# LEXER AND PARSER BUILD

build_lexer_and_parser: lexer parser

lexer: $(SRC_DIR)/$(LEXER_SRC)
$(SRC_DIR)/$(LEXER_SRC): $(LEXER)
	$(LEX) --header-file=$(SRC_DIR)/$(LEXER_INC) -o $@ $<

parser: $(SRC_DIR)/$(PARSER_SRC)
$(SRC_DIR)/$(PARSER_SRC): $(PARSER)
	$(YACC) -d -o $@ $<

.PHONY: build_lexer_and_parser lexer parser


# ------------------------------------------------------------------------------
# EXECUTION

# execution of main target
run: build
	@./$(TARGET) $(p)

rund:     rundebug
rundebug: build
	$(GDB) ./$(TARGET) $(p)

.PHONY: run rund rundebug


# ------------------------------------------------------------------------------
# MEMORY-CHECK

# memory-check for main target
mc:       memcheck
memcheck: build
	$(VALGRIND) $(VALGRIND_OPTIONS) ./$(TARGET)

mcf:           memcheck-full
memcheck-full: VALGRIND_OPTIONS += $(VALGRIND_OPTIONS_EXTRA)
memcheck-full: build
	$(VALGRIND) $(VALGRIND_OPTIONS) ./$(TARGET)

.PHONY: mc memcheck mcf memcheck-full


# ------------------------------------------------------------------------------
# CLEANUP

# cleanup main target
clean:
	$(RM) $(TARGET) $(OBJ) $(LEXER_AND_PARSER_FILES)

rebuild: clean default

.PHONY: clean rebuild


# ------------------------------------------------------------------------------
# TEST

# test target
test: build_test
build_test: $(OBJ_DIR_TEST)/ build_lexer_and_parser $(TARGET_TEST)

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
test-run: test
	@./$(TARGET_TEST)

trund:         test-rundebug
test-rundebug: test
	$(GDB) ./$(TARGET_TEST)

.PHONY: trun test-run trund test-rundebug


# memory-check for test target
tmc:           test-memcheck
test-memcheck: test
	$(VALGRIND) $(VALGRIND_OPTIONS) ./$(TARGET_TEST)

tmcf:               test-memcheck-full
test-memcheck-full: VALGRIND_OPTIONS += $(VALGRIND_OPTIONS_EXTRA)
test-memcheck-full: test
	$(VALGRIND) $(VALGRIND_OPTIONS) ./$(TARGET_TEST)

.PHONY: tmc test-memcheck tmcf test-memcheck-full


# cleanup test target
tclean: test-clean
test-clean:
	$(RM) $(TARGET_TEST) $(OBJ_TEST) $(LEXER_AND_PARSER_FILES)

# cleanup all targets
aclean: clean tclean

.PHONY: tclean test-clean
.PHONY: aclean
