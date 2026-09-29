# I forget how to make a Makefile so theres probably a lot of redundancy here
# This is essentially my cisc210 final project Makefile but refactored for the cube

INCLUDE := -I ~/include
LDFLAGS := -L ~/lib -lsense -lm

all: rubiks
rubiks: main.o input.o display.o map.o cube.o
	cc -o rubiks main.o input.o display.o map.o cube.o $(LDFLAGS)

clean:
	rm -f *.o rubiks

main.o: main.c
	cc -c main.c $(INCLUDE)

input.o: input.c input.h
	cc -c input.c $(INCLUDE)

display.o: display.c display.h
	cc -c display.c $(INCLUDE)

map.o: map.c map.h
	cc -c map.c $(INCLUDE)
	
cube.o: cube.c cube.h
	cc -c cube.c $(INCLUDE)


# Copilots Makefile suggestion:

# CC := cc
# CFLAGS := -Wall -Wextra -std=c11 -I ~/include
# LDFLAGS := -L ~/lib -lsense -lm

# OBJ := main.o input.o display.o map.o cube.o

# all: rubiks

# rubiks: $(OBJ)
#     $(CC) -o rubiks $(OBJ) $(LDFLAGS)

# %.o: %.c
#     $(CC) $(CFLAGS) -c $<

# clean:
#     rm -f *.o rubiks
