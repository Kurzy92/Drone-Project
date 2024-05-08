################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sensors_Drivers/BOSCH_BMP3/BMP3_SensorAPI/examples/fifo_watermark_temp/fifo_watermark_temp.c 

OBJS += \
./Sensors_Drivers/BOSCH_BMP3/BMP3_SensorAPI/examples/fifo_watermark_temp/fifo_watermark_temp.o 

C_DEPS += \
./Sensors_Drivers/BOSCH_BMP3/BMP3_SensorAPI/examples/fifo_watermark_temp/fifo_watermark_temp.d 


# Each subdirectory must supply rules for building sources it contributes
Sensors_Drivers/BOSCH_BMP3/BMP3_SensorAPI/examples/fifo_watermark_temp/%.o Sensors_Drivers/BOSCH_BMP3/BMP3_SensorAPI/examples/fifo_watermark_temp/%.su Sensors_Drivers/BOSCH_BMP3/BMP3_SensorAPI/examples/fifo_watermark_temp/%.cyclo: ../Sensors_Drivers/BOSCH_BMP3/BMP3_SensorAPI/examples/fifo_watermark_temp/%.c Sensors_Drivers/BOSCH_BMP3/BMP3_SensorAPI/examples/fifo_watermark_temp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Embedded/Drone_Project/Drone_Project/Sensors_Drivers/BMP3XX/Inc" -I"C:/Embedded/Drone_Project/Drone_Project/Sensors_Drivers/LIS3MDL/Inc" -I"C:/Embedded/Drone_Project/Drone_Project/Sensors_Drivers/LSM6DSOX/Inc" -I"C:/Embedded/Drone_Project/Drone_Project/Sensors_Drivers/ESC/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Sensors_Drivers-2f-BOSCH_BMP3-2f-BMP3_SensorAPI-2f-examples-2f-fifo_watermark_temp

clean-Sensors_Drivers-2f-BOSCH_BMP3-2f-BMP3_SensorAPI-2f-examples-2f-fifo_watermark_temp:
	-$(RM) ./Sensors_Drivers/BOSCH_BMP3/BMP3_SensorAPI/examples/fifo_watermark_temp/fifo_watermark_temp.cyclo ./Sensors_Drivers/BOSCH_BMP3/BMP3_SensorAPI/examples/fifo_watermark_temp/fifo_watermark_temp.d ./Sensors_Drivers/BOSCH_BMP3/BMP3_SensorAPI/examples/fifo_watermark_temp/fifo_watermark_temp.o ./Sensors_Drivers/BOSCH_BMP3/BMP3_SensorAPI/examples/fifo_watermark_temp/fifo_watermark_temp.su

.PHONY: clean-Sensors_Drivers-2f-BOSCH_BMP3-2f-BMP3_SensorAPI-2f-examples-2f-fifo_watermark_temp

