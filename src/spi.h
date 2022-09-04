#include <avr/io.h>

#define SPI_DDR DDRB
#define CS PINB2
#define MOSI PINB3
#define MISO PINB4
#define SCK PINB5

void spiInit();
void spiTransmitByte(uint8_t data);
uint8_t spiReceive();
uint8_t spiTxRx(uint8_t data);