################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/src/FatFs/option/syscall.c \
../example/src/FatFs/option/unicode.c 

OBJS += \
./example/src/FatFs/option/syscall.o \
./example/src/FatFs/option/unicode.o 

C_DEPS += \
./example/src/FatFs/option/syscall.d \
./example/src/FatFs/option/unicode.d 


# Each subdirectory must supply rules for building sources it contributes
example/src/FatFs/option/%.o: ../example/src/FatFs/option/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\lpc_chip_175x_6x\inc\usbd" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\src\FatFs" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\src\Perifericos" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\src\TFT" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\src\Fonts" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\lpc_chip_175x_6x\inc" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\inc" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\freertos\inc" -O0 -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


