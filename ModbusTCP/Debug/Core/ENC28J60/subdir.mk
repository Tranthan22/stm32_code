################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/ENC28J60/arp.c \
../Core/ENC28J60/enc28j60.c \
../Core/ENC28J60/icmp.c \
../Core/ENC28J60/interrupt.c \
../Core/ENC28J60/net.c \
../Core/ENC28J60/tcp.c \
../Core/ENC28J60/uart.c 

OBJS += \
./Core/ENC28J60/arp.o \
./Core/ENC28J60/enc28j60.o \
./Core/ENC28J60/icmp.o \
./Core/ENC28J60/interrupt.o \
./Core/ENC28J60/net.o \
./Core/ENC28J60/tcp.o \
./Core/ENC28J60/uart.o 

C_DEPS += \
./Core/ENC28J60/arp.d \
./Core/ENC28J60/enc28j60.d \
./Core/ENC28J60/icmp.d \
./Core/ENC28J60/interrupt.d \
./Core/ENC28J60/net.d \
./Core/ENC28J60/tcp.d \
./Core/ENC28J60/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/ENC28J60/%.o Core/ENC28J60/%.su Core/ENC28J60/%.cyclo: ../Core/ENC28J60/%.c Core/ENC28J60/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Core/ENC28J60 -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-ENC28J60

clean-Core-2f-ENC28J60:
	-$(RM) ./Core/ENC28J60/arp.cyclo ./Core/ENC28J60/arp.d ./Core/ENC28J60/arp.o ./Core/ENC28J60/arp.su ./Core/ENC28J60/enc28j60.cyclo ./Core/ENC28J60/enc28j60.d ./Core/ENC28J60/enc28j60.o ./Core/ENC28J60/enc28j60.su ./Core/ENC28J60/icmp.cyclo ./Core/ENC28J60/icmp.d ./Core/ENC28J60/icmp.o ./Core/ENC28J60/icmp.su ./Core/ENC28J60/interrupt.cyclo ./Core/ENC28J60/interrupt.d ./Core/ENC28J60/interrupt.o ./Core/ENC28J60/interrupt.su ./Core/ENC28J60/net.cyclo ./Core/ENC28J60/net.d ./Core/ENC28J60/net.o ./Core/ENC28J60/net.su ./Core/ENC28J60/tcp.cyclo ./Core/ENC28J60/tcp.d ./Core/ENC28J60/tcp.o ./Core/ENC28J60/tcp.su ./Core/ENC28J60/uart.cyclo ./Core/ENC28J60/uart.d ./Core/ENC28J60/uart.o ./Core/ENC28J60/uart.su

.PHONY: clean-Core-2f-ENC28J60

