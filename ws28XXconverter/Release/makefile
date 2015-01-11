################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
ws28XXconverter.lss \

FLASH_IMAGE += \
ws28XXconverter.hex \

EEPROM_IMAGE += \
ws28XXconverter.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: ws28XXconverter.elf secondary-outputs

# Tool invocations
ws28XXconverter.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,ws28XXconverter.map -mmcu=attiny85 -o "ws28XXconverter.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ws28XXconverter.lss: ws28XXconverter.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S ws28XXconverter.elf  >"ws28XXconverter.lss"
	@echo 'Finished building: $@'
	@echo ' '

ws28XXconverter.hex: ws28XXconverter.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex ws28XXconverter.elf  "ws28XXconverter.hex"
	@echo 'Finished building: $@'
	@echo ' '

ws28XXconverter.eep: ws28XXconverter.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex ws28XXconverter.elf  "ws28XXconverter.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: ws28XXconverter.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=attiny85 ws28XXconverter.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) ws28XXconverter.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
