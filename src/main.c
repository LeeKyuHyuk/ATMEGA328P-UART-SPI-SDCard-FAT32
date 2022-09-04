#include <avr/interrupt.h>
#include "uart.h"
/*
  CPU의 Frequency를 16MHz로 설정합니다
*/
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
    // Buffer를 할당합니다.
    const uint8_t bufSize = 20;
    char buf[bufSize];

    // UART 전송속도를 설정합니다
    const unsigned int baudRate = (F_CPU / 16 / 9600) - 1;

    _delay_ms(100); // VCC 안정화를 위해 100ms 동안 대기합니다

    cli(); // 모든 인터럽트를 비활성화 합니다

    // UART를 초기화 합니다
    uartInit(baudRate);

    // UART를 통해 데이터의 전체라인을 수신합니다
    uartPuts("Please enter any text : ");
    uartGetLine(buf, bufSize);

    // 수신한 데이터를 출력합니다
    uartPuts("\n\r");
    uartPuts("You entered : ");
    uartPuts(buf);
    uartPuts("\n\r");

    return 0;
}