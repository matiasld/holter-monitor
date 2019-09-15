################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/src/FatFs/SD_AJS.c \
../example/src/FatFs/diskio.c \
../example/src/FatFs/ff.c 

OBJS += \
./example/src/FatFs/SD_AJS.o \
./example/src/FatFs/diskio.o \
./example/src/FatFs/ff.o 

C_DEPS += \
./example/src/FatFs/SD_AJS.d \
./example/src/FatFs/diskio.d \
./example/src/FatFs/ff.d 


# Each subdirectory must supply rules for building sources it contributes
example/src/FatFs/%.o: ../example/src/FatFs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\lpc_chip_175x_6x\inc\usbd" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\src\FatFs" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\src\Perifericos" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\src\TFT" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\src\Fonts" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\lpc_chip_175x_6x\inc" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\inc" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\freertos\inc" -O0 -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


