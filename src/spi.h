#include <avr/io.h>

#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define CS PINB2
#define MOSI PINB3
#define MISO PINB4
#define SCK PINB5

#define CS_ENABLE() PORT_SPI &= ~(1 << CS)
#define CS_DISABLE() PORT_SPI |= (1 << CS)

void spiInit();
uint8_t spiTransfer(uint8_t data);