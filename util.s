    .segment    "util"

  .autoimport on
	.importzp	sp, sreg, regsave, regbank
	.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
	.macpack	longbranch

; ----------------------------------
; exports
; ----------------------------------
.EXPORT func_minikey, func_init_sid, func_prepare_adsrs, func_prepare_sid
.EXPORT func_decode_note, func_music_loop_iteration, func_update_decoded_music
.EXPORT func_music_loop_preparation, func_prepare_waveform_control_registers
.EXPORT func_prepare_song, func_calculate_note_frequency
.EXPORT _prepare_song, _draw_sprintf, _dstr, _draw_text
.export		_screen_loc
.export		_rel_loc
.export		_gtmpw
.export		_gtmpw2
.export		_gtmpw3
.export		_gtmpw4
.export		_a
.export		_b
.export		_gk
.export		_gtmp
.export		_num_repairs
.export   _snd_trigger, _snd_idx, _snd_delay, _sky_idx

; .EXPORT _fq, _dr, _oc, _nt, _pnm, _delay_cnt, _use_bug, _k, _v1, _v2, _v3

; ----------------------------------
; variables
; ----------------------------------

_screen_loc:
	.res	2,$00
_rel_loc:
	.res	2,$00
_gtmpw:
	.res	2,$00
_gtmpw2:
	.res	2,$00
_gtmpw3:
	.res	2,$00
_gtmpw4:
	.res	2,$00
_a:
	.res	1,$00
_b:
	.res	1,$00
_gk:
	.res	1,$00
_gtmp:
	.res	1,$00
_num_repairs:
	.res	1,$00
_dstr:
	.res	40,$00

_snd_trigger:
	.byte	$00
_snd_idx:
	.byte	$00
_snd_delay:
	.byte	$00
_sky_idx:
	.word	$0000

; define frequency table
_fq:
  .word 34334, 36376, 38539, 40830  ; C,  C#, D,  D#
  .word 43258, 45830, 48556, 51443  ; E,  F,  F#, G
  .word 54502, 57743, 61176, 64814  ; G#, A,  A#, B

_dr:  .word 0   ; duration of currently decoded note-data
_oc:  .word 0   ; octave of currently decoded note-data
_nt:  .word 0   ; note-in-octave of currently decoded note-data

_pnm: .res 2, $00 ; pointer to current nm

_delay_cnt:
	.byte	$03
_use_bug:
	.byte	$01

_k:
	.res	1,$00

_v1:
	.res	2,$00
_v2:
	.res	2,$00
_v3:
	.res	2,$00

_autogateflag:
	.word	$0001
	.word	$0001
	.word	$0001

_sub_idx_v1:
	.word	$0000
	.res	10,$00
_sub_idx_v2:
	.word	$0000
	.res	10,$00
_sub_idx_v3:
	.word	$0000
	.res	10,$00
_psub_idxs:
	.addr	_sub_idx_v1
	.addr	_sub_idx_v2
	.addr	_sub_idx_v3
_psub_idx:
	.res	2,$00
_pstart_idx:
	.res	2,$00
_start_idx:
	.word	$FFFF
	.word	$FFFF
	.word	$FFFF
_ph:
	.res	2,$00
_pl:
	.res	2,$00
_pc:
	.res	2,$00
_h:
	.res	3,$00
_l:
	.res	3,$00
_c:
	.res	3,$00
_pcaller_idx:
	.res	2,$00
_caller_idx:
	.word	$FFFF
	.res	4,$00
_v:
	.byte	$11
	.byte	$41
	.byte	$81
_marker_pos:
	.word	$0000
_repeat_to_marker_pos:
	.word	$0000
_repeat_to_marker_count:
	.word	$0000
_repeat_to_beginning_pos:
	.word	$0000
_rptcnt:
	.word	$0000
_nm:
	.word	$0000
	.res	4,$00
_pwa:
	.res	2,$00
_pwb:
	.res	2,$00
_wa:
	.byte	$00
	.res	2,$00
_wb:
	.byte	$00
	.res	2,$00
_fr:
	.dword	$00000000
_hf:
	.res	2,$00
_lf:
	.res	2,$00
_bar_idx:
	.byte	$00
_music_idx:
	.word	$0000
_music_t:
	.byte	$00
_j:
	.res	2,$00
_bar_length:
	.byte	$48
_cmd:
	.res	2,$00
_data:
	.res	2,$00
_pidx:
	.res	2,$00
_idx:
	.word	$0000
	.res	4,$00
_premain:
	.res	2,$00
_remain:
	.byte	$00
	.res	2,$00
_found_note:
	.byte	$00
_within_sub:
	.byte	$00
_ppvoice:
	.res	2,$00
_pvoice:
	.res	2,$00
_pptr_voice:
	.res	2,$00
_ptr_voice:
	.res	6,$00
_pvoices:
	.res	6,$00
_pval:
	.res	2,$00
_sid_offset:
	.byte	$00
	.byte	$07
	.byte	$0E
; ----------------------------------
; functions
; ----------------------------------

func_decode_note:
;
; dr = *pnm >> 7;
;
	lda     _pnm+1
	sta     ptr1+1
	lda     _pnm
	sta     ptr1
	ldy     #$01
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     asrax4
	jsr     asrax3
	sta     _dr
	stx     _dr+1
;
; oc = (*pnm >> 4) & 0x07;
;
	lda     _pnm+1
	sta     ptr1+1
	lda     _pnm
	sta     ptr1
	iny
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     asrax4
	ldx     #$00
	and     #$07
	sta     _oc
	stx     _oc+1
;
; nt = *pnm & 0x0F;
;
	lda     _pnm+1
	sta     ptr1+1
	lda     _pnm
	sta     ptr1
	lda     (ptr1),y
	and     #$0F
	sta     _nt
	stx     _nt+1
;
; }
;
	rts

; ---------------------------------------------------------------
; void __near__ calculate_note_frequency (void)
; ---------------------------------------------------------------

func_calculate_note_frequency:
.proc	_calculate_note_frequency: near

;
; fr = fq[nt];
;
	lda     _nt
	ldx     _nt+1
	stx     tmp1
	asl     a
	rol     tmp1
	clc
	adc     #<(_fq)
	sta     ptr1
	lda     tmp1
	adc     #>(_fq)
	sta     ptr1+1
	ldy     #$01
	lda     (ptr1),y
	sta     _fr+1
	dey
	lda     (ptr1),y
	sta     _fr
	sty     _fr+2
	sty     _fr+3
;
; if (oc != 7)
;
	lda     _oc+1
	bne     L005D
	lda     _oc
	cmp     #$07
	beq     L0034
;
; for (j = 6; j >= oc; j--)
;
L005D:	ldx     #$00
	lda     #$06
L005F:	sta     _j
	stx     _j+1
	lda     _j
	cmp     _oc
	lda     _j+1
	sbc     _oc+1
	bvs     L005C
	eor     #$80
L005C:	bpl     L0034
;
; fr = fr >> 1;
;
	lda     _fr+3
	sta     sreg+1
	lda     _fr+2
	sta     sreg
	ldx     _fr+1
	lda     _fr
	jsr     shreax1
	sta     _fr
	stx     _fr+1
	ldy     sreg
	sty     _fr+2
	ldy     sreg+1
	sty     _fr+3
;
; for (j = 6; j >= oc; j--)
;
	lda     _j
	ldx     _j+1
	sec
	sbc     #$01
	bcs     L005F
	dex
	jmp     L005F
;
; hf = fr >> 8;
;
L0034:	lda     _fr+2
	sta     sreg
	ldx     _fr+1
	stx     _hf
	ldx     sreg
	stx     _hf+1
;
; lf = fr & 0xff;
;
	lda     #$00
	sta     _lf+1
	lda     _fr
	sta     _lf
;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ prepare_song (__near__ int *, __near__ int *, __near__ int *, unsigned char, unsigned char, unsigned char)
; ---------------------------------------------------------------

func_prepare_song:
.proc	_prepare_song: near

;
; {
;
	jsr     pusha
;
; v1 = pv1;
;
	ldy     #$08
	lda     (sp),y
	sta     _v1+1
	dey
	lda     (sp),y
	sta     _v1
;
; v2 = pv2;
;
	dey
	lda     (sp),y
	sta     _v2+1
	dey
	lda     (sp),y
	sta     _v2
;
; v3 = pv3;
;
	dey
	lda     (sp),y
	sta     _v3+1
	dey
	lda     (sp),y
	sta     _v3
;
; bar_length = p_bar_length;
;
	dey
	lda     (sp),y
	sta     _bar_length
;
; delay_cnt = p_delay_cnt;
;
	dey
	lda     (sp),y
	sta     _delay_cnt
;
; use_bug = p_use_bug;
;
	dey
	lda     (sp),y
	sta     _use_bug
;
; }
;
	ldy     #$09
	jmp     addysp

.endproc
; ---------------------------------------------------------------
; void __near__ prepare_waveform_control_registers (void)
; ---------------------------------------------------------------

func_prepare_waveform_control_registers:
.proc	_prepare_waveform_control_registers: near

;
; *pwa = v[k];   // set the waveform control to proper voice
;
	lda     _pwa+1
	sta     ptr1+1
	lda     _pwa
	sta     ptr1
	ldy     _k
	lda     _v,y
	ldy     #$00
	sta     (ptr1),y
;
; *pwb = *pwa - 1; // turn the gate-bit (bit0) of the voice's control-register off (releases the note from the sustain)
;
	lda     _pwb+1
	sta     sreg+1
	lda     _pwb
	sta     sreg
	lda     _pwa+1
	sta     ptr1+1
	lda     _pwa
	sta     ptr1
	lda     (ptr1),y
	sec
	sbc     #$01
	sta     (sreg),y
;
; if (*pnm < 0) // if encoded note value is negative, this equates to silence, so set waveform controls to 0.
;
	lda     _pnm+1
	sta     ptr1+1
	lda     _pnm
	sta     ptr1
	iny
	lda     (ptr1),y
	tax
	cpx     #$80
	bcc     L0058
;
; *pnm = -*pnm; // invert it, and what remains equates to a duration value
;
	lda     _pnm+1
	sta     sreg+1
	lda     _pnm
	sta     sreg
	lda     _pnm+1
	sta     ptr1+1
	lda     _pnm
	sta     ptr1
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     negax
	sta     (sreg),y
	iny
	txa
	sta     (sreg),y
;
; *pwa = 0;
;
	lda     _pwa+1
	sta     ptr1+1
	lda     _pwa
	sta     ptr1
	lda     #$00
	dey
	sta     (ptr1),y
;
; *pwb = 0;
;
	lda     _pwb+1
	sta     ptr1+1
	lda     _pwb
	sta     ptr1
	tya
	sta     (ptr1),y
;
; }
;
L0058:	rts

.endproc
; ---------------------------------------------------------------
; void __near__ music_loop_preparation (void)
; ---------------------------------------------------------------

func_music_loop_preparation:
.proc	_music_loop_preparation: near

;
; rptcnt = repeat_to_marker_count;
;
	lda     _repeat_to_marker_count+1
	sta     _rptcnt+1
	lda     _repeat_to_marker_count
	sta     _rptcnt
;
; start_idx[0] = -1;
;
	lda     #$FF
	sta     _start_idx
	sta     _start_idx+1
;
; start_idx[1] = -1;
;
	sta     _start_idx+2
	sta     _start_idx+2+1
;
; start_idx[2] = -1;
;
	sta     _start_idx+4
	sta     _start_idx+4+1
;
; caller_idx[0] = -1;
;
	sta     _caller_idx
	sta     _caller_idx+1
;
; caller_idx[1] = -1;
;
	sta     _caller_idx+2
	sta     _caller_idx+2+1
;
; caller_idx[2] = -1;
;
	sta     _caller_idx+4
	sta     _caller_idx+4+1
;
; bar_idx = 0;
;
	lda     #$00
	sta     _bar_idx
;
; music_idx = 0;
;
	tax
	sta     _music_idx
	sta     _music_idx+1
;
; music_t = 0;
;
	sta     _music_t
;
; marker_pos = 0;
;
	sta     _marker_pos
	sta     _marker_pos+1
;
; repeat_to_marker_pos = 0;
;
	sta     _repeat_to_marker_pos
	sta     _repeat_to_marker_pos+1
;
; repeat_to_marker_count = 0;
;
	sta     _repeat_to_marker_count
	sta     _repeat_to_marker_count+1
;
; repeat_to_beginning_pos = 0;
;
	sta     _repeat_to_beginning_pos
	sta     _repeat_to_beginning_pos+1
;
; rptcnt = 0;
;
	sta     _rptcnt
	sta     _rptcnt+1
;
; idx[0] = 0;
;
	sta     _idx
	sta     _idx+1
;
; idx[1] = 0;
;
	sta     _idx+2
	sta     _idx+2+1
;
; idx[2] = 0;
;
	sta     _idx+4
	sta     _idx+4+1
;
; remain[0] = 0;
;
	sta     _remain
;
; remain[1] = 0;
;
	sta     _remain+1
;
; remain[2] = 0;
;
	sta     _remain+2
;
; found_note = 0;
;
	sta     _found_note
;
; within_sub = 0;
;
	sta     _within_sub
;
; autogateflag[0] = 1;
;
	lda     #$01
	sta     _autogateflag
	stx     _autogateflag+1
;
; autogateflag[1] = 1;
;
	sta     _autogateflag+2
	stx     _autogateflag+2+1
;
; autogateflag[2] = 1;
;
	sta     _autogateflag+4
	stx     _autogateflag+4+1
;
; v[0] = C_TRIANGLE + C_GATE;
;
	lda     #$11
	sta     _v
;
; v[1] = C_PULSE + C_GATE;
;
	lda     #$41
	sta     _v+1
;
; v[2] = C_RANDOM + C_GATE;
;
	lda     #$81
	sta     _v+2
;
; pvoices[0] = v1;
;
	lda     _v1+1
	sta     _pvoices+1
	lda     _v1
	sta     _pvoices
;
; pvoices[1] = v2;
;
	lda     _v2+1
	sta     _pvoices+2+1
	lda     _v2
	sta     _pvoices+2
;
; pvoices[2] = v3;
;
	lda     _v3+1
	sta     _pvoices+4+1
	lda     _v3
	sta     _pvoices+4
;
; ptr_voice[0] = v1;
;
	lda     _v1+1
	sta     _ptr_voice+1
	lda     _v1
	sta     _ptr_voice
;
; ptr_voice[1] = v2;
;
	lda     _v2+1
	sta     _ptr_voice+2+1
	lda     _v2
	sta     _ptr_voice+2
;
; ptr_voice[2] = v3;
;
	lda     _v3+1
	sta     _ptr_voice+4+1
	lda     _v3
	sta     _ptr_voice+4
;
; Poke(_SID_+10, 8);   // (0xd40a) Set high pulse width for voice 2
;
	lda     #$08
	sta     $D40A
;
; Poke(_SID_+22, 128); // (0xd416) Set high frequency for filter cutoff
;
	lda     #$80
	sta     $D416
;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; int __near__ update_decoded_music (void)
; ---------------------------------------------------------------

func_update_decoded_music:
.proc	_update_decoded_music: near

;
; pidx = idx;
;
	lda     #>(_idx)
	sta     _pidx+1
	lda     #<(_idx)
	sta     _pidx
;
; premain = remain;
;
	lda     #>(_remain)
	sta     _premain+1
	lda     #<(_remain)
	sta     _premain
;
; pnm = nm;
;
	lda     #>(_nm)
	sta     _pnm+1
	lda     #<(_nm)
	sta     _pnm
;
; pstart_idx = start_idx;
;
	lda     #>(_start_idx)
	sta     _pstart_idx+1
	lda     #<(_start_idx)
	sta     _pstart_idx
;
; pcaller_idx = caller_idx;
;
	lda     #>(_caller_idx)
	sta     _pcaller_idx+1
	lda     #<(_caller_idx)
	sta     _pcaller_idx
;
; ppvoice = pvoices;
;
	lda     #>(_pvoices)
	sta     _ppvoice+1
	lda     #<(_pvoices)
	sta     _ppvoice
;
; pptr_voice = ptr_voice;
;
	lda     #>(_ptr_voice)
	sta     _pptr_voice+1
	lda     #<(_ptr_voice)
	sta     _pptr_voice
;
; psub_idx = psub_idxs;
;
	lda     #>(_psub_idxs)
	sta     _psub_idx+1
	lda     #<(_psub_idxs)
	sta     _psub_idx
;
; for (k = 0; k <= 2; k++)
;
	lda     #$00
	sta     _k
L01EA:	lda     _k
	cmp     #$03
	jcs     L0080
;
; found_note = 0;
;
	lda     #$00
	sta     _found_note
;
; within_sub = 0;
;
	sta     _within_sub
;
; pvoice = *ppvoice;
;
	lda     _ppvoice+1
	sta     ptr1+1
	lda     _ppvoice
	sta     ptr1
	ldy     #$01
	lda     (ptr1),y
	sta     _pvoice+1
	dey
	lda     (ptr1),y
	sta     _pvoice
;
; pval = *pptr_voice;
;
	lda     _pptr_voice+1
	sta     ptr1+1
	lda     _pptr_voice
	sta     ptr1
	iny
	lda     (ptr1),y
	sta     _pval+1
	dey
	lda     (ptr1),y
	sta     _pval
;
; if (*premain == 0)
;
	lda     _premain+1
	sta     ptr1+1
	lda     _premain
	sta     ptr1
	lda     (ptr1),y
	jne     L0092
;
; while (!found_note)
;
	jmp     L0156
;
; *pnm = *pval;
;
L0091:	lda     _pnm
	ldx     _pnm+1
	jsr     pushax
	lda     _pval+1
	sta     ptr1+1
	lda     _pval
	sta     ptr1
	ldy     #$01
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     staxspidx
;
; if (*pnm < 0)  // is this a rest note?
;
	lda     _pnm+1
	sta     ptr1+1
	lda     _pnm
	sta     ptr1
	iny
	lda     (ptr1),y
	tax
	cpx     #$80
	bcc     L0097
;
; if (!within_sub)
;
	lda     _within_sub
	jne     L0157
;
; found_note = 1;
;
	sty     _found_note
;
; if (*pstart_idx == -1)
;
	lda     _pstart_idx+1
	sta     ptr1+1
	lda     _pstart_idx
	sta     ptr1
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	cpx     #$FF
	jne     L0092
	cmp     #$FF
	jne     L0092
;
; *pstart_idx = (int)pval;
;
	lda     _pstart_idx+1
	sta     ptr1+1
	lda     _pstart_idx
	sta     ptr1
	lda     _pval
	sta     (ptr1),y
	iny
	lda     _pval+1
	sta     (ptr1),y
;
; break;
;
	jmp     L0092
;
; else if (*pnm == REPEAT_TO_BEGINNING)
;
L0097:	lda     _pnm+1
	sta     ptr1+1
	lda     _pnm
	sta     ptr1
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	cpx     #$40
	bne     L00A5
	cmp     #$00
	bne     L00A5
;
; pval = (int*)*pstart_idx; // back to start pointer of track
;
	lda     _pstart_idx+1
	sta     ptr1+1
	lda     _pstart_idx
	sta     ptr1
	iny
	lda     (ptr1),y
	sta     _pval+1
	dey
	lda     (ptr1),y
	sta     _pval
;
; *premain = 0;
;
	lda     _premain+1
	sta     ptr1+1
	lda     _premain
	sta     ptr1
	tya
	sta     (ptr1),y
;
; else if (*pnm & SPECIAL_TOKEN)
;
	jmp     L0156
L00A5:	lda     _pnm+1
	sta     ptr1+1
	lda     _pnm
	sta     ptr1
	iny
	lda     (ptr1),y
	and     #$20
	jeq     L00AD
;
; cmd = *pnm & 0x1F;
;
	lda     _pnm+1
	sta     ptr1+1
	lda     _pnm
	sta     ptr1
	dey
	lda     (ptr1),y
	ldx     #$00
	and     #$1F
	sta     _cmd
	stx     _cmd+1
;
; data = (*pnm >> 5) & 0xff;
;
	lda     _pnm+1
	sta     ptr1+1
	lda     _pnm
	sta     ptr1
	iny
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     asrax4
	jsr     asrax1
	ldx     #$00
	sta     _data
	stx     _data+1
;
; pval++;
;
	lda     _pval
	ldx     _pval+1
	clc
	adc     #$02
	bcc     L00B5
	inx
L00B5:	sta     _pval
	stx     _pval+1
;
; if (!within_sub)  // only parse certain commands if we are not in the initial sub-hunting phase...
;
	lda     _within_sub
	jne     L00C4
;
; if (*pstart_idx == -1 && cmd != CMD_DEFINE_SUB)
;
	lda     _pstart_idx+1
	sta     ptr1+1
	lda     _pstart_idx
	sta     ptr1
	iny
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	cpx     #$FF
	bne     L00B8
	cmp     #$FF
	bne     L00B8
	lda     _cmd+1
	bne     L00B9
	lda     _cmd
	cmp     #$06
	beq     L00B8
;
; *pstart_idx = (int)(pval-1); // *pidx;
;
L00B9:	lda     _pstart_idx+1
	sta     ptr1+1
	lda     _pstart_idx
	sta     ptr1
	lda     _pval
	ldx     _pval+1
	sec
	sbc     #$02
	bcs     L01EB
	dex
L01EB:	sta     (ptr1),y
	iny
	txa
	sta     (ptr1),y
;
; switch(cmd)
;
L00B8:	lda     _cmd
	ldx     _cmd+1
;
; } // end switch
;
	cpx     #$00
	jne     L00C4
	cmp     #$01
	beq     L00C6
	cmp     #$02
	beq     L01EC
	cmp     #$03
	beq     L00D1
	cmp     #$04
	jeq     L01ED
	cmp     #$05
	jeq     L00FE
	cmp     #$09
	jeq     L01EE
	cmp     #$0A
	jeq     L00EE
	cmp     #$0B
	jeq     L01EF
	jmp     L00C4
;
; v[k] = data;
;
L00C6:	ldy     _k
	lda     _data
	sta     _v,y
;
; break;
;
	jmp     L00C4
;
; autogateflag[k] = data;
;
L01EC:	lda     _k
	asl     a
	bcc     L01E8
	inx
	clc
L01E8:	adc     #<(_autogateflag)
	sta     ptr1
	txa
	adc     #>(_autogateflag)
	sta     ptr1+1
	lda     _data
	ldy     #$00
	sta     (ptr1),y
	iny
	lda     _data+1
	sta     (ptr1),y
;
; break;
;
	jmp     L00C4
;
; data = *pval; // pvoice[*pidx];
;
L00D1:	lda     _pval+1
	sta     ptr1+1
	lda     _pval
	sta     ptr1
	ldy     #$01
	lda     (ptr1),y
	sta     _data+1
	lda     (ptr1,x)
	sta     _data
;
; PokeW(_SID_+(sid_offset[k])+2, data);
;
	ldy     _k
	lda     _sid_offset,y
	pha
	txa
	clc
	adc     #$D4
	tax
	pla
	clc
	adc     #$02
	bcc     L00DA
	inx
L00DA:	sta     ptr1
	stx     ptr1+1
	lda     _data
	ldy     #$00
	sta     (ptr1),y
	iny
	lda     _data+1
	sta     (ptr1),y
;
; pval++;
;
	lda     _pval
	ldx     _pval+1
	clc
	adc     #$02
	bcc     L00DE
	inx
L00DE:	sta     _pval
	stx     _pval+1
;
; break;
;
	jmp     L00C4
;
; Poke(_SID_+0x17, data);
;
L01ED:	lda     _data
	sta     $D417
;
; break;
;
	jmp     L00C4
;
; Poke(_SID_+0x18, data);
;
L01EE:	lda     _data
	sta     $D418
;
; break;
;
	jmp     L00C4
;
; data = *pval;
;
L00EE:	lda     _pval+1
	sta     ptr1+1
	lda     _pval
	sta     ptr1
	ldy     #$01
	lda     (ptr1),y
	sta     _data+1
	lda     (ptr1,x)
	sta     _data
;
; Poke(_SID_+0x15, data & 0x07);  // low bits 0-2
;
	and     #$07
	sta     $D415
;
; Poke(_SID_+0x16, data << 3); // high byte (bits 3-10)
;
	lda     _data
	asl     a
	asl     a
	asl     a
	sta     $D416
;
; pval++;
;
	lda     _pval
	ldx     _pval+1
	clc
	adc     #$02
	bcc     L00FC
	inx
L00FC:	sta     _pval
	stx     _pval+1
;
; break;
;
	jmp     L00C4
;
; data = *pval; // pvoice[*pidx];
;
L00FE:	lda     _pval+1
	sta     ptr1+1
	lda     _pval
	sta     ptr1
	ldy     #$01
	lda     (ptr1),y
	sta     _data+1
	lda     (ptr1,x)
	sta     _data
;
; if (use_bug)
;
	lda     _use_bug
	beq     L0101
;
; PokeW(_SID_+(sid_offset[k])+5, data);
;
	ldy     _k
	lda     _sid_offset,y
	pha
	txa
	clc
	adc     #$D4
	tax
	pla
	clc
	adc     #$05
	bcc     L0109
	inx
L0109:	sta     ptr1
	stx     ptr1+1
	lda     _data
	ldy     #$00
	sta     (ptr1),y
	iny
	lda     _data+1
	sta     (ptr1),y
;
; Poke(_SID_+19, (0<<4) + 8);   // Set Attack/Decay for voice 3 (A=0, D=10)
;
	lda     #$08
	sta     $D413
;
; Poke(_SID_+20, (0<<4) + 5);  // Set Sustain/Release for voice 3 (S=0, R=5)
;
	lda     #$05
	sta     $D414
;
; else
;
	jmp     L0118
;
; Poke(_SID_+(sid_offset[k])+5, data & 0xff);
;
L0101:	ldy     _k
	lda     _sid_offset,y
	pha
	txa
	clc
	adc     #$D4
	tax
	pla
	clc
	adc     #$05
	bcc     L011F
	inx
L011F:	sta     ptr1
	stx     ptr1+1
	lda     _data
	ldy     #$00
	sta     (ptr1),y
;
; Poke(_SID_+(sid_offset[k])+6, (data >> 8) & 0xff);
;
	ldy     _k
	lda     _sid_offset,y
	pha
	lda     #$00
	clc
	adc     #$D4
	tax
	pla
	clc
	adc     #$06
	bcc     L0128
	inx
L0128:	sta     ptr1
	stx     ptr1+1
	lda     _data+1
	ldy     #$00
	sta     (ptr1),y
;
; pval++;
;
L0118:	lda     _pval
	ldx     _pval+1
	clc
	adc     #$02
	bcc     L012D
	inx
L012D:	sta     _pval
	stx     _pval+1
;
; break;
;
	jmp     L00C4
;
; screen_flag = 1;
;
L01EF:	lda     #$01
	sta     _screen_flag
;
; switch(cmd)
;
L00C4:	lda     _cmd
	ldx     _cmd+1
;
; }
;
	cpx     #$00
	jne     L0156
	cmp     #$06
	beq     L0136
	cmp     #$07
	beq     L013E
	cmp     #$08
	beq     L0146
	jmp     L0156
;
; (*psub_idx)[data] = (int)pval;
;
L0136:	lda     _psub_idx+1
	sta     ptr1+1
	lda     _psub_idx
	sta     ptr1
	ldy     #$01
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     pushax
	lda     _data
	ldx     _data+1
	stx     tmp1
	asl     a
	rol     tmp1
	ldx     tmp1
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	lda     _pval
	ldy     #$00
	sta     (ptr1),y
	iny
	lda     _pval+1
	sta     (ptr1),y
;
; within_sub = 1;
;
	sty     _within_sub
;
; break;
;
	jmp     L0156
;
; if (!within_sub)
;
L013E:	lda     _within_sub
	bne     L01F0
;
; pval = (int*)*pcaller_idx;
;
	lda     _pcaller_idx+1
	sta     ptr1+1
	lda     _pcaller_idx
	sta     ptr1
	ldy     #$01
	lda     (ptr1),y
	sta     _pval+1
	lda     (ptr1,x)
	sta     _pval
;
; within_sub = 0;
;
L01F0:	stx     _within_sub
;
; break;
;
	jmp     L0156
;
; if (*pstart_idx == -1)
;
L0146:	lda     _pstart_idx+1
	sta     ptr1+1
	lda     _pstart_idx
	sta     ptr1
	ldy     #$01
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	cpx     #$FF
	bne     L0147
	cmp     #$FF
	bne     L0147
;
; *pstart_idx = (int)(pval - 1); // *pidx - 1;
;
	lda     _pstart_idx+1
	sta     ptr1+1
	lda     _pstart_idx
	sta     ptr1
	lda     _pval
	ldx     _pval+1
	sec
	sbc     #$02
	bcs     L01F1
	dex
L01F1:	sta     (ptr1),y
	iny
	txa
	sta     (ptr1),y
;
; *pcaller_idx = (int)(pval); // *pidx;
;
L0147:	lda     _pcaller_idx+1
	sta     ptr1+1
	lda     _pcaller_idx
	sta     ptr1
	lda     _pval
	ldy     #$00
	sta     (ptr1),y
	iny
	lda     _pval+1
	sta     (ptr1),y
;
; pval = (int*)((*psub_idx)[data]);
;
	lda     _psub_idx+1
	sta     ptr1+1
	lda     _psub_idx
	sta     ptr1
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     pushax
	lda     _data
	ldx     _data+1
	stx     tmp1
	asl     a
	rol     tmp1
	ldx     tmp1
	jsr     tosaddax
	sta     ptr1
	stx     ptr1+1
	ldy     #$01
	lda     (ptr1),y
	sta     _pval+1
	dey
	lda     (ptr1),y
	sta     _pval
;
; break;
;
	jmp     L0156
;
; if (!within_sub)
;
L00AD:	lda     _within_sub
	bne     L0157
;
; found_note = 1;
;
	sty     _found_note
;
; if (*pstart_idx == -1)
;
	lda     _pstart_idx+1
	sta     ptr1+1
	lda     _pstart_idx
	sta     ptr1
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	cpx     #$FF
	bne     L0092
	cmp     #$FF
	bne     L0092
;
; *pstart_idx = (int)pval; // *pidx;
;
	lda     _pstart_idx+1
	sta     ptr1+1
	lda     _pstart_idx
	sta     ptr1
	lda     _pval
	sta     (ptr1),y
	iny
	lda     _pval+1
	sta     (ptr1),y
;
; break;
;
	jmp     L0092
;
; pval++;
;
L0157:	lda     _pval
	ldx     _pval+1
	clc
	adc     #$02
	bcc     L0161
	inx
L0161:	sta     _pval
	stx     _pval+1
;
; while (!found_note)
;
L0156:	lda     _found_note
	jeq     L0091
;
; *pptr_voice = pval;
;
L0092:	lda     _pptr_voice+1
	sta     ptr1+1
	lda     _pptr_voice
	sta     ptr1
	lda     _pval
	ldy     #$00
	sta     (ptr1),y
	iny
	lda     _pval+1
	sta     (ptr1),y
;
; premain++;
;
	lda     _premain
	ldx     _premain+1
	clc
	adc     #$01
	bcc     L0165
	inx
L0165:	sta     _premain
	stx     _premain+1
;
; pnm++;
;
	lda     _pnm
	ldx     _pnm+1
	clc
	adc     #$02
	bcc     L0167
	inx
L0167:	sta     _pnm
	stx     _pnm+1
;
; pidx++;
;
	lda     _pidx
	ldx     _pidx+1
	clc
	adc     #$02
	bcc     L0169
	inx
L0169:	sta     _pidx
	stx     _pidx+1
;
; pstart_idx++;
;
	lda     _pstart_idx
	ldx     _pstart_idx+1
	clc
	adc     #$02
	bcc     L016B
	inx
L016B:	sta     _pstart_idx
	stx     _pstart_idx+1
;
; pcaller_idx++;
;
	lda     _pcaller_idx
	ldx     _pcaller_idx+1
	clc
	adc     #$02
	bcc     L016D
	inx
L016D:	sta     _pcaller_idx
	stx     _pcaller_idx+1
;
; ppvoice++;
;
	lda     _ppvoice
	ldx     _ppvoice+1
	clc
	adc     #$02
	bcc     L016F
	inx
L016F:	sta     _ppvoice
	stx     _ppvoice+1
;
; pptr_voice++;
;
	lda     _pptr_voice
	ldx     _pptr_voice+1
	clc
	adc     #$02
	bcc     L0171
	inx
L0171:	sta     _pptr_voice
	stx     _pptr_voice+1
;
; psub_idx++;
;
	lda     _psub_idx
	ldx     _psub_idx+1
	clc
	adc     #$02
	bcc     L0173
	inx
L0173:	sta     _psub_idx
	stx     _psub_idx+1
;
; for (k = 0; k <= 2; k++)
;
	inc     _k
	jmp     L01EA
;
; pidx = idx;
;
L0080:	lda     #>(_idx)
	sta     _pidx+1
	lda     #<(_idx)
	sta     _pidx
;
; pnm = nm;
;
	lda     #>(_nm)
	sta     _pnm+1
	lda     #<(_nm)
	sta     _pnm
;
; premain = remain;
;
	lda     #>(_remain)
	sta     _premain+1
	lda     #<(_remain)
	sta     _premain
;
; pl = l;
;
	lda     #>(_l)
	sta     _pl+1
	lda     #<(_l)
	sta     _pl
;
; ph = h;
;
	lda     #>(_h)
	sta     _ph+1
	lda     #<(_h)
	sta     _ph
;
; pc = c;
;
	lda     #>(_c)
	sta     _pc+1
	lda     #<(_c)
	sta     _pc
;
; ppvoice = pvoices;
;
	lda     #>(_pvoices)
	sta     _ppvoice+1
	lda     #<(_pvoices)
	sta     _ppvoice
;
; pptr_voice = ptr_voice;
;
	lda     #>(_ptr_voice)
	sta     _pptr_voice+1
	lda     #<(_ptr_voice)
	sta     _pptr_voice
;
; pwa = wa;
;
	lda     #>(_wa)
	sta     _pwa+1
	lda     #<(_wa)
	sta     _pwa
;
; pwb = wb;
;
	lda     #>(_wb)
	sta     _pwb+1
	lda     #<(_wb)
	sta     _pwb
;
; for (k = 0; k <= 2; k++)
;
	lda     #$00
	sta     _k
	tax
L01F2:	lda     _k
	cmp     #$03
	jcs     L01F4
;
; pvoice = *ppvoice;
;
	lda     _ppvoice+1
	sta     ptr1+1
	lda     _ppvoice
	sta     ptr1
	ldy     #$01
	lda     (ptr1),y
	sta     _pvoice+1
	lda     (ptr1,x)
	sta     _pvoice
;
; pval = *pptr_voice;
;
	lda     _pptr_voice+1
	sta     ptr1+1
	lda     _pptr_voice
	sta     ptr1
	ldy     #$01
	lda     (ptr1),y
	sta     _pval+1
	dey
	lda     (ptr1),y
	sta     _pval
;
; if (*premain == 0)
;
	lda     _premain+1
	sta     ptr1+1
	lda     _premain
	sta     ptr1
	lda     (ptr1),y
	jne     L0194
;
; *pnm = *pval; // **pptr_voice; //pvoice[*pidx];
;
	lda     _pnm
	ldx     _pnm+1
	jsr     pushax
	lda     _pval+1
	sta     ptr1+1
	lda     _pval
	sta     ptr1
	ldy     #$01
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     staxspidx
;
; pval++; // (*pptr_voice)++;
;
	lda     _pval
	ldx     _pval+1
	clc
	adc     #$02
	bcc     L0199
	inx
L0199:	sta     _pval
	stx     _pval+1
;
; if (*pnm == 0)
;
	lda     _pnm+1
	sta     ptr1+1
	lda     _pnm
	sta     ptr1
	iny
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	cpx     #$00
	bne     L019A
	cmp     #$00
	bne     L019A
;
; pval--; // (*pptr_voice)--;
;
	lda     _pval
	ldx     _pval+1
	sec
	sbc     #$02
	bcs     L019E
	dex
L019E:	sta     _pval
	stx     _pval+1
;
; *pptr_voice = pval;
;
	lda     _pptr_voice+1
	sta     ptr1+1
	lda     _pptr_voice
	sta     ptr1
	lda     _pval
	sta     (ptr1),y
	iny
	lda     _pval+1
	sta     (ptr1),y
;
; continue;
;
	jmp     L0239
;
; prepare_waveform_control_registers();
;
L019A:	jsr     _prepare_waveform_control_registers
;
; __asm__ ( "jsr " FUNC_DECODE_NOTE );
;
	jsr     func_decode_note
;
; calculate_note_frequency();
;
	jsr     _calculate_note_frequency
;
; *pl = lf;
;
	lda     _pl+1
	sta     ptr1+1
	lda     _pl
	sta     ptr1
	lda     _lf
	ldy     #$00
	sta     (ptr1),y
;
; *ph = hf;
;
	lda     _ph+1
	sta     ptr1+1
	lda     _ph
	sta     ptr1
	lda     _hf
	sta     (ptr1),y
;
; *pc = *pwa;
;
	lda     _pc
	ldx     _pc+1
	jsr     pushax
	lda     _pwa+1
	sta     ptr1+1
	lda     _pwa
	sta     ptr1
	ldy     #$00
	lda     (ptr1),y
	jsr     staspidx
;
; *premain = dr;
;
	lda     _premain+1
	sta     ptr1+1
	lda     _premain
	sta     ptr1
	lda     _dr
	ldy     #$00
	sta     (ptr1),y
;
; else if (*premain == 1 && autogateflag[k]) // are we on the last 1/32nd of the present note? Time to gate off?
;
	jmp     L01C0
L0194:	lda     _premain+1
	sta     ptr1+1
	lda     _premain
	sta     ptr1
	lda     (ptr1),y
	cmp     #$01
	bne     L01C0
	lda     _k
	asl     a
	bcc     L01E9
	inx
	clc
L01E9:	adc     #<(_autogateflag)
	sta     ptr1
	txa
	adc     #>(_autogateflag)
	sta     ptr1+1
	lda     (ptr1),y
	iny
	ora     (ptr1),y
	beq     L01C0
;
; *pc = *pc & 0xFE; // assure gate-bit is turned off
;
	lda     _pc+1
	sta     sreg+1
	lda     _pc
	sta     sreg
	lda     _pc+1
	sta     ptr1+1
	lda     _pc
	sta     ptr1
	dey
	lda     (ptr1),y
	and     #$FE
	sta     (sreg),y
;
; (*premain)--;
;
L01C0:	lda     _premain
	ldx     _premain+1
	jsr     pushax
	sta     ptr1
	stx     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	sec
	sbc     #$01
	jsr     staspidx
;
; *pptr_voice = pval;
;
	lda     _pptr_voice+1
	sta     ptr1+1
	lda     _pptr_voice
	sta     ptr1
	lda     _pval
	ldy     #$00
	sta     (ptr1),y
	iny
	lda     _pval+1
	sta     (ptr1),y
;
; pidx++;
;
	lda     _pidx
	ldx     _pidx+1
	clc
	adc     #$02
	bcc     L01CC
	inx
L01CC:	sta     _pidx
	stx     _pidx+1
;
; pnm++;
;
L0239:	lda     _pnm
	ldx     _pnm+1
	clc
	adc     #$02
	bcc     L01CE
	inx
L01CE:	sta     _pnm
	stx     _pnm+1
;
; premain++;
;
	lda     _premain
	ldx     _premain+1
	clc
	adc     #$01
	bcc     L01D0
	inx
L01D0:	sta     _premain
	stx     _premain+1
;
; pl++;
;
	lda     _pl
	ldx     _pl+1
	clc
	adc     #$01
	bcc     L01D2
	inx
L01D2:	sta     _pl
	stx     _pl+1
;
; ph++;
;
	lda     _ph
	ldx     _ph+1
	clc
	adc     #$01
	bcc     L01D4
	inx
L01D4:	sta     _ph
	stx     _ph+1
;
; pc++;
;
	lda     _pc
	ldx     _pc+1
	clc
	adc     #$01
	bcc     L01D6
	inx
L01D6:	sta     _pc
	stx     _pc+1
;
; ppvoice++;
;
	lda     _ppvoice
	ldx     _ppvoice+1
	clc
	adc     #$02
	bcc     L01D8
	inx
L01D8:	sta     _ppvoice
	stx     _ppvoice+1
;
; pptr_voice++;
;
	lda     _pptr_voice
	ldx     _pptr_voice+1
	clc
	adc     #$02
	bcc     L01DA
	inx
L01DA:	sta     _pptr_voice
	stx     _pptr_voice+1
;
; pwa++;
;
	lda     _pwa
	ldx     _pwa+1
	clc
	adc     #$01
	bcc     L01DC
	inx
L01DC:	sta     _pwa
	stx     _pwa+1
;
; pwb++;
;
	lda     _pwb
	ldx     _pwb+1
	clc
	adc     #$01
	bcc     L01DE
	inx
L01DE:	sta     _pwb
	stx     _pwb+1
;
; for (k = 0; k <= 2; k++)
;
	ldx     #$00
	inc     _k
	jmp     L01F2
;
; return 1;
;
L01F4:	lda     #$01
;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ music_loop_iteration (void)
; ---------------------------------------------------------------

func_music_loop_iteration:
.proc	_music_loop_iteration: near

;
; music_t++;
;
	inc     _music_t
;
; if (music_t < delay_cnt)
;
	lda     _music_t
	cmp     _delay_cnt
;
; return;
;
	bcs     L0269
;
; }
;
	rts
;
; music_t = 0;
;
L0269:	lda     #$00
	sta     _music_t
;
; update_decoded_music();
;
	jsr     _update_decoded_music
;
; if (use_bug)
;
	lda     _use_bug
	beq     L01E7
;
; Poke(_SID_,    l[0]);  // (0xd400) v1 freq lo-byte
;
	lda     _l
	sta     $D400
;
; Poke(_SID_+7,  l[1]);  // (0xd407) v2 freq lo-byte
;
	lda     _l+1
	sta     $D407
;
; Poke(_SID_+14, l[2]);  // (0xd40e) v3 freq lo-byte
;
	lda     _l+2
	sta     $D40E
;
; Poke(_SID_+1,  h[0]);  // (0xd401) v1 freq hi-byte
;
	lda     _h
	sta     $D401
;
; Poke(_SID_+8,  h[1]);  // (0xd408) v2 freq hi-byte
;
	lda     _h+1
	sta     $D408
;
; Poke(_SID_+15, h[2]);  // (0xd40f) v3 freq hi-byte
;
	lda     _h+2
	sta     $D40F
;
; Poke(_SID_+4,  c[0]);  // (0xd404) v1 control register
;
	lda     _c
	sta     $D404
;
; Poke(_SID_+11, c[1]);  // (0xd40b) v2 control register
;
	lda     _c+1
	sta     $D40B
;
; else
;
	jmp     L0268
;
; if (c[0]) // only do for non-zero values
;
L01E7:	lda     _c
	beq     L0220
;
; Poke(_SID_,    l[0]);  // (0xd400) v1 freq lo-byte
;
	lda     _l
	sta     $D400
;
; Poke(_SID_+1,  h[0]);  // (0xd401) v1 freq hi-byte
;
	lda     _h
	sta     $D401
;
; Poke(_SID_+4,  c[0]);  // (0xd404) v1 control register
;
	lda     _c
	sta     $D404
;
; if (c[1])
;
L0220:	lda     _c+1
	beq     L0235
;
; Poke(_SID_+7,  l[1]);  // (0xd407) v2 freq lo-byte
;
	lda     _l+1
	sta     $D407
;
; Poke(_SID_+8,  h[1]);  // (0xd408) v2 freq hi-byte
;
	lda     _h+1
	sta     $D408
;
; Poke(_SID_+11, c[1]);  // (0xd40b) v2 control register
;
	lda     _c+1
	sta     $D40B
;
; if (c[2])
;
L0235:	lda     _c+2
	beq     L024A
;
; Poke(_SID_+14, l[2]);  // (0xd40e) v3 freq lo-byte
;
	lda     _l+2
	sta     $D40E
;
; Poke(_SID_+15, h[2]);  // (0xd40f) v3 freq hi-byte
;
	lda     _h+2
	sta     $D40F
;
; Poke(_SID_+18, c[2]);  // (0xd412) v3 control register
;
L0268:	lda     _c+2
	sta     $D412
;
; music_idx++; // increment to next 1/32nd measure
;
L024A:	lda     _music_idx
	ldx     _music_idx+1
	clc
	adc     #$01
	bcc     L0260
	inx
L0260:	sta     _music_idx
	stx     _music_idx+1
;
; bar_idx++;
;
	inc     _bar_idx
;
; if (bar_idx == bar_length)
;
	lda     _bar_length
	cmp     _bar_idx
	bne     L0262
;
; bar_idx = 0;
;
	lda     #$00
	sta     _bar_idx
;
; }
;
L0262:	rts

.endproc
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

  ldy #00

l1:
  sta (ptr1),y
  iny
  cpy #25
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

.proc	_draw_sprintf: near

;
; {
;
	jsr     enter
;
; va_start (args, str);
;
	jsr     decsp2
	ldy     #$02
	lda     (sp),y
	jsr     leaa0sp
	sec
	sbc     #$01
	bcs     L0A0F
	dex
L0A0F:	jsr     stax0sp
;
; vsprintf(dstr, str, args);
;
	jsr     decsp4
	lda     #<(_dstr)
	ldy     #$02
	sta     (sp),y
	iny
	lda     #>(_dstr)
	sta     (sp),y
	ldy     #$06
	lda     (sp),y
	jsr     leaa0sp
	clc
	adc     #$03
	bcc     L0A13
	inx
L0A13:	sta     ptr1
	stx     ptr1+1
	ldy     #$01
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	sta     (sp),y
	iny
	txa
	sta     (sp),y
	ldy     #$05
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	jsr     _vsprintf
;
; draw_text(dstr, posx, posy, 0);
;
	jsr     decsp4
	lda     #<(_dstr)
	ldy     #$02
	sta     (sp),y
	iny
	lda     #>(_dstr)
	sta     (sp),y
	ldy     #$06
	lda     (sp),y
	jsr     leaa0sp
	sta     ptr1
	stx     ptr1+1
	lda     (ptr1),y
	ldy     #$01
	sta     (sp),y
	ldy     #$06
	lda     (sp),y
	jsr     leaa0sp
	dey
	sta     ptr1
	stx     ptr1+1
	lda     (ptr1),y
	ldy     #$00
	sta     (sp),y
	tya
	jsr     _draw_text
;
; }
;
	ldy     #$02
	jmp     leavey

.endproc

; ---------------------------------------------------------------
; void __near__ draw_text (__near__ unsigned char *, unsigned char, unsigned char, unsigned char)
; ---------------------------------------------------------------

.proc	_draw_text: near

;
; {
;
	jsr     pusha
;
; screen_loc = (vicbase+0x2000) + posx*8 + posy*40*8;
;
	lda     _vicbase
	sta     ptr1
	lda     _vicbase+1
	clc
	adc     #$20
	sta     ptr1+1
	ldx     #$00
	ldy     #$02
	lda     (sp),y
	jsr     shlax3
	clc
	adc     ptr1
	pha
	txa
	adc     ptr1+1
	tax
	pla
	jsr     pushax
	ldy     #$03
	lda     (sp),y
	jsr     pusha0
	lda     #$28
	jsr     tosumula0
	jsr     shlax3
	jsr     tosaddax
	sta     _screen_loc
	stx     _screen_loc+1
;
; a = 0;
;
	lda     #$00
	sta     _a
;
; while (str[a] != 0)
;
	jmp     L0A11
;
; c64loc = 0x5c00 + ( (str[a] & 0x3f) << 3); // base of my character data
;
L0A0F:	ldy     #$04
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	ldy     _a
	sta     ptr1
	stx     ptr1+1
	lda     (ptr1),y
	ldx     #$00
	and     #$3F
	jsr     shlax3
	sta     _c64loc
	txa
	clc
	adc     #$5C
	sta     _c64loc+1
;
; if (invert)
;
	ldy     #$00
	lda     (sp),y
	jeq     L0A19
;
; INV_CHAR_LINE
;
	lda     _screen_loc
	sta     sreg
	lda     _screen_loc+1
	sta     sreg+1
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	lda     (ptr1),y
	eor     #$FF
	sta     (sreg),y
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0A23
	inx
L0A23:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0A25
	inx
L0A25:	sta     _c64loc
	stx     _c64loc+1
;
; INV_CHAR_LINE
;
	lda     _screen_loc
	sta     sreg
	lda     _screen_loc+1
	sta     sreg+1
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	lda     (ptr1),y
	eor     #$FF
	sta     (sreg),y
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0A2E
	inx
L0A2E:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0A30
	inx
L0A30:	sta     _c64loc
	stx     _c64loc+1
;
; INV_CHAR_LINE
;
	lda     _screen_loc
	sta     sreg
	lda     _screen_loc+1
	sta     sreg+1
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	lda     (ptr1),y
	eor     #$FF
	sta     (sreg),y
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0A39
	inx
L0A39:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0A3B
	inx
L0A3B:	sta     _c64loc
	stx     _c64loc+1
;
; INV_CHAR_LINE
;
	lda     _screen_loc
	sta     sreg
	lda     _screen_loc+1
	sta     sreg+1
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	lda     (ptr1),y
	eor     #$FF
	sta     (sreg),y
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0A44
	inx
L0A44:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0A46
	inx
L0A46:	sta     _c64loc
	stx     _c64loc+1
;
; INV_CHAR_LINE
;
	lda     _screen_loc
	sta     sreg
	lda     _screen_loc+1
	sta     sreg+1
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	lda     (ptr1),y
	eor     #$FF
	sta     (sreg),y
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0A4F
	inx
L0A4F:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0A51
	inx
L0A51:	sta     _c64loc
	stx     _c64loc+1
;
; INV_CHAR_LINE
;
	lda     _screen_loc
	sta     sreg
	lda     _screen_loc+1
	sta     sreg+1
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	lda     (ptr1),y
	eor     #$FF
	sta     (sreg),y
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0A5A
	inx
L0A5A:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0A5C
	inx
L0A5C:	sta     _c64loc
	stx     _c64loc+1
;
; INV_CHAR_LINE
;
	lda     _screen_loc
	sta     sreg
	lda     _screen_loc+1
	sta     sreg+1
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	lda     (ptr1),y
	eor     #$FF
	sta     (sreg),y
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0A65
	inx
L0A65:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0A67
	inx
L0A67:	sta     _c64loc
	stx     _c64loc+1
;
; INV_CHAR_LINE
;
	lda     _screen_loc
	sta     sreg
	lda     _screen_loc+1
	sta     sreg+1
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	lda     (ptr1),y
	eor     #$FF
	sta     (sreg),y
;
; else
;
	jmp     L0E81
;
; CHAR_LINE
;
L0A19:	lda     _screen_loc
	ldx     _screen_loc+1
	jsr     pushax
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	jsr     staspidx
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0A7C
	inx
L0A7C:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0A7E
	inx
L0A7E:	sta     _c64loc
	stx     _c64loc+1
;
; CHAR_LINE
;
	lda     _screen_loc
	ldx     _screen_loc+1
	jsr     pushax
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	jsr     staspidx
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0A87
	inx
L0A87:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0A89
	inx
L0A89:	sta     _c64loc
	stx     _c64loc+1
;
; CHAR_LINE
;
	lda     _screen_loc
	ldx     _screen_loc+1
	jsr     pushax
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	jsr     staspidx
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0A92
	inx
L0A92:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0A94
	inx
L0A94:	sta     _c64loc
	stx     _c64loc+1
;
; CHAR_LINE
;
	lda     _screen_loc
	ldx     _screen_loc+1
	jsr     pushax
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	jsr     staspidx
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0A9D
	inx
L0A9D:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0A9F
	inx
L0A9F:	sta     _c64loc
	stx     _c64loc+1
;
; CHAR_LINE
;
	lda     _screen_loc
	ldx     _screen_loc+1
	jsr     pushax
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	jsr     staspidx
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0AA8
	inx
L0AA8:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0AAA
	inx
L0AAA:	sta     _c64loc
	stx     _c64loc+1
;
; CHAR_LINE
;
	lda     _screen_loc
	ldx     _screen_loc+1
	jsr     pushax
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	jsr     staspidx
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0AB3
	inx
L0AB3:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0AB5
	inx
L0AB5:	sta     _c64loc
	stx     _c64loc+1
;
; CHAR_LINE
;
	lda     _screen_loc
	ldx     _screen_loc+1
	jsr     pushax
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	jsr     staspidx
	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0ABE
	inx
L0ABE:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0AC0
	inx
L0AC0:	sta     _c64loc
	stx     _c64loc+1
;
; CHAR_LINE
;
	lda     _screen_loc
	ldx     _screen_loc+1
	jsr     pushax
	lda     _c64loc
	sta     ptr1
	lda     _c64loc+1
	sta     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	jsr     staspidx
L0E81:	lda     _screen_loc
	ldx     _screen_loc+1
	clc
	adc     #$01
	bcc     L0AC9
	inx
L0AC9:	sta     _screen_loc
	stx     _screen_loc+1
	lda     _c64loc
	ldx     _c64loc+1
	clc
	adc     #$01
	bcc     L0ACB
	inx
L0ACB:	sta     _c64loc
	stx     _c64loc+1
;
; a++;
;
	inc     _a
;
; while (str[a] != 0)
;
L0A11:	ldy     #$04
	lda     (sp),y
	tax
	dey
	lda     (sp),y
	ldy     _a
	sta     ptr1
	stx     ptr1+1
	lda     (ptr1),y
	jne     L0A0F
;
; }
;
	jmp     incsp5

.endproc
