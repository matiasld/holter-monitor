################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/src/cr_startup_lpc175x_6x.c \
../example/src/freertos_blinky.c \
../example/src/hid_desc.c \
../example/src/hid_generic.c \
../example/src/sysinit.c 

OBJS += \
./example/src/cr_startup_lpc175x_6x.o \
./example/src/freertos_blinky.o \
./example/src/hid_desc.o \
./example/src/hid_generic.o \
./example/src/sysinit.o 

C_DEPS += \
./example/src/cr_startup_lpc175x_6x.d \
./example/src/freertos_blinky.d \
./example/src/hid_desc.d \
./example/src/hid_generic.d \
./example/src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
example/src/%.o: ../example/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\lpc_chip_175x_6x\inc\usbd" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\src\FatFs" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\src\Perifericos" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\src\TFT" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\src\Fonts" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\lpc_chip_175x_6x\inc" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\example\inc" -I"C:\Users\Matata\Dropbox\Tecnicas Digitales 2\Proyecto\Codigo\Micro\Proyecto\freertos_blinky\freertos\inc" -O0 -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


