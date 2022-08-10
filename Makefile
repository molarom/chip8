VPATH = %.o objfiles

OBJSRC = chip8_sdl.c chip8_cpu.c chip8_mem.c main.c

CFLAGS = -Wall -Wextra 
SDLLIBS = `sdl2-config --cflags --libs`
OBJFILES = $(patsubst %.c,objfiles/%.o,$(OBJSRC))

chip8: $(OBJFILES)
	gcc $(CFLAGS) $? -o $@ $(SDLLIBS) 


objfiles/main.o: main.c
	gcc -c $< -o $@

objfiles/chip8_cpu.o: chip8_cpu.c
	gcc -c $< -o $@

objfiles/chip8_mem.o: chip8_mem.c
	gcc -c $< -o $@

objfiles/chip8_sdl.o: chip8_sdl.c
	gcc -c $< -o $@

.PHONY:
	clean
clean :
	rm objfiles/*.o chip8 test debug*
