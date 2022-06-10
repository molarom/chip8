VPATH = %.o objfiles

SRC = chip8_cpu.c chip8_mem.c chip8_sdl.c unit_test.c

CFLAGS = -Wall -Wextra 
SDLLIBS = `sdl2-config --cflags --libs`
UNITTEST = $(patsubst %.c,objfiles/%.o,$(SRC))

chip8: main.o $(OBJFILES)
	gcc $(CFLAGS) $? -o $@ $(SDLLIBS) 

test: $(UNITTEST)
	gcc -o $@ $(CFLAGS) $(UNITTEST) $(SDLLIBS)

debug_test: $(UNITTEST)
	gcc -g -fsanitize=address -o $@ $(CFLAGS) $(UNITTEST) $(SDLLIBS)

objfiles/chip8_cpu.o: chip8_cpu.c
	gcc -g -c $< -o $@

objfiles/chip8_mem.o: chip8_mem.c
	gcc -g -c $< -o $@

objfiles/chip8_sdl.o: chip8_sdl.c
	gcc -g -c $< -o $@

objfiles/unit_test.o: unit_test.c
	gcc -g -c $< -o $@

.PHONY:
	clean
clean :
	rm objfiles/*.o *.h.gch test debug* main 
