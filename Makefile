CC65=	/usr/local/bin/cc65
CL65=	/usr/local/bin/cl65
COPTS=	-t c64 -O -Or -Oi -Os --cpu 6502
LOPTS=	-C gidemo.cfg

ASSFILES = main.s \
#						music.s \
#						music_game.s \
#						music_intro.s \
#						music_outro.s \
#						music_credits.s \
#						petscii.s \
#						sprites.s
#						charset.s

# DATAFILES=  ascii8x8.bin dad.bin mum.bin tram.bin
DATAFILES= ryu_idle2.bin ryu_idle3.bin ryu_idle4.bin \
	ryu_walk1.bin ryu_walk2.bin ryu_walk3.bin ryu_walk4.bin ryu_walk5.bin ryu_walk6.bin ryu_walk7.bin ryu_walk8.bin \
	ryu_walkb1.bin ryu_walkb2.bin ryu_walkb3.bin ryu_walkb4.bin ryu_walkb5.bin ryu_walkb6.bin ryu_walkb7.bin ryu_walkb8.bin \
	ryu_jump1.bin ryu_jump2.bin ryu_jump3.bin ryu_jump4.bin \
	ryu_fjump1.bin ryu_fjump2.bin ryu_fjump3.bin ryu_fjump4.bin ryu_fjump5.bin ryu_fjump6.bin ryu_fjump7.bin \
	ryu_crouch1.bin ryu_crouch2.bin ryu_block1.bin ryu_block2.bin \
	ryu_lpunch1.bin ryu_lpunch2.bin ryu_lpunch3.bin \
	ryu_mhpunch1.bin ryu_mhpunch2.bin ryu_mhpunch3.bin \
	ryu_flpunch1.bin ryu_flpunch2.bin ryu_flpunch3.bin \
	ryu_fmpunch1.bin ryu_fmpunch2.bin ryu_fmpunch3.bin ryu_fmpunch4.bin \
	ryu_fhpunch1.bin ryu_fhpunch2.bin ryu_fhpunch3.bin ryu_fhpunch4.bin


all: gidemo.d64 data.reu

run:
	#open -a /Applications/Vice/x64.app/Contents/MacOS/x64 gidemo.d64
	#/Applications/Vice/x64.app/Contents/MacOS/x64 gidemo.d64 &
	cmd /c "start c:/Users/gurcei/Downloads/GTK3VICE-3.3-win32-r35872/x64.exe --reuimage data.reu gidemo.d64"

%.s:	%.c $(DATAFILES) gidemo.cfg
	$(CC65) $(COPTS) --add-source -o $@ $<

data.reu: $(DATAFILES)
	rm -f data.reu
	cat $(DATAFILES) > data.reu
	dd if=/dev/zero of=data.reu bs=1 count=1 seek=16777215

# 'pngprepare' and 'asciih' tools borrowed from mega65-ide project
# --------------------------------------------------
# ascii8x8.bin: ascii00-7f.png pngprepare
# 	./pngprepare charrom ascii00-7f.png ascii8x8.bin

%.bin: %.png pngprepare
	./pngprepare gihires $< $@

pngprepare: pngprepare.c
	$(CC) -I/usr/local/include -L/usr/local/lib -g -O0 -o pngprepare pngprepare.c -lpng
# --------------------------------------------------

gidemo.prg: $(ASSFILES) gidemo.cfg
	$(CL65) $(COPTS) $(LOPTS) -vm -l gidemo.list -m gidemo.map -Ln gidemo.lbl -o gidemo.prg $(ASSFILES)

gidemo.d64: gidemo.prg
	rm -f gidemo.d64
	cbmconvert -v2 -D4o gidemo.d64 gidemo.prg

clean:
	rm -f gidemo.map
	rm -f gidemo.prg
	rm -f main.s main.o
	rm -f gidemo.list
	rm -f gidemo.d64
	rm -f asciih ascii.h ascii8x8.bin
	rm -f dad.bin mum.bin tram.bin
	rm -f data.reu

