CC=avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=atmega128
OBJ2HEX=avr-objcopy 
AVRPROG2=avrprog2
TARGET=softwarePwm

program : $(TARGET).hex
	$(AVRPROG2) -d -m atmega128 -f 10 --flash w:$(TARGET).hex
%.obj : %.o
	$(CC) $(CFLAGS) $< -o $@

%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

clean :
	rm -f *.hex *.obj *.o
