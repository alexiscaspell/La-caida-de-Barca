################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/TestAlgoritmosMemoria.c \
../src/funcionesTestMemoria.c 

OBJS += \
./src/TestAlgoritmosMemoria.o \
./src/funcionesTestMemoria.o 

C_DEPS += \
./src/TestAlgoritmosMemoria.d \
./src/funcionesTestMemoria.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/git/tp-2015-2c-los-javimancos/commonsDeAsedio" -I"/home/utnso/workspace/commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


