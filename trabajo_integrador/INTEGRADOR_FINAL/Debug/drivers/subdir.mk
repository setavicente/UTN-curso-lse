################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_adc.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_ctimer.c \
../drivers/fsl_dma.c \
../drivers/fsl_gpio.c \
../drivers/fsl_i2c.c \
../drivers/fsl_i2c_dma.c \
../drivers/fsl_inputmux.c \
../drivers/fsl_power.c \
../drivers/fsl_reset.c \
../drivers/fsl_sctimer.c \
../drivers/fsl_swm.c \
../drivers/fsl_usart.c 

C_DEPS += \
./drivers/fsl_adc.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_ctimer.d \
./drivers/fsl_dma.d \
./drivers/fsl_gpio.d \
./drivers/fsl_i2c.d \
./drivers/fsl_i2c_dma.d \
./drivers/fsl_inputmux.d \
./drivers/fsl_power.d \
./drivers/fsl_reset.d \
./drivers/fsl_sctimer.d \
./drivers/fsl_swm.d \
./drivers/fsl_usart.d 

OBJS += \
./drivers/fsl_adc.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_ctimer.o \
./drivers/fsl_dma.o \
./drivers/fsl_gpio.o \
./drivers/fsl_i2c.o \
./drivers/fsl_i2c_dma.o \
./drivers/fsl_inputmux.o \
./drivers/fsl_power.o \
./drivers/fsl_reset.o \
./drivers/fsl_sctimer.o \
./drivers/fsl_swm.o \
./drivers/fsl_usart.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/vicenteseta/Documents/UTN-curso-lse/workspace_lpc845/INTEGRADOR_FINAL/board" -I"/Users/vicenteseta/Documents/UTN-curso-lse/workspace_lpc845/INTEGRADOR_FINAL/source" -I"/Users/vicenteseta/Documents/UTN-curso-lse/workspace_lpc845/INTEGRADOR_FINAL/component/uart" -I"/Users/vicenteseta/Documents/UTN-curso-lse/workspace_lpc845/INTEGRADOR_FINAL/drivers" -I"/Users/vicenteseta/Documents/UTN-curso-lse/workspace_lpc845/INTEGRADOR_FINAL/CMSIS" -I"/Users/vicenteseta/Documents/UTN-curso-lse/workspace_lpc845/INTEGRADOR_FINAL/CMSIS/m-profile" -I"/Users/vicenteseta/Documents/UTN-curso-lse/workspace_lpc845/INTEGRADOR_FINAL/device" -I"/Users/vicenteseta/Documents/UTN-curso-lse/workspace_lpc845/INTEGRADOR_FINAL/utilities/debug_console_lite" -I"/Users/vicenteseta/Documents/UTN-curso-lse/workspace_lpc845/INTEGRADOR_FINAL/utilities/str" -I"/Users/vicenteseta/Documents/UTN-curso-lse/workspace_lpc845/INTEGRADOR_FINAL/utilities" -I"/Users/vicenteseta/Documents/UTN-curso-lse/workspace_lpc845/INTEGRADOR_FINAL/device/periph2" -I"/Users/vicenteseta/Documents/UTN-curso-lse/workspace_lpc845/freertos/inc" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_adc.d ./drivers/fsl_adc.o ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_common_arm.d ./drivers/fsl_common_arm.o ./drivers/fsl_ctimer.d ./drivers/fsl_ctimer.o ./drivers/fsl_dma.d ./drivers/fsl_dma.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_i2c.d ./drivers/fsl_i2c.o ./drivers/fsl_i2c_dma.d ./drivers/fsl_i2c_dma.o ./drivers/fsl_inputmux.d ./drivers/fsl_inputmux.o ./drivers/fsl_power.d ./drivers/fsl_power.o ./drivers/fsl_reset.d ./drivers/fsl_reset.o ./drivers/fsl_sctimer.d ./drivers/fsl_sctimer.o ./drivers/fsl_swm.d ./drivers/fsl_swm.o ./drivers/fsl_usart.d ./drivers/fsl_usart.o

.PHONY: clean-drivers

