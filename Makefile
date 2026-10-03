INCLUDE := -I ~/include
LDFLAGS := -L ~/lib -lsense -lm

SRC := main.c $(wildcard src/*.c)

all: rubiks

rubiks: $(SRC) $(wildcard include/*.h)
	cc -o rubiks $(SRC) $(INCLUDE) $(LDFLAGS)

clean:
	rm -f rubiks

# Regenerate compile_commands.json from the real build for LSPs.
compile_commands.json:
	bear -- $(MAKE) clean all

.PHONY: all clean compile_commands.json
