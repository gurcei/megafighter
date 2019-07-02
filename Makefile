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
DATAFILES= ryu.bin

all: gidemo.d64

run:
	#open -a /Applications/Vice/x64.app/Contents/MacOS/x64 gidemo.d64
	#/Applications/Vice/x64.app/Contents/MacOS/x64 gidemo.d64 &
	cmd /c "start c:/Users/gurcei/Downloads/GTK3VICE-3.3-win32-r35872/x64.exe --reuimage ryu_idle1.bin gidemo.d64"

%.s:	%.c $(DATAFILES) gidemo.cfg
	$(CC65) $(COPTS) --add-source -o $@ $<

data.reu: $(DATAFILES)
	

# 'pngprepare' and 'asciih' tools borrowed from mega65-ide project
# --------------------------------------------------
# ascii8x8.bin: ascii00-7f.png pngprepare
# 	./pngprepare charrom ascii00-7f.png ascii8x8.bin

ryu.bin: ryu_idle1.png pngprepare
	./pngprepare gihires ryu_idle1.png ryu_idle1.bin

#dad.bin: dad.png pngprepare
#	./pngprepare 4sprmulti dad.png dad.bin 0 25
#
#mum.bin: mum.png pngprepare
#	./pngprepare 4sprmulti mum.png mum.bin 25 20
#
#tram.bin: tram.png pngprepare
#	./pngprepare 4sprmulti tram.png tram.bin 0 16

#asciih: asciih.c
#	$(CC) -o asciih asciih.c
#ascii.h:  asciih
#	./asciih

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

