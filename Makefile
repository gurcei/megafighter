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

# DATAFILES=  ascii8x8.bin dad.bin mum.bin tram.bin
DATAFILES= \
	title.bin \
	options_border_top.bin options_border_mid.bin options_border_bottom.bin \
	ryu_stage_cropped.bin \
	stage_ryu_floor00.bin stage_ryu_floor01.bin stage_ryu_floor02.bin stage_ryu_floor03.bin stage_ryu_floor04.bin stage_ryu_floor05.bin stage_ryu_floor06.bin stage_ryu_floor07.bin  stage_ryu_floor08.bin  stage_ryu_floor09.bin stage_ryu_floor10.bin stage_ryu_floor11.bin stage_ryu_floor12.bin stage_ryu_floor13.bin stage_ryu_floor14.bin stage_ryu_floor15.bin stage_ryu_floor16.bin stage_ryu_floor17.bin stage_ryu_floor18.bin   stage_ryu_floor19.bin stage_ryu_floor20.bin stage_ryu_floor21.bin stage_ryu_floor22.bin stage_ryu_floor23.bin stage_ryu_floor24.bin \
  stage_ryu_sky1.bin stage_ryu_sky2.bin stage_ryu_sky3.bin stage_ryu_sky4.bin stage_ryu_sky5.bin stage_ryu_sky6.bin stage_ryu_sky7.bin stage_ryu_sky8.bin \
	stage_ryu_fence_left1.bin stage_ryu_fence_left2.bin stage_ryu_fence_left3.bin stage_ryu_fence_left4.bin stage_ryu_fence_left5.bin stage_ryu_fence_left6.bin stage_ryu_fence_left7.bin stage_ryu_fence_left8.bin \
	stage_ryu_fence_right1.bin stage_ryu_fence_right2.bin stage_ryu_fence_right3.bin stage_ryu_fence_right4.bin stage_ryu_fence_right5.bin stage_ryu_fence_right6.bin stage_ryu_fence_right7.bin stage_ryu_fence_right8.bin \
	stage_ryu_building_left1.bin stage_ryu_building_left2.bin stage_ryu_building_left3.bin stage_ryu_building_left4.bin stage_ryu_building_left5.bin stage_ryu_building_left6.bin stage_ryu_building_left7.bin stage_ryu_building_left8.bin \
	stage_ryu_building_right1.bin stage_ryu_building_right2.bin stage_ryu_building_right3.bin stage_ryu_building_right4.bin stage_ryu_building_right5.bin stage_ryu_building_right6.bin stage_ryu_building_right7.bin stage_ryu_building_right8.bin \
  stage_ryu_temple1.bin stage_ryu_temple2.bin stage_ryu_temple3.bin stage_ryu_temple4.bin stage_ryu_temple5.bin stage_ryu_temple6.bin stage_ryu_temple7.bin stage_ryu_temple8.bin \
	ryu_idle2.bin ryu_idle3.bin ryu_idle4.bin \
	ryu_walk1.bin ryu_walk2.bin ryu_walk3.bin ryu_walk4.bin ryu_walk5.bin ryu_walk6.bin ryu_walk7.bin ryu_walk8.bin \
	ryu_jump2.bin ryu_jump3.bin ryu_jump4.bin \
	ryu_fjump1.bin ryu_fjump2.bin ryu_fjump3.bin ryu_fjump4.bin ryu_fjump5.bin ryu_fjump6.bin ryu_fjump7.bin \
	ryu_crouch1.bin ryu_crouch2.bin ryu_block1.bin ryu_block2.bin \
	ryu_lpunch1.bin ryu_lpunch2.bin \
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
	ryu_knockdown1.bin ryu_knockdown2.bin ryu_knockdown3.bin ryu_knockdown4.bin ryu_knockdown5.bin ryu_knockdown6.bin ryu_knockdown7.bin ryu_knockdown8.bin \
	ryu_stunned1.bin ryu_stunned2.bin ryu_stunned3.bin \
	ryu_ko1.bin ryu_ko2.bin ryu_ko3.bin ryu_ko4.bin ryu_ko5.bin \
	ryu_victory1.bin ryu_victory2.bin ryu_victory3.bin \
	ryu_victoryalt1.bin ryu_victoryalt2.bin ryu_victoryalt3.bin ryu_victoryalt4.bin ryu_victoryalt5.bin ryu_victoryalt6.bin ryu_victoryalt7.bin \
	ryu_mugshot1.bin ryu_mugshot2.bin ryu_mugshot3.bin

BMP_META_FILES=$(DATAFILES:bin=bin.bmp_meta)
SEGS_META_FILES=$(DATAFILES:bin=bin.segs_meta)
ID_NAMES=`echo $(DATAFILES:.bin=,) | tr \[a-z\] \[A-Z\]`

DATAFILES_REV=$(DATAFILES:.bin=_rev.bin)
BMP_META_FILES_REV=$(DATAFILES_REV:bin=bin.bmp_meta)
SEGS_META_FILES_REV=$(DATAFILES_REV:bin=bin.segs_meta)
ID_NAMES_REV=`echo $(DATAFILES_REV:.bin=,) | tr \[a-z\] \[A-Z\]`

all: gidemo.d64 data.reu util.h

run:
	#open -a /Applications/Vice/x64.app/Contents/MacOS/x64 gidemo.d64
	#/Applications/Vice/x64.app/Contents/MacOS/x64 gidemo.d64 &
	cmd /c "start c:/Users/gurcei/Downloads/GTK3VICE-3.3-win32-r35872/x64.exe --reuimage data.reu gidemo.d64"

%.s: %.c $(DATAFILES) gidemo.cfg ascii8x8.bin
	$(CC65) $(COPTS) --add-source -o $@ $<

data.reu: $(DATAFILES)
	rm -f data.reu
	rm -f segs_meta.reu
	rm -f bmp_meta.bin
	rm -f bitmap_ids.h
	# create a 'bitmap_ids.h' file to list ids for all bitmaps
	echo "enum BMP_IDS {" > bitmap_ids.h
	echo $(ID_NAMES) | sed "s/ /\n/g" >> bitmap_ids.h
	echo $(ID_NAMES_REV) | sed "s/ /\n/g" >> bitmap_ids.h
	echo "BMP_MAX" >> bitmap_ids.h
	echo "};" >> bitmap_ids.h

	# bulk bitmap_meta.bin file will reside in c64 memory, at 0x1000
	cat $(BMP_META_FILES) > bmp_meta.bin
	cat $(BMP_META_FILES_REV) >> bmp_meta.bin
	
	# reu bank 0 is segment data
	cat $(SEGS_META_FILES) > data.reu
	cat $(SEGS_META_FILES_REV) >> data.reu
	dd if=/dev/zero of=data.reu bs=1 count=1 seek=65535

	# reu bank 1 is petscii screen data
	cat intro.bin >> data.reu
	dd if=/dev/zero of=data.reu bs=1 count=1 seek=131071

	# reu bank 2 is music data
	cat music_intro.bin >> data.reu
	cat music_game.bin >> data.reu
	dd if=/dev/zero of=data.reu bs=1 count=1 seek=196607


	# reu bank 3 and onwards is bitmap data
	cat $(DATAFILES) >> data.reu
	echo "sizeof(DATAFILES)"
	ls -lh data.reu
	cat $(DATAFILES_REV) >> data.reu
	echo "sizeof(DATAFILES)"
	ls -lh data.reu
	dd if=/dev/zero of=data.reu bs=1 count=1 seek=16777215

util.h: gidemo.lbl
	cat gidemo.lbl | grep \.func_ | sed "s/\(.*\) \(.*\) \.\(.*\)/\#define \U\3 \L\"$$\2\"/" > util.h
	cat gidemo.lbl | grep \.var_ | sed "s/\(.*\) \(.*\) \.\(.*\)/\#define \U\3 \L\"$$\2\"/" >> util.h

# 'pngprepare' and 'asciih' tools borrowed from mega65-ide project
# --------------------------------------------------
ascii8x8.bin: ascii00-7f.png pngprepare
	./pngprepare charrom ascii00-7f.png ascii8x8.bin 1

%.bin: %.png pngprepare
	./pngprepare gihires2 $< $@

pngprepare: pngprepare.c
	$(CC) -I/usr/local/include -L/usr/local/lib -g -O0 -o pngprepare pngprepare.c -lpng
# --------------------------------------------------

gidemo.prg gidemo.lbl: data.reu $(ASSFILES) gidemo.cfg
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
	rm -f data.reu
	rm -f *.bin *.bin.bmp_meta *.bin.segs_meta

