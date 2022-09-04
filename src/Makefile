CC = avr-gcc
CFLAGS = -Wall -Os -mmcu=atmega328p
OBJCOPY = avr-objcopy

OBJ = main.o uart.o spi.o sd.o

all: atmega328p-uart-spi-sdcard-fat32.hex

%.o: %.c
	$(CC) $(CFLAGS) -c $<

atmega328p-uart-spi-sdcard-fat32.elf: $(OBJ)
	$(CC) $(CFLAGS) -o atmega328p-uart-spi-sdcard-fat32.elf $(OBJ)

atmega328p-uart-spi-sdcard-fat32.hex: atmega328p-uart-spi-sdcard-fat32.elf
	$(OBJCOPY) atmega328p-uart-spi-sdcard-fat32.elf -O ihex atmega328p-uart-spi-sdcard-fat32.hex

clean:
	rm -f *.o *.elf *.hex
