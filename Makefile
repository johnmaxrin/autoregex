OUTPUT_DIR := build
OUTPUT_SRC_DIR := $(OUTPUT_DIR)/src
LEX_FILE := src/parser/lexer.l
YACC_FILE := src/parser/parser.y
SRC_FILES := $(wildcard src/*.c)
SRC_OBJS  := $(patsubst src/%.c, $(OUTPUT_SRC_DIR)/%.o, $(SRC_FILES))

LEX_C := $(OUTPUT_DIR)/lex.yy.c
YACC_C := $(OUTPUT_DIR)/y.tab.c
YACC_H := $(OUTPUT_DIR)/y.tab.h

CC := gcc
CFLAGS := -g

EXECUTABLE := libregexapi.a

all: $(OUTPUT_DIR) $(EXECUTABLE)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR) $(OUTPUT_SRC_DIR) 

$(LEX_C): $(LEX_FILE)
	flex -o $@ $<

$(YACC_C) $(YACC_H): $(YACC_FILE)
	bison -d -o  $(YACC_C) $<

$(OUTPUT_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT_SRC_DIR)/%.o : src/%.c
	$(CC) -c -g $< -o $@

$(EXECUTABLE):  $(OUTPUT_DIR)/y.tab.o $(OUTPUT_DIR)/lex.yy.o $(SRC_OBJS)
	ar rcs $@ $^

clean:
	rm -rf $(OUTPUT_DIR) $(EXECUTABLE)

.PHONY: all clean

