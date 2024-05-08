################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sensors_Drivers/ESC/Src/esc.c 

OBJS += \
./Sensors_Drivers/ESC/Src/esc.o 

C_DEPS += \
./Sensors_Drivers/ESC/Src/esc.d 


# Each subdirectory must supply rules for building sources it contributes
Sensors_Drivers/ESC/Src/%.o Sensors_Drivers/ESC/Src/%.su Sensors_Drivers/ESC/Src/%.cyclo: ../Sensors_Drivers/ESC/Src/%.c Sensors_Drivers/ESC/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -DARM_MATH_CM7 -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Embedded/Drone_Project/Drone_Project/Sensors_Drivers/BMP3XX/Inc" -I"C:/Embedded/Drone_Project/Drone_Project/Sensors_Drivers/LIS3MDL/Inc" -I"C:/Embedded/Drone_Project/Drone_Project/Sensors_Drivers/LSM6DSOX/Inc" -I"C:/Embedded/Drone_Project/Drone_Project/Sensors_Drivers/ESC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Sensors_Drivers-2f-ESC-2f-Src

clean-Sensors_Drivers-2f-ESC-2f-Src:
	-$(RM) ./Sensors_Drivers/ESC/Src/esc.cyclo ./Sensors_Drivers/ESC/Src/esc.d ./Sensors_Drivers/ESC/Src/esc.o ./Sensors_Drivers/ESC/Src/esc.su

.PHONY: clean-Sensors_Drivers-2f-ESC-2f-Src

