################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/EXTI_Callback_user.c \
../Core/Src/Led_user.c \
../Core/Src/PID_Regler_user.c \
../Core/Src/Spulenschaltung_user.c \
../Core/Src/bno055.c \
../Core/Src/main.c \
../Core/Src/main_user.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/usb_prinf.c 

C_DEPS += \
./Core/Src/EXTI_Callback_user.d \
./Core/Src/Led_user.d \
./Core/Src/PID_Regler_user.d \
./Core/Src/Spulenschaltung_user.d \
./Core/Src/bno055.d \
./Core/Src/main.d \
./Core/Src/main_user.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/usb_prinf.d 

OBJS += \
./Core/Src/EXTI_Callback_user.o \
./Core/Src/Led_user.o \
./Core/Src/PID_Regler_user.o \
./Core/Src/Spulenschaltung_user.o \
./Core/Src/bno055.o \
./Core/Src/main.o \
./Core/Src/main_user.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/usb_prinf.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/EXTI_Callback_user.cyclo ./Core/Src/EXTI_Callback_user.d ./Core/Src/EXTI_Callback_user.o ./Core/Src/EXTI_Callback_user.su ./Core/Src/Led_user.cyclo ./Core/Src/Led_user.d ./Core/Src/Led_user.o ./Core/Src/Led_user.su ./Core/Src/PID_Regler_user.cyclo ./Core/Src/PID_Regler_user.d ./Core/Src/PID_Regler_user.o ./Core/Src/PID_Regler_user.su ./Core/Src/Spulenschaltung_user.cyclo ./Core/Src/Spulenschaltung_user.d ./Core/Src/Spulenschaltung_user.o ./Core/Src/Spulenschaltung_user.su ./Core/Src/bno055.cyclo ./Core/Src/bno055.d ./Core/Src/bno055.o ./Core/Src/bno055.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/main_user.cyclo ./Core/Src/main_user.d ./Core/Src/main_user.o ./Core/Src/main_user.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su ./Core/Src/usb_prinf.cyclo ./Core/Src/usb_prinf.d ./Core/Src/usb_prinf.o ./Core/Src/usb_prinf.su

.PHONY: clean-Core-2f-Src

