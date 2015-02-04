#executables:
CC=avr-g++
OBJCOPY=avr-objcopy
NM = avr-nm
OBJDUMP = avr-objdump
SIZE = avr-size
AVRDUDE = avrdude

#avrdude args
PROGRAMMER = usbasp
AVRDUDE_ARGS = 

#directories and files
OBJDIR = bin
SRCDIR = src
CPPSRC = Authenticator.cpp Buffer.cpp HashBase.cpp Interface.cpp Login.cpp LowLevelDevice.cpp Person.cpp Timer.cpp interrupt_handlers.cpp main.cpp
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) $(CPPSRC:%.cpp=$(OBJDIR)/%.o) $(ASRC:%.S=$(OBJDIR)/%.o) 
TARGET = main
VPATH = bin src inc .

# CPP FLAGS
INCLUDE = inc
OPTIMISATION = s
MCU = atmega8
FREQUENCY = 16000000L
ALLCPPFLAGS = -I $(INCLUDE) -mmcu=$(MCU) -DF_CPU=$(FREQUENCY) -O$(OPTIMISATION) -U__AVR2560__



all: begin $(TARGET).elf $(TARGET).sym $(TARGET).lss $(TARGET).hex size end

begin:
	@echo
	@echo "--- COMPILING" $(TARGET) "---"

end:
	@echo
	@echo "--- COMPLETED" $(TARGET) "---"
	
size: 
	$(SIZE) --mcu=$(MCU) --format=avr $(TARGET).elf
	
%.hex: %.elf
	@echo
	@echo "--- CREATING FLASH FILE" $@ "---"
	$(OBJCOPY) -Oihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

	
%.lss: %.elf
	@echo
	@echo "--- CREATING LISTING" $@ "---"
	$(OBJDUMP) -h -S -z $< > $@


%.sym: %.elf
	@echo
	@echo " --- CREATING SYMBOL TABLE" $@ "--- "
	$(NM) -n $< > $@


.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo "---- LINKING" $@ "---- "
	$(CC) $(ALLCPPFLAGS) $^ -o $@
	
	
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $< $(ALLCPPFLAGS) -c -o $@

	
clean:
	rm -f $(TARGET).elf $(TARGET).sym $(TARGET).lss $(TARGET).hex $(TARGET).eep $(OBJDIR)/* 
	
$(shell mkdir $(OBJDIR) 2>/dev/null)

install: $(TARGET).hex
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -U flash:w:$(TARGET).hex $(AVRDUDE_ARGS)

	
install328: $(TARGET).hex
	$(AVRDUDE) -p $(MCU) -c arduino -U flash:w:$(TARGET).hex $(AVRDUDE_ARGS) -P COM11
