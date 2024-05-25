obj := crt0.o main.o
bin := hello-$(shell date +%Y%m%d-%H%M%S).gba

CC := arm-none-eabi-gcc
AS := arm-none-eabi-as
OBJCOPY := arm-none-eabi-objcopy

options := -O3 -fomit-frame-pointer -marm -mcpu=arm7tdmi

CFLAGS := $(options) -std=c11 -pedantic -Wall

$(bin): $(obj)
	$(CC) -o out.elf $(obj) -Tlnkscript -nostartfiles -lm
	$(OBJCOPY) -O binary out.elf $(bin)
	$(RM) out.elf
	retroarch -L /usr/lib/x86_64-linux-gnu/libretro/mgba_libretro.so $(bin)

.PHONY: clean
clean:
	rm -f $(obj) $(obj:.o=.d)

copy:
	rm /media/plus/6436-3334/Roms/GBA/*.gba
	cp *.gba /media/plus/6436-3334/Roms/GBA/
	ls -al /media/plus/6436-3334/Roms/GBA/
