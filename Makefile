CC65=	/usr/local/bin/cc65
CL65=	/usr/local/bin/cl65
COPTS=	-t c64 -O -Or -Oi -Os --cpu 6502
LOPTS=	-C gidemo.cfg

ASSFILES = main.s \
						bmpmeta.s \
						charset.s \
						music.s \
						util.s
#						music_intro.s \
#						music_game.s \
#						music_game.s \
#						music_outro.s \
#						music_credits.s \
#						petscii.s \
#						sprites.s
#						charset.s

SNDFILES= $(wildcard Sound/*.wav)
SHORTSNDFILES=$(foreach file,$(SNDFILES),$(shell echo SND_`basename ${file}`))
SND_ID_NAMES=`echo $(SHORTSNDFILES:.wav=,) | tr \[a-z\] \[A-Z\]`

W64FILES = $(patsubst %.wav,%.w64,$(SNDFILES))

GRAPHIC_DIRS=__Misc__ Ryu StageRyu

DATAFILES = $(foreach dir,$(GRAPHIC_DIRS),$(patsubst %.png,%.bin,$(wildcard Graphics/${dir}/*.png)))

BMP_META_FILES=$(DATAFILES:bin=bin.bmp_meta)
SEGS_META_FILES=$(DATAFILES:bin=bin.segs_meta)
SHORTDATAFILES=$(foreach file,$(DATAFILES),$(shell basename ${file}))
ID_NAMES=`echo $(SHORTDATAFILES:.bin=,) | tr \[a-z\] \[A-Z\]`

DATAFILES_REV=$(DATAFILES:.bin=_rev.bin)
BMP_META_FILES_REV=$(DATAFILES_REV:bin=bin.bmp_meta)
SEGS_META_FILES_REV=$(DATAFILES_REV:bin=bin.segs_meta)
SHORTDATAFILES_REV=$(foreach file,$(DATAFILES_REV),$(shell basename ${file}))
ID_NAMES_REV=`echo $(SHORTDATAFILES_REV:.bin=,) | tr \[a-z\] \[A-Z\]`

all: gidemo.d81 data.reu util.h

run:
	/Users/tramvo/c64/mega65/xemu/build/bin/xmega65.native -hyperdebug -loadcram ../../../mega65-core/bin/COLOURRAM.BIN -skipunhandledmem -newhack -8 gidemo.d81 -fpga 12
	#open -a /Applications/Vice/x64.app/Contents/MacOS/x64 gidemo.d64
	#/Applications/Vice/x64.app/Contents/MacOS/x64 -reu -reusize 16384 -reuimage data.reu -moncommands commands.txt gidemo.d64 &
	#cmd /c "start c:/Users/gurcei/Downloads/GTK3VICE-3.3-win32-r35872/x64.exe --reuimage data.reu gidemo.d64"

%.s: %.c $(DATAFILES) $(W64FILES) gidemo.cfg ascii8x8.bin
	$(CC65) $(COPTS) --add-source -o $@ $<

data.reu: $(DATAFILES) $(W64FILES) Petscii/intro.bin
	rm -f data.reu
	rm -f segs_meta.reu
	rm -f bmp_meta.bin
	rm -f bitmap_ids.h
	# create a 'bitmap_ids.h' file to list ids for all bitmaps
	echo "enum BMP_IDS {" > bitmap_ids.h
	echo $(ID_NAMES) | sed 's/ /&~/; y/~/\n/;' >> bitmap_ids.h
	echo $(ID_NAMES_REV) | sed 's/ /&~/; y/~/\n/;' >> bitmap_ids.h
	echo "BMP_MAX" >> bitmap_ids.h
	echo "};" >> bitmap_ids.h

	rm -f sound_ids.h
	echo "enum sound_ids {" > sound_ids.h
	echo $(SND_ID_NAMES) >> sound_ids.h
	echo "};" >> sound_ids.h

	# bulk bitmap_meta.bin file will reside in c64 memory, at 0x1000
	cat $(BMP_META_FILES) > bmp_meta.bin
	cat $(BMP_META_FILES_REV) >> bmp_meta.bin
	
	# reu bank 0 is segment data
	cat $(SEGS_META_FILES) > data.reu
	cat $(SEGS_META_FILES_REV) >> data.reu
	dd if=/dev/zero of=data.reu bs=1 count=1 seek=65535

	# reu bank 1 is petscii screen data
	cat Petscii/intro.bin >> data.reu
	dd if=/dev/zero of=data.reu bs=1 count=1 seek=131071

	# reu bank 2 is music data
	cat Music/music_intro.bin >> data.reu
	cat Music/music_game.bin >> data.reu
	dd if=/dev/zero of=data.reu bs=1 count=1 seek=196607

	# reu bank 3 and beyond will hold digi-sound wave samples
	cat $(W64FILES) >> data.reu
	dd if=/dev/zero of=data.reu bs=1 count=1 seek=524287

	# skipping bank 4, seems like the reu-copy corrupts a little bit in here?

	# reu bank 8 and onwards is bitmap data
	cat $(DATAFILES) >> data.reu
	echo "sizeof(DATAFILES)"
	ls -lh data.reu
	cat $(DATAFILES_REV) >> data.reu
	echo "sizeof(DATAFILES)"
	ls -lh data.reu

util.h: gidemo.lbl
	cat gidemo.lbl | grep \.func_ | gsed "s/\(.*\) \(.*\) \.\(.*\)/\#define \U\3 \"$$\2\"/" > util.h
	cat gidemo.lbl | grep \.var_ | gsed "s/\(.*\) \(.*\) \.\(.*\)/\#define \U\3 \"$$\2\"/" >> util.h
	# For Mac OSX, might need to replace sed call with: awk '{print "#define " toupper(substr($3,2)) " \"$" $2 "\""}'
	# Alternatively, do "brew install gsed"

# 'pngprepare' and 'asciih' tools borrowed from mega65-ide project
# --------------------------------------------------
ascii8x8.bin: ascii00-7f.png pngprepare
	./pngprepare charrom ascii00-7f.png ascii8x8.bin 1

%.bin: %.png pngprepare
	./pngprepare gihires2 $< $@

pngprepare: pngprepare.c hitboxes.h
	$(CC) -I/usr/local/include -L/usr/local/lib -g -O0 -o pngprepare pngprepare.c -lpng
# --------------------------------------------------
%.w64: %.wav wav64
	./wav64 $< $@

wav64: wav64.c
	$(CC) wav64.c -o wav64
# --------------------------------------------------

gidemo.prg gidemo.lbl: data.reu $(ASSFILES) gidemo.cfg
	$(CL65) $(COPTS) $(LOPTS) -vm -l gidemo.list -m gidemo.map -Ln gidemo.lbl -o gidemo.prg $(ASSFILES)

gidemo.d81: gidemo.prg
	rm -f gidemo.d81
	cbmconvert -v2 -D8o gidemo.d81 gidemo.prg

clean:
	rm -f gidemo.map
	rm -f gidemo.prg
	rm -f main.s main.o
	rm -f gidemo.list
	rm -f gidemo.d81
	rm -f asciih ascii.h ascii8x8.bin
	rm -f data.reu
	rm -f *.bin *.bin.bmp_meta *.bin.segs_meta

