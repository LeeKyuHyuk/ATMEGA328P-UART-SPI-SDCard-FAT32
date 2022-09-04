#include <avr/interrupt.h>
/*
  CPU의 Frequency를 16MHz로 설정합니다
*/
#define F_CPU 16000000UL
#include <util/delay.h>
#include "uart.h"
#include "spi.h"
#include "sd.h"

int main(void)
{
  // UART 전송속도를 설정합니다
  const unsigned int baudRate = (F_CPU / 16 / 9600) - 1;

  _delay_ms(100); // VCC 안정화를 위해 100ms 동안 대기합니다

  cli(); // 모든 인터럽트를 비활성화 합니다

  // UART를 초기화 합니다
  uartInit(baudRate);

  // SPI 초기화
  spiInit();

  // SDCard 초기화
  if (sdInit() == SD_SUCCESS)
  {
    uartPuts("[INFO] SDCard initialized!\n\r");
  }
  else
  {
    uartPuts("[ERROR] Error initializaing SD CARD!\n\r");
  }

  while (1)
    ;

  return 0;
}