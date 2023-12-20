#include "LoRa.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

char* concatenateStrings(const char* str1, const char* str2);
void transmit_test(LoRa* _LoRa);
void receive_test(LoRa* _LoRa, UART_HandleTypeDef *huart, uint8_t* receive_data);
void LoRa_start(LoRa* _LoRa, UART_HandleTypeDef *huart);
