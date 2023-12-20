#include "LoRa_f.h"
#include "main.h"

char* concatenateStrings(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = (char*)malloc(len1 + len2 + 1);

    if (result == NULL) {
        fprintf(stderr, "Khong the cap phat bo nhos.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(result, str1);
    strcat(result, str2);

    return result;
}
void transmit_test(LoRa* _LoRa){
  char* send_data = concatenateStrings("", "du lieu truyen di la");
  sprintf(send_data + strlen(send_data), "%d" , 1234);
  //snprintf(send_data,sizeof(send_data),"du lieu truyen di la 1234");
  LoRa_transmit(_LoRa, (uint8_t*)send_data, strlen(send_data), 100);
  free(send_data);
}
void receive_test(LoRa* _LoRa, UART_HandleTypeDef *huart, uint8_t* receive_data){
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	LoRa_receive(_LoRa, receive_data, 10);


}

void LoRa_start(LoRa* _LoRa, UART_HandleTypeDef *huart){
	if (LoRa_init(_LoRa)==LORA_OK){
		//char send_data[] = "hello";
		  LoRa_transmit(_LoRa, (uint8_t*)" LoRa_STM ok", sizeof("hello LoRa_STM ok"), 100);
		  HAL_UART_Transmit(huart, (uint8_t*)" LoRa_STM ok", sizeof("hello LoRa_STM ok"), 100);
		  LoRa_startReceiving(_LoRa);
	}
	else{
		  HAL_UART_Transmit(huart, (uint8_t*)" LoRa_STM fail", sizeof("hello LoRa_STM fail"), 200);
	}
}

