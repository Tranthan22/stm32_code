################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/ENC28J60_O/arp.c \
../Core/ENC28J60_O/dhcp.c \
../Core/ENC28J60_O/enc28j60.c \
../Core/ENC28J60_O/http.c \
../Core/ENC28J60_O/icmp.c \
../Core/ENC28J60_O/net.c \
../Core/ENC28J60_O/tcp.c \
../Core/ENC28J60_O/uart.c \
../Core/ENC28J60_O/udp.c 

OBJS += \
./Core/ENC28J60_O/arp.o \
./Core/ENC28J60_O/dhcp.o \
./Core/ENC28J60_O/enc28j60.o \
./Core/ENC28J60_O/http.o \
./Core/ENC28J60_O/icmp.o \
./Core/ENC28J60_O/net.o \
./Core/ENC28J60_O/tcp.o \
./Core/ENC28J60_O/uart.o \
./Core/ENC28J60_O/udp.o 

C_DEPS += \
./Core/ENC28J60_O/arp.d \
./Core/ENC28J60_O/dhcp.d \
./Core/ENC28J60_O/enc28j60.d \
./Core/ENC28J60_O/http.d \
./Core/ENC28J60_O/icmp.d \
./Core/ENC28J60_O/net.d \
./Core/ENC28J60_O/tcp.d \
./Core/ENC28J60_O/uart.d \
./Core/ENC28J60_O/udp.d 


# Each subdirectory must supply rules for building sources it contributes
Core/ENC28J60_O/%.o Core/ENC28J60_O/%.su Core/ENC28J60_O/%.cyclo: ../Core/ENC28J60_O/%.c Core/ENC28J60_O/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Core/ENC28J60 -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-ENC28J60_O

clean-Core-2f-ENC28J60_O:
	-$(RM) ./Core/ENC28J60_O/arp.cyclo ./Core/ENC28J60_O/arp.d ./Core/ENC28J60_O/arp.o ./Core/ENC28J60_O/arp.su ./Core/ENC28J60_O/dhcp.cyclo ./Core/ENC28J60_O/dhcp.d ./Core/ENC28J60_O/dhcp.o ./Core/ENC28J60_O/dhcp.su ./Core/ENC28J60_O/enc28j60.cyclo ./Core/ENC28J60_O/enc28j60.d ./Core/ENC28J60_O/enc28j60.o ./Core/ENC28J60_O/enc28j60.su ./Core/ENC28J60_O/http.cyclo ./Core/ENC28J60_O/http.d ./Core/ENC28J60_O/http.o ./Core/ENC28J60_O/http.su ./Core/ENC28J60_O/icmp.cyclo ./Core/ENC28J60_O/icmp.d ./Core/ENC28J60_O/icmp.o ./Core/ENC28J60_O/icmp.su ./Core/ENC28J60_O/net.cyclo ./Core/ENC28J60_O/net.d ./Core/ENC28J60_O/net.o ./Core/ENC28J60_O/net.su ./Core/ENC28J60_O/tcp.cyclo ./Core/ENC28J60_O/tcp.d ./Core/ENC28J60_O/tcp.o ./Core/ENC28J60_O/tcp.su ./Core/ENC28J60_O/uart.cyclo ./Core/ENC28J60_O/uart.d ./Core/ENC28J60_O/uart.o ./Core/ENC28J60_O/uart.su ./Core/ENC28J60_O/udp.cyclo ./Core/ENC28J60_O/udp.d ./Core/ENC28J60_O/udp.o ./Core/ENC28J60_O/udp.su

.PHONY: clean-Core-2f-ENC28J60_O

