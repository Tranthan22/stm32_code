#ifndef HTTP_H_
#define HTTP_H_
//--------------------------------------------------
//Include cac thu vien can thiet
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"

//--------------------------------------------------
void HTTP_pack_init(void);
uint8_t HTTP_get_status(void);
void HTTP_set_status(uint8_t s);
uint8_t HTTP_get_data_num(uint8_t** data,uint16_t* data_length);
uint32_t HTTP_get_Sequence_Number(void);
void HTTP_set_Sequence_Number(uint32_t SequenceN);
//--------------------------------------------------
#endif /* HTTP_H_ */

