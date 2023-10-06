SRC_FILES := $(wildcard src/*.c)
SRC_OBJS := $(patsubst src/%.c, build/%.o,$(SRC_FILES))

all : $(SRC_OBJS)
	gcc $(SRC_OBJS) -o autoregex

build/%.o : src/%.c
	gcc -c $< -o $@

clean:
	rm build/*.o autoregex