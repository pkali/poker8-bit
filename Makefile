CFLAGS=--target atari --add-source -Oi
AFLAGS=--target atari
LDFLAGS=--target atari -v
OBJS=target/eval.o target/poker.o

all: target poker.xex
#	ls -o poker.xex

target:
	mkdir target

poker.xex: target/poker.bin
#	pucrunch -fshort -c64 -m5 target/potion.bin potion.prg
	cp target/poker.bin poker.xex

target/poker.bin: $(OBJS)
	ld65 -o target/poker.bin $(LDFLAGS) -Ln target/poker.labels $(OBJS) atari.lib

target/%.o: target/%.s
	ca65 $(AFLAGS) -o $@ $<

target/%.s: %.c
	cc65 $(CFLAGS) -o $@ $<

.PHONY: all