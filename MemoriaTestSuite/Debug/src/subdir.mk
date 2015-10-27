################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MemoriaTestSuite.c \
../src/Tests.c \
../src/funcionesMemoria.c 

OBJS += \
./src/MemoriaTestSuite.o \
./src/Tests.o \
./src/funcionesMemoria.o 

C_DEPS += \
./src/MemoriaTestSuite.d \
./src/Tests.d \
./src/funcionesMemoria.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/alexis/git/tp-2015-2c-los-javimancos/commonsDeAsedio" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


