DEVICE  = atmega328p
F_CPU   = 8000000	# edit this line for crystal speed, in Hz

OBJECTS = adc.o buttons.o gtimer.o leds.o main.o

CC = avr-gcc -Os -Wall -DF_CPU=$(F_CPU) -mmcu=$(DEVICE) $(CFLAGS)

default: main.hex

clean:
	rm -f main.hex main.lst main.obj main.cof main.list main.map main.eep.hex main.elf *.o main.s

# Generic rule for compiling C files:
.c.o:
	$(CC) -c $< -o $@

# Generic rule for assembling Assembler source files:
.S.o:
	$(CC) -x assembler-with-cpp -c $< -o $@

# Generic rule for compiling C to assembler, used for debugging only.
.c.s:
	$(CC) -S $< -o $@

# file targets:

main.elf: $(OBJECTS)
	$(CC) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex main.eep.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size main.hex

# debugging targets:

disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(CC) -E main.c
