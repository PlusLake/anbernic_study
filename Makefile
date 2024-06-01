obj := crt0.o main.o
bin := hifumi-$(shell date +%Y%m%d-%H%M%S).gba

CC := arm-none-eabi-gcc
AS := arm-none-eabi-as
OBJCOPY := arm-none-eabi-objcopy

options := -O3 -fomit-frame-pointer -marm -mcpu=arm7tdmi

CFLAGS := $(options) -std=c11 -pedantic -Wall

.PHONY: clean images

$(bin): images
	$(CC) -o out.elf crt0.s main.c -Tlinkscript -nostartfiles -lm
	$(OBJCOPY) -O binary out.elf $(bin)
	$(RM) out.elf
	retroarch -L /usr/lib/x86_64-linux-gnu/libretro/mgba_libretro.so $(bin)

images:
	printf "" > images.c
	javac ConvertImage.java
	java ConvertImage HIFUMI_0 < images/hifumi_0.png >> images.c
	java ConvertImage HIFUMI_1 < images/hifumi_1.png >> images.c
	java ConvertImage HIFUMI_2 < images/hifumi_2.png >> images.c
	java ConvertImage HIFUMI_3 < images/hifumi_3.png >> images.c

clean:
	rm -f $(obj) $(obj:.o=.d)

copy:
	rm /media/plus/6436-3334/Roms/GBA/hifumi*.gba | true
	cp *.gba /media/plus/6436-3334/Roms/GBA/
	ls -al /media/plus/6436-3334/Roms/GBA/
