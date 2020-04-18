################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bin/Blocked.cpp \
../src/bin/Exit.cpp \
../src/bin/New.cpp \
../src/bin/Ready.cpp \
../src/bin/Running.cpp \
../src/bin/kill.cpp \
../src/bin/system.cpp \
../src/bin/terminal.cpp 

OBJS += \
./src/bin/Blocked.o \
./src/bin/Exit.o \
./src/bin/New.o \
./src/bin/Ready.o \
./src/bin/Running.o \
./src/bin/kill.o \
./src/bin/system.o \
./src/bin/terminal.o 

CPP_DEPS += \
./src/bin/Blocked.d \
./src/bin/Exit.d \
./src/bin/New.d \
./src/bin/Ready.d \
./src/bin/Running.d \
./src/bin/kill.d \
./src/bin/system.d \
./src/bin/terminal.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/%.o: ../src/bin/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


