    .segment    "util"

; Groepaz's minikey technique
; https://codebase64.org/doku.php?id=base:reading_the_keyboard

minikey:
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
