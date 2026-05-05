MICRO_SD_PATH=/media/plus/6436-3334/Roms/PORTS/
REMOTE_PATH=/opt/muos/bin

compile:
	aarch64-linux-gnu-gcc main.c -o main -lSDL2

deploy:
	adb push main $(REMOTE_PATH)/sos_main
	adb push ascii.bmp $(REMOTE_PATH)/ascii.bmp

launcher:
	adb push frontend.sh /opt/muos/script/mux/frontend.sh
	adb push startup.sh /opt/muos/script/system/startup.sh
	adb push func.sh /opt/muos/script/var/func.sh
	adb push sysinit /opt/muos/script/init/sysinit

reboot:
	adb shell reboot
