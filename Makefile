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
	ryu_fhpunch1.bin ryu_fhpunch2.bin ryu_fhpunch3.bin ryu_fhpunch4.bin \
	ryu_lmkick1.bin ryu_lmkick2.bin \
	ryu_hkick1.bin ryu_hkick2.bin ryu_hkick3.bin ryu_hkick4.bin ryu_hkick5.bin \
	ryu_flkick1.bin ryu_flkick2.bin \
	ryu_fmkick1.bin ryu_fmkick2.bin ryu_fmkick3.bin \
	ryu_fhkick1.bin ryu_fhkick2.bin ryu_fhkick3.bin \
	ryu_crouch_lpunch1.bin ryu_crouch_lpunch2.bin \
	ryu_crouch_mpunch1.bin ryu_crouch_mpunch2.bin \
	ryu_crouch_hpunch1.bin ryu_crouch_hpunch2.bin ryu_crouch_hpunch3.bin \
	ryu_crouch_lkick1.bin ryu_crouch_lkick2.bin \
	ryu_crouch_mkick1.bin ryu_crouch_mkick2.bin \
	ryu_crouch_hkick1.bin ryu_crouch_hkick2.bin ryu_crouch_hkick3.bin ryu_crouch_hkick4.bin ryu_crouch_hkick5.bin \
	ryu_jump_lmhpunch1.bin ryu_jump_lmhpunch2.bin ryu_jump_lmhpunch3.bin \
	ryu_jump_lmkick1.bin ryu_jump_lmkick2.bin \
	ryu_jump_hkick1.bin ryu_jump_hkick2.bin ryu_jump_hkick3.bin \
	ryu_fjump_lpunch1.bin ryu_fjump_lpunch2.bin \
	ryu_fjump_mhkick1.bin ryu_fjump_mhkick2.bin ryu_fjump_mhkick3.bin \
	ryu_shouryuken1.bin ryu_shouryuken2.bin ryu_shouryuken3.bin ryu_shouryuken4.bin ryu_shouryuken5.bin ryu_shouryuken6.bin ryu_shouryuken7.bin \
	ryu_tatsumaki1.bin ryu_tatsumaki2.bin ryu_tatsumaki3.bin ryu_tatsumaki4.bin ryu_tatsumaki5.bin ryu_tatsumaki6.bin ryu_tatsumaki7.bin ryu_tatsumaki8.bin ryu_tatsumaki9.bin \
	ryu_hadouken1.bin ryu_hadouken2.bin ryu_hadouken3.bin ryu_hadouken4.bin ryu_hadouken5.bin \
	ryu_hadproj1.bin ryu_hadproj2.bin ryu_hadproj3.bin ryu_hadproj4.bin ryu_hadproj5.bin ryu_hadproj6.bin ryu_hadproj7.bin ryu_hadproj8.bin ryu_hadproj9.bin ryu_hadproj10.bin ryu_hadproj11.bin ryu_hadproj12.bin ryu_hadproj13.bin ryu_hadproj14.bin ryu_hadproj15.bin ryu_hadproj16.bin ryu_hadproj17.bin ryu_hadproj18.bin \
	ryu_shouldertoss1.bin ryu_shouldertoss2.bin ryu_shouldertoss3.bin ryu_shouldertoss4.bin ryu_shouldertoss5.bin \
	ryu_backroll1.bin ryu_backroll2.bin ryu_backroll3.bin ryu_backroll4.bin ryu_backroll5.bin ryu_backroll6.bin ryu_backroll7.bin \
	ryu_hit1.bin ryu_hit2.bin ryu_hit3.bin ryu_hit4.bin \
	ryu_facehit1.bin ryu_facehit2.bin ryu_facehit3.bin ryu_facehit4.bin \
	ryu_crouchhit1.bin ryu_crouchhit2.bin ryu_crouchhit3.bin \
	ryu_knockdown1.bin ryu_knockdown2.bin ryu_knockdown3.bin ryu_knockdown4.bin ryu_knockdown5.bin ryu_knockdown6.bin ryu_knockdown7.bin ryu_knockdown8.bin


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

