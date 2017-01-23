################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/epsilometer_CMU_init.c \
../src/epsilometer_GPIO_init.c \
../src/epsilometer_TIMER_init.c \
../src/epsilometer_analog.c \
../src/epsilometer_coms.c \
../src/epsilometer_main.c 

OBJS += \
./src/epsilometer_CMU_init.o \
./src/epsilometer_GPIO_init.o \
./src/epsilometer_TIMER_init.o \
./src/epsilometer_analog.o \
./src/epsilometer_coms.o \
./src/epsilometer_main.o 

C_DEPS += \
./src/epsilometer_CMU_init.d \
./src/epsilometer_GPIO_init.d \
./src/epsilometer_TIMER_init.d \
./src/epsilometer_analog.d \
./src/epsilometer_coms.d \
./src/epsilometer_main.d 


# Each subdirectory must supply rules for building sources it contributes
src/epsilometer_CMU_init.o: ../src/epsilometer_CMU_init.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DDEBUG=1' '-DEFM32WG842F256=1' -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/emlib" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/ff12b/src/option" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/Calendar" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/ff12b/src" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32WG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/epsilometer_CMU_init.d" -MT"src/epsilometer_CMU_init.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/epsilometer_GPIO_init.o: ../src/epsilometer_GPIO_init.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DDEBUG=1' '-DEFM32WG842F256=1' -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/emlib" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/ff12b/src/option" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/Calendar" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/ff12b/src" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32WG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/epsilometer_GPIO_init.d" -MT"src/epsilometer_GPIO_init.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/epsilometer_TIMER_init.o: ../src/epsilometer_TIMER_init.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DDEBUG=1' '-DEFM32WG842F256=1' -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/emlib" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/ff12b/src/option" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/Calendar" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/ff12b/src" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32WG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/epsilometer_TIMER_init.d" -MT"src/epsilometer_TIMER_init.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/epsilometer_analog.o: ../src/epsilometer_analog.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DDEBUG=1' '-DEFM32WG842F256=1' -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/emlib" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/ff12b/src/option" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/Calendar" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/ff12b/src" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32WG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/epsilometer_analog.d" -MT"src/epsilometer_analog.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/epsilometer_coms.o: ../src/epsilometer_coms.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DDEBUG=1' '-DEFM32WG842F256=1' -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/emlib" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/ff12b/src/option" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/Calendar" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/ff12b/src" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32WG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/epsilometer_coms.d" -MT"src/epsilometer_coms.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/epsilometer_main.o: ../src/epsilometer_main.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DDEBUG=1' '-DEFM32WG842F256=1' -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/emlib" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/ff12b/src/option" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/Calendar" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/ff12b/src" -I"/Users/aleboyer/SimplicityStudio/v4_workspace/Epsilometer_v1/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32WG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/epsilometer_main.d" -MT"src/epsilometer_main.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


