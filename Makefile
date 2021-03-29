CC = gcc
CFLAGS =
LFLAGS =
LIBPATH = -L/usr/X11/lib
INCPATH =
LIBS = -lX11 -lm
INCS =
DEFS = -DX -DUNIX -O3 -Wall
OBJS = main.o \
	cdp1802.o \
	clock.o \
	comx35.o \
	debug.o \
	elf2.o \
	fdc.o \
	hbelf.o \
	hex.o \
	ide.o \
	keyboard.o \
	keypad.o \
	led.o \
	pixie.o \
	portext.o \
	printer.o \
	ps2kbd.o \
	push.o \
	redled.o \
	studio2.o \
	super.o \
	til311.o \
	tms9918.o \
	toggle.o \
	vt100.o \
	v1870.o \
	v6845.o \
	awlib.o

elf: $(OBJS)
	$(CC) $(LFLAGS) $(LIBPATH) $(OBJS) $(LIBS) -o elf

.c.o:
	$(CC) $(CFLAGS) $(INCPATH) $(INCS) $(DEFS) -c $<

install: elf
	cp elf ..
dist:
	make clean
	-rmdir elf
	mkdir elf
	cp *.c elf
	cp *.h elf
	mkdir elf/images
	cp images/*.xpm elf/images
	cp elf.txt elf
	cp Makefile elf
	tar cvfz elf.tgz ./elf
	rm -r ./elf

clean:
	-rm *.o
	-rm elf

main.o: main.c elf.h
cdp1802.o: cdp1802.c elf.h
clock.o: clock.c elf.h
comx35.o: comx35.c elf.h
debug.o: debug.c elf.h
elf2.o: elf2.c elf.h
fdc.o: fdc.c elf.h
hbelf.o: hbelf.c elf.h
hex.o: hex.c elf.h
ide.o: ide.c elf.h
keyboard.o: keyboard.c elf.h
keypad.o: keypad.c elf.h
led.o: led.c elf.h
pixie.o: pixie.c elf.h
portext.o: portext.c elf.h
ps2kbd.o:  ps2kbd.c elf.h
printer.o: printer.c elf.h
push.o: push.c elf.h
super.o: super.c elf.h
studio2.o: studio2.c elf.h
til311.o: til311.c
redled.o: redled.c elf.h
tms9918.o: tms9918.c elf.h
toggle.o: toggle.c
vt100.o: vt100.c
v6845.o: v6845.c elf.h
v1870.o: v1870.c elf.h
