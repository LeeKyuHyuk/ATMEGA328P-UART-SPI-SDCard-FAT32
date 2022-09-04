/* CPU의 Frequency를 16MHz로 설정합니다 */
#define F_CPU 16000000UL
#include <util/delay.h>
#include "uart.h"
#include "spi.h"
#include "sd.h"

void sdPowerUpSeq()
{
    // SDCard CS Assert
    CS_ENABLE();

    // SDCard에 전원이 공급되는데 최소로 필요한 1msec의 Dealy 실행
    _delay_ms(1);

    /*
      동기화하기 위해 80개의 클럭사이클을 보냅니다
      SPI는 직렬 통신이기 때문에 각 클럭 사이클당 1비트의 데이터가 전송됩니다
      0xFF(11111111b)는 8비트이기 때문에 10번을 보내면
      80개의 클럭 사이클을 보내는 것과 같습니다
    */
    for (uint8_t i = 0; i < 10; i++)
        spiTransfer(0xFF);

    // SDCard CS Deassert
    CS_DISABLE();
    spiTransfer(0xFF);
}

void sdCommand(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    // SDCard에 Command를 전송합니다
    spiTransfer(cmd | 0x40);

    // Argument를 전송합니다
    spiTransfer((uint8_t)(arg >> 24));
    spiTransfer((uint8_t)(arg >> 16));
    spiTransfer((uint8_t)(arg >> 8));
    spiTransfer((uint8_t)(arg));

    // CRC를 전송합니다
    spiTransfer(crc | 0x01);
}

uint8_t sdReadRes1()
{
    uint8_t index = 0, res1;

    // 실제 데이터가 수신될 떄까지 폴링을 유지합니다.
    while ((res1 = spiTransfer(0xFF)) == 0xFF)
    {
        index++;
        // 8바이트 동안 수신된 데이터가 없으면 멈춥니다
        if (index > 8)
            break;
    }

    return res1;
}

uint8_t sdGoIdleState()
{
    // SDCard CS Assert
    spiTransfer(0xFF);
    CS_ENABLE();
    spiTransfer(0xFF);

    // CMD0를 전송합니다
    sdCommand(CMD0, CMD0_ARG, CMD0_CRC);

    // Response를 읽습니다
    uint8_t res1 = sdReadRes1();

    // SDCard CS Deassert
    spiTransfer(0xFF);
    CS_DISABLE();
    spiTransfer(0xFF);

    return res1;
}

void sdReadRes3Res7(uint8_t *res)
{
    // R1 Response를 읽습니다
    res[0] = sdReadRes1();

    /*
      R1 Response에 Error가 존재하면
      더 이상 진행하지 않고 반환합니다
    */
    if (res[0] > 1)
        return;

    /*
      R1 Response에 Error가 없다면
      남은 바이트를 읽습니다
    */
    res[1] = spiTransfer(0xFF);
    res[2] = spiTransfer(0xFF);
    res[3] = spiTransfer(0xFF);
    res[4] = spiTransfer(0xFF);
}

void sdSendIfCond(uint8_t *res)
{
    // SDCard CS Assert
    spiTransfer(0xFF);
    CS_ENABLE();
    spiTransfer(0xFF);

    // CMD8 전송
    sdCommand(CMD8, CMD8_ARG, CMD8_CRC);

    // CMD8에 대한 Rsponse를 읽습니다
    sdReadRes3Res7(res);

    // SDCard CS Deassert
    spiTransfer(0xFF);
    CS_DISABLE();
    spiTransfer(0xFF);
}

void sdPrintR1(uint8_t res)
{
    if (res & 0b10000000)
    {
        uartPuts("\tError: MSB = 1\r\n");
        return;
    }
    if (res == 0)
    {
        uartPuts("\tCard Ready\r\n");
        return;
    }
    if (PARAM_ERROR(res))
        uartPuts("\tParameter Error\r\n");
    if (ADDR_ERROR(res))
        uartPuts("\tAddress Error\r\n");
    if (ERASE_SEQ_ERROR(res))
        uartPuts("\tErase Sequence Error\r\n");
    if (CRC_ERROR(res))
        uartPuts("\tCRC Error\r\n");
    if (ILLEGAL_CMD(res))
        uartPuts("\tIllegal Command\r\n");
    if (ERASE_RESET(res))
        uartPuts("\tErase Reset Error\r\n");
    if (IN_IDLE(res))
        uartPuts("\tIn Idle State\r\n");
}

void sdPrintR7(uint8_t *res)
{
    sdPrintR1(res[0]);

    if (res[0] > 1)
        return;

    uartPuts("\tCommand Version: ");
    uartPutHex8(CMD_VER(res[1]));
    uartPuts("\r\n");

    uartPuts("\tVoltage Accepted: ");
    if (VOL_ACC(res[3]) == VOLTAGE_ACC_27_33)
        uartPuts("2.7-3.6V\r\n");
    else if (VOL_ACC(res[3]) == VOLTAGE_ACC_LOW)
        uartPuts("LOW VOLTAGE\r\n");
    else if (VOL_ACC(res[3]) == VOLTAGE_ACC_RES1)
        uartPuts("RESERVED\r\n");
    else if (VOL_ACC(res[3]) == VOLTAGE_ACC_RES2)
        uartPuts("RESERVED\r\n");
    else
        uartPuts("NOT DEFINED\r\n");

    uartPuts("\tEcho: ");
    uartPutHex8(res[4]);
    uartPuts("\r\n");
}

void sdReadOcr(uint8_t *res)
{
    // SDCard CS Assert
    spiTransfer(0xFF);
    CS_ENABLE();
    spiTransfer(0xFF);

    // CMD58 전송
    sdCommand(CMD58, CMD58_ARG, CMD58_CRC);

    // Response를 읽습니다
    sdReadRes3Res7(res);

    // SDCard CS Deassert
    spiTransfer(0xFF);
    CS_DISABLE();
    spiTransfer(0xFF);
}

void sdPrintR3(uint8_t *res)
{
    sdPrintR1(res[0]);

    if (res[0] > 1)
        return;

    uartPuts("\tCard Power Up Status: ");
    if (POWER_UP_STATUS(res[1]))
    {
        uartPuts("READY\r\n");
        uartPuts("\tCCS Status: ");
        if (CCS_VAL(res[1]))
        {
            uartPuts("1\r\n");
        }
        else
            uartPuts("0\r\n");
    }
    else
    {
        uartPuts("BUSY\r\n");
    }

    uartPuts("\tVDD Window: ");
    if (VDD_2728(res[3]))
        uartPuts("2.7-2.8, ");
    if (VDD_2829(res[2]))
        uartPuts("2.8-2.9, ");
    if (VDD_2930(res[2]))
        uartPuts("2.9-3.0, ");
    if (VDD_3031(res[2]))
        uartPuts("3.0-3.1, ");
    if (VDD_3132(res[2]))
        uartPuts("3.1-3.2, ");
    if (VDD_3233(res[2]))
        uartPuts("3.2-3.3, ");
    if (VDD_3334(res[2]))
        uartPuts("3.3-3.4, ");
    if (VDD_3435(res[2]))
        uartPuts("3.4-3.5, ");
    if (VDD_3536(res[2]))
        uartPuts("3.5-3.6");
    uartPuts("\r\n");
}

uint8_t sdSendApp()
{
    // SDCard CS Assert
    spiTransfer(0xFF);
    CS_ENABLE();
    spiTransfer(0xFF);

    // CMD55 전송
    sdCommand(CMD55, CMD55_ARG, CMD55_CRC);

    // Response를 읽습니다
    uint8_t r1 = sdReadRes1();

    // SDCard CS Deassert
    spiTransfer(0xFF);
    CS_DISABLE();
    spiTransfer(0xFF);

    return r1;
}

uint8_t sdSendOpCond()
{
    // SDCard CS Assert
    spiTransfer(0xFF);
    CS_ENABLE();
    spiTransfer(0xFF);

    // ACMD41 전송
    sdCommand(ACMD41, ACMD41_ARG, ACMD41_CRC);

    // Response를 읽습니다
    uint8_t r1 = sdReadRes1();

    // SDCard CS Deassert
    spiTransfer(0xFF);
    CS_DISABLE();
    spiTransfer(0xFF);

    return r1;
}

uint8_t sdInit()
{
    uint8_t res[5], cmdAttempts = 0;

    sdPowerUpSeq();

    /*
      CMD0을 보내서 SDCard를 IDLE로 만듭니다
      최대 10번 Retry 합니다
    */
    while ((res[0] = sdGoIdleState()) != 0x01)
    {
        cmdAttempts++;
        if (cmdAttempts > 10)
            return SD_ERROR;
    }

    // CMD8을 보냅니다
    sdSendIfCond(res);
    if (res[0] != 0x01)
    {
        return SD_ERROR;
    }

    // Echo Pattern을 확인합니다
    if (res[4] != 0xAA)
    {
        return SD_ERROR;
    }

    // ACMD41을 통해 SDCard 초기화를 시도합니다
    cmdAttempts = 0;
    do
    {
        if (cmdAttempts > 100)
            return SD_ERROR;

        // ACMD41을 보내기전에 CMD55를 보냅니다
        res[0] = sdSendApp();

        // Response에 Error가 없다면 ACMD41을 보냅니다
        if (res[0] < 2)
        {
            res[0] = sdSendOpCond();
        }

        // 10ms동안 기다립니다
        _delay_ms(10);

        cmdAttempts++;
    } while (res[0] != SD_READY);

    // OCR을 읽습니다
    sdReadOcr(res);

    // SDCard가 Ready 상태인지 확인합니다
    if (!(res[1] & 0x80))
        return SD_ERROR;

    return SD_SUCCESS;
}
