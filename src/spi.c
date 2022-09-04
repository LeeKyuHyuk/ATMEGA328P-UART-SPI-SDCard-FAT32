#include "spi.h"

void spiInit()
{
    // CS와 MOSI, SCK를 출력으로 설정합니다
    SPI_DDR |= (1 << CS) | (1 << MOSI) | (1 << SCK);

    // SPI를 활성화하고 Master로 Clock은 fOSC/128로 설정합니다.
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}

void spiTransmitByte(uint8_t data)
{
    // SPI로 전송할 데이터를 SPDR(SPI Data Register)에 저장
    SPDR = data;

    /*
      SPIF(SPI Interrupt Flag)가 Set될 때까지 기다립니다
      SPIF가 Set이 되었다면 SPI로 데이터가 전송이 완료된것입니다
    */
    while (!(SPSR & (1 << SPIF)))
        ;
}

uint8_t spiReceive()
{
    /*
      SPI에서 데이터를 전송하면 항상 0xFF를 전송하고 SPI 데이터 레지스터를
      반환합니다. 슬레이브가 데이터를 다시 전송하기 위해 클럭을 생성하기
      때문에 0xFF를 전송하는데, 이것을 사용하여 SPDR에 0xFF를 Dummy Byte로
      전송하면 SPI 수신을 받을 수 있습니다
    */
    SPDR = 0xFF;

    /*
      SPIF(SPI Interrupt Flag)가 Set될 때까지 기다립니다
      SPIF가 Set이 되었다면 SPI로 데이터가 전송이 완료된것입니다
    */
    while (!(SPSR & (1 << SPIF)))
        ;

    // SPI로 수신한 데이터(SPDR)을 반환합니다
    return SPDR;
}

uint8_t spiTxRx(uint8_t data)
{
    // SPI로 전송할 데이터를 SPDR(SPI Data Register)에 저장
    SPDR = data;

    /*
      SPIF(SPI Interrupt Flag)가 Set될 때까지 기다립니다
      SPIF가 Set이 되었다면 SPI로 데이터가 전송이 완료된것입니다
    */
    while (!(SPSR & (1 << SPIF)))
        ;

    // SPI로 수신한 데이터(SPDR)을 반환합니다
    return SPDR;
}