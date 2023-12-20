################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/LoRa/LoRa.c \
../Core/LoRa/Loraf.c 

OBJS += \
./Core/LoRa/LoRa.o \
./Core/LoRa/Loraf.o 

C_DEPS += \
./Core/LoRa/LoRa.d \
./Core/LoRa/Loraf.d 


# Each subdirectory must supply rules for building sources it contributes
Core/LoRa/%.o Core/LoRa/%.su Core/LoRa/%.cyclo: ../Core/LoRa/%.c Core/LoRa/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Core/LoRa -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-LoRa

clean-Core-2f-LoRa:
	-$(RM) ./Core/LoRa/LoRa.cyclo ./Core/LoRa/LoRa.d ./Core/LoRa/LoRa.o ./Core/LoRa/LoRa.su ./Core/LoRa/Loraf.cyclo ./Core/LoRa/Loraf.d ./Core/LoRa/Loraf.o ./Core/LoRa/Loraf.su

.PHONY: clean-Core-2f-LoRa

