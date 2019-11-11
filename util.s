    .segment    "util"

	.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4

.EXPORT func_minikey, func_init_sid, func_prepare_adsrs, func_prepare_sid

; ----------------------------------
; variables
; ----------------------------------

; ----------------------------------
; functions
; ----------------------------------
; Groepaz's minikey technique
; https://codebase64.org/doku.php?id=base:reading_the_keyboard

func_minikey:
	lda #$0
	sta $dc03	; port b ddr (input)
	lda #$ff
	sta $dc02	; port a ddr (output)
			
	lda #$00
	sta $dc00	; port a
	lda $dc01 ; port b
	cmp #$ff
	beq nokey
	; got column
	tay
			
	lda #$7f
	sta nokey2+1
	ldx #8
nokey2:
	lda #0
	sta $dc00	; port a
	
	sec
	ror nokey2+1
	dex
	bmi nokey
			
	lda $dc01       ; port b
	cmp #$ff
	beq nokey2
			
	; got row in X
	txa
	ora columntab,y
			
	sec
	rts
			
nokey:
	clc
	rts

columntab:
	.repeat 256,count
		.if count = ($ff-$80)
			.byte $70
		.elseif count = ($ff-$40)
			.byte $60
		.elseif count = ($ff-$20)
			.byte $50
		.elseif count = ($ff-$10)
			.byte $40
		.elseif count = ($ff-$08)
			.byte $30
		.elseif count = ($ff-$04)
			.byte $20
		.elseif count = ($ff-$02)
			.byte $10
		.elseif count = ($ff-$01)
			.byte $00
		.else
			.byte $ff
		.endif
	.endrepeat

; ----------------------------------

.proc func_init_sid
func_init_sid:
  ; ptr1 = 0xd400 (start of sid registers)
  lda #$d4
  sta ptr1+1
  lda #$00
  sta ptr1

  ldy #24

l1:
  sta (ptr1),y
  dey
  bne l1
  rts
.endproc

; ----------------------------------

func_prepare_adsrs:
  lda #$00
  sta $d400+5           ; (0xd405) Set Attack/Decay for voice 1 (A=0, D=0)
                        ; aaaa dddd (a=attack, d=decay)

  lda #(15<<4) + 0
  sta $d400+6           ; Set Sustain/Release for voice 1 (S=15, R=0)
                        ; 1111 0000
                        ; ssss rrrr (s=sustain, r=release)

  lda #(2<<4) + 5
  sta $d400+12          ; (0xd40c) Set Attack/Decay for voice 2 (A=5, D=5)
                        ; 0101 0101
                        ; aaaa dddd (a=attack, d=decay)

  lda #(8<<4) + 1
  sta $d400+13          ; (0xd40d) Set Sustain/Release for voice 2 (S=8, R=5)
                        ; 1000 0101
                        ; ssss rrrr (s=sustain, r=release)

  lda #(0<<4) + 8
  sta $d400+19          ; Set Attack/Decay for voice 3 (A=0, D=10)
                        ; 0000 1010
                        ; aaaa dddd (a=attack, d=decay)

  lda #(0<<4) + 5
  sta $d400+20          ; Set Sustain/Release for voice 3 (S=0, R=5)
                        ; 0000 0101
                        ; ssss rrrr (s=sustain, r=release)
  rts

; ----------------------------------

func_prepare_sid:
  jsr func_init_sid
  jsr func_prepare_adsrs

  lda #31
  sta $d400+24          ; (0xd418) Set volume 15, low-pass filtering
                        ; 0001 1111
                        ; chbl vvvv (c=cutoff voice3 output 1=off/0=on, h=select high-pass filter, b=select band-pass filter,
                        ; l=select low-pass filter, v = volume)
  rts


