TARGET=main

CC=msp430-gcc
CFLAGS=-Wall -mmcu=msp430g2452 # -Wl,-Map=mapfile.map

OBJS=main.o spi.o display.o delay.o

all: $(OBJS)
	$(CC) $(CFLAGS) -o main.elf $(OBJS)
#	msp430-objdump -DS main.elf > main.lst
	msp430-size --target=elf32-msp430 main.elf

program: $(TARGET).elf
	mspdebug rf2500 "prog $(TARGET).elf"

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -fr main.elf $(OBJS)
