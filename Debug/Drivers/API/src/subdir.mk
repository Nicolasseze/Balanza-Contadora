################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/src/API_debounce.c \
../Drivers/API/src/API_delay.c \
../Drivers/API/src/hx711_driver.c \
../Drivers/API/src/hx711_port.c \
../Drivers/API/src/lcd_driver.c \
../Drivers/API/src/lcd_port.c 

OBJS += \
./Drivers/API/src/API_debounce.o \
./Drivers/API/src/API_delay.o \
./Drivers/API/src/hx711_driver.o \
./Drivers/API/src/hx711_port.o \
./Drivers/API/src/lcd_driver.o \
./Drivers/API/src/lcd_port.o 

C_DEPS += \
./Drivers/API/src/API_debounce.d \
./Drivers/API/src/API_delay.d \
./Drivers/API/src/hx711_driver.d \
./Drivers/API/src/hx711_port.d \
./Drivers/API/src/lcd_driver.d \
./Drivers/API/src/lcd_port.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/src/%.o Drivers/API/src/%.su Drivers/API/src/%.cyclo: ../Drivers/API/src/%.c Drivers/API/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/nicolas-porco/STM32CubeIDE/BalanzaContadora/BalanzaContadora/Drivers/API/src" -I"/home/nicolas-porco/STM32CubeIDE/BalanzaContadora/BalanzaContadora/Drivers/API/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-src

clean-Drivers-2f-API-2f-src:
	-$(RM) ./Drivers/API/src/API_debounce.cyclo ./Drivers/API/src/API_debounce.d ./Drivers/API/src/API_debounce.o ./Drivers/API/src/API_debounce.su ./Drivers/API/src/API_delay.cyclo ./Drivers/API/src/API_delay.d ./Drivers/API/src/API_delay.o ./Drivers/API/src/API_delay.su ./Drivers/API/src/hx711_driver.cyclo ./Drivers/API/src/hx711_driver.d ./Drivers/API/src/hx711_driver.o ./Drivers/API/src/hx711_driver.su ./Drivers/API/src/hx711_port.cyclo ./Drivers/API/src/hx711_port.d ./Drivers/API/src/hx711_port.o ./Drivers/API/src/hx711_port.su ./Drivers/API/src/lcd_driver.cyclo ./Drivers/API/src/lcd_driver.d ./Drivers/API/src/lcd_driver.o ./Drivers/API/src/lcd_driver.su ./Drivers/API/src/lcd_port.cyclo ./Drivers/API/src/lcd_port.d ./Drivers/API/src/lcd_port.o ./Drivers/API/src/lcd_port.su

.PHONY: clean-Drivers-2f-API-2f-src

