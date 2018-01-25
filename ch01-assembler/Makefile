ARMGNU ?= arm-none-eabi

all: blink.bin

blink.bin: blink.s flash.ld
	$(ARMGNU)-as -mcpu=cortex-m3 -mthumb blink.s -o blink.o
	$(ARMGNU)-ld -o blink.elf blink.o -T flash.ld -M=blink.map
	$(ARMGNU)-objdump -D blink.elf > blink.list
	$(ARMGNU)-objcopy blink.elf blink.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.map

flash:
	JLinkExe -device LPC1768 -If SWD -CommanderScript flashlpc1768.jlink

debug:
	JLinkGDBServer -device LPC1768 -If SWD -CommanderScript flashlpc1768.jlink
