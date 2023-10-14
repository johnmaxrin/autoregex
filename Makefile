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

EXECUTABLE := autoregex

all: $(OUTPUT_DIR) $(EXECUTABLE)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR) $(OUTPUT_SRC_DIR) 

$(LEX_C): $(LEX_FILE)
	lex -o $@ $<

$(YACC_C) $(YACC_H): $(YACC_FILE)
	yacc -d -o  $(YACC_C) $<

$(OUTPUT_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT_SRC_DIR)/%.o : src/%.c
	$(CC) -c $< -o $@

$(EXECUTABLE):  $(OUTPUT_DIR)/y.tab.o $(OUTPUT_DIR)/lex.yy.o $(SRC_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OUTPUT_DIR) $(EXECUTABLE)

.PHONY: all clean

