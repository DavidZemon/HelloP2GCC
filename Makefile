export P2GCC_LIBDIR := $(HOME)/reusable/software/p2gcc/lib
export PATH := $(HOME)/reusable/software/p2gcc/bin:/opt/parallax/bin:$(PATH)

run-%: %.bin
	@echo --- LOADING $< ---
	loadp2 -v -t -p /dev/ttyUSB0 -m 010c1f08 $<

%.bin: %.c
	@echo --- COMPILING $< ---
	bash p2gcc -v -o $@ $<
	@rm $$(echo $* | cut -d. -f1).o

%.bin: %.p2asm
	@echo --- Assembling $< ---
	p2asm -c $<
	@rm $$(echo $* | cut -d. -f1).lst

clean:
	rm -f *.bin

help:
	@ls -1a *.c | sed 's/^/run-/g' | sed 's/\.c//g'
	@ls -1a *.c | sed 's/\.c/\.bin/g'
