#define CMD0 0
#define CMD0_ARG 0x00000000
#define CMD0_CRC 0x94

#define CMD8 8
#define CMD8_ARG 0x000001AA
#define CMD8_CRC 0x86

#define CMD58 58
#define CMD58_ARG 0x00000000
#define CMD58_CRC 0x00

#define CMD55 55
#define CMD55_ARG 0x00000000
#define CMD55_CRC 0x00

#define ACMD41 41
#define ACMD41_ARG 0x40000000
#define ACMD41_CRC 0x00

void sdPowerUpSeq();
void sdCommand(uint8_t cmd, uint32_t arg, uint8_t crc);
uint8_t sdReadRes1();
uint8_t sdGoIdleState();
void sdSendIfCond(uint8_t *res);

#define PARAM_ERROR(X) X & 0b01000000
#define ADDR_ERROR(X) X & 0b00100000
#define ERASE_SEQ_ERROR(X) X & 0b00010000
#define CRC_ERROR(X) X & 0b00001000
#define ILLEGAL_CMD(X) X & 0b00000100
#define ERASE_RESET(X) X & 0b00000010
#define IN_IDLE(X) X & 0b00000001

void sdPrintR1(uint8_t res);

#define CMD_VER(X) ((X >> 4) & 0xF0)
#define VOL_ACC(X) (X & 0x1F)

#define VOLTAGE_ACC_27_33 0b00000001
#define VOLTAGE_ACC_LOW 0b00000010
#define VOLTAGE_ACC_RES1 0b00000100
#define VOLTAGE_ACC_RES2 0b00001000

void sdReadRes3Res7(uint8_t *res);

void sdReadOcr(uint8_t *res);

#define POWER_UP_STATUS(X) X & 0x40
#define CCS_VAL(X) X & 0x40
#define VDD_2728(X) X & 0b10000000
#define VDD_2829(X) X & 0b00000001
#define VDD_2930(X) X & 0b00000010
#define VDD_3031(X) X & 0b00000100
#define VDD_3132(X) X & 0b00001000
#define VDD_3233(X) X & 0b00010000
#define VDD_3334(X) X & 0b00100000
#define VDD_3435(X) X & 0b01000000
#define VDD_3536(X) X & 0b10000000

void sdPrintR3(uint8_t *res);

uint8_t sdSendApp();
uint8_t sdSendOpCond();

#define SD_READY 0
#define SD_SUCCESS 0
#define SD_ERROR 1

uint8_t sdInit();