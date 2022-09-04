#include "uart.h"
#include "spi.h"

void spiInit()
{
  // CS와 MOSI, SCK를 출력으로 설정합니다
  DDR_SPI |= (1 << CS) | (1 << MOSI) | (1 << SCK);

  // MISO를 Pull-up 레지스터로 활성화 합니다
  DDR_SPI |= (1 << MISO);

  // SPI를 활성화하고 Master로 Clock은 fOSC/128로 설정합니다.
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
  uartPuts("[INFO] SPI initialized!\n\r");
}

uint8_t spiTransfer(uint8_t data)
{
  // SPI로 전송할 데이터를 SPDR(SPI Data Register)에 저장
  SPDR = data;

  /*
    SPIF(SPI Interrupt Flag)가 Set될 때까지 기다립니다
    SPIF가 Set이 되었다면 SPI로 데이터가 전송이 완료된것입니다
  */
  while (!(SPSR & (1 << SPIF)))
    ;

  // SPDR을 반환합니다
  return SPDR;
}