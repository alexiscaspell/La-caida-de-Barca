################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: compactacion

dependents:
	-cd /home/utnso/workspace/commonsDeAsedio/Debug && $(MAKE) all

# Tool invocations
compactacion: $(OBJS) $(USER_OBJS) /home/utnso/workspace/commonsDeAsedio/Debug/libcommonsDeAsedio.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/workspace/commonsDeAsedio/Debug" -o "compactacion" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) compactacion
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/workspace/commonsDeAsedio/Debug/libcommonsDeAsedio.so:

-include ../makefile.targets
