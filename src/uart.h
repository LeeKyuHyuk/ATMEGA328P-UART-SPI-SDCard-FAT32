#include <stdint.h>

void uartInit(uint16_t ubrr);
void uartPut(unsigned char data);
void uartPuts(char *s);
void uartPutHex8(uint8_t val);
void uartPutHex16(uint16_t val);
void uartPutU8(uint8_t val);
void uartPutS8(int8_t val);
void uartPutU16(uint16_t val);
void uartPutS16(int16_t val);
unsigned char uartGet(void);
void uartGetLine(char *buf, uint8_t n);