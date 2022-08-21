	include "gtereg.h"
	include "inline_a.h"

MESHZ	equ	0


	section .text
	opt	at-


push	macro
.spoffs	set	0
	subu	sp,sp,narg*4
	rept	narg
	sw	\1,.spoffs(sp)
	shift
.spoffs	set	.spoffs+4
	endr
	endm

pop	macro
.spoffs	set	0
	rept	narg
	lw	\1,.spoffs(sp)
	shift
.spoffs	set	.spoffs+4
	endr
	addu	sp,sp,.spoffs
	endm

func	macro
	function	"\1",\1,\1_end-\1
	xdef	\1
\1:
	endm

endfunc	macro
	jr	ra
	nop
\1_end
	endm






;; SVECTOR *TransMultPoints(SVECTOR *from,SVECTOR*to,int count)



;======================================================================
;Function Name: TransMultPoints
;Entry Params : SVECTOR *src, SVECTOR *dst, int count
;Exit Params  : SVECTOR *endoftransformedpoints
;Description  :
;
;
;
;
;======================================================================
	FUNC	TransMultPoints

	opt	at+
	addi	t0,zero,5
	ble	a2,t0,.Singlecheck
	opt	at-

;MEM>>CPU
	lw	t0,0(a0)
	lhu	t1,4(a0)
	lw	t2,8(a0)
	lhu	t3,12(a0)
	lw	t4,16(a0)
	lhu	t5,20(a0)
;CPU>>GTEin
	mtc2	t0,C2_VXY0
	mtc2	t1,C2_VZ0
	mtc2	t2,C2_VXY1
	mtc2	t3,C2_VZ1
	mtc2	t4,C2_VXY2
	mtc2	t5,C2_VZ2
	addiu	a0,a0,24	; Update a0 to point to next set of 3 verts
	addi	a2,a2,-3	; Decrement the count by 3
	RTPT

Loop1:
;MEMin>>CPU
	lw	t0,0(a0)	; Read 3 verts into t0-t5
	lhu	t1,4(a0)
	lw	t2,8(a0)
	lhu	t3,12(a0)
	lw	t4,16(a0)
	lhu	t5,20(a0)
	addiu	a0,a0,24	; Update a0 to point to next set of 3 verts
;CPU>>GTEin
	mfc2	t6,C2_SXY0	; Pull previous answers
	mtc2	t0,C2_VXY0
	mtc2	t1,C2_VZ0
	mtc2	t2,C2_VXY1
	mtc2	t3,C2_VZ1
	mtc2	t4,C2_VXY2
	mtc2	t5,C2_VZ2
;GTEout>>CPU
	mfc2	t1,C2_SZ1
	mfc2	t2,C2_SXY1
	mfc2	t3,C2_SZ2
	mfc2	t4,C2_SXY2
	mfc2	t5,C2_SZ3

;CPU>>MEMout
;	sra	t1,t1,2
;	sra	t3,t3,2
;	sra	t5,t5,2
;	nop
;	nop

	RTPT

	sw	t6,0(a1)	; Write previous result into output structure
	sh	t1,4(a1)
	sw	t2,8(a1)
	sh	t3,12(a1)
	sw	t4,16(a1)
	sh	t5,20(a1)
	addi    a2,a2,-3
	bgtz	a2,Loop1
	addiu	a1,a1,24 	; Update a1 to point to next set of 3 verts
;Take last set of three from GTE and store in memory

;GTEout>>CPU
	mfc2	t0,C2_SXY0	; Pull previous answers
	mfc2	t1,C2_SZ1
	mfc2	t2,C2_SXY1
	mfc2	t3,C2_SZ2
	mfc2	t4,C2_SXY2
	mfc2	t5,C2_SZ3

;CPU>>MEMout
;	sra	t1,t1,2
;	sra	t3,t3,2
;	sra	t5,t5,2
	sw	t0,0(a1)	; Write previous result into output structure
	sh	t1,4(a1)
	sw	t2,8(a1)
	sh	t3,12(a1)
	sw	t4,16(a1)
	sh	t5,20(a1)
	addiu	a1,a1,24 	; Update a1 to point to next set of 3 verts

.Singlecheck
	beq	a2,zero,DoneAll
	nop
	addiu	a2,a2,3
Singles
	lw  t0,0(a0)
	lw  t1,4(a0)
	mtc2    t0,C2_VXY0
	mtc2    t1,C2_VZ0
	addiu   a0,a0,8
	nop
	RTPS
	mfc2    t0,C2_SXY2
	mfc2    t1,C2_SZ3
;	sra     t1,t1,2
	sw      t0,0(a1)
	sh      t1,4(a1)
	addi    a2,a2,-1
	addiu   a1,a1,8
	bne     a2,zero,Singles
	nop
DoneAll
	addu   v0,zero,a1       ; Return the pointer to the first free transformed point slot.
	ENDFUNC	TransMultPoints



;======================================================================
;Function Name: TransMultNormal_Light
;Entry Params : SVECTOR *src,SVECTOR *dst,int count
;Exit Params  : SVECTOR *endoftransformednormals
;Description  :
;
;
;
;
;======================================================================
	FUNC	TransMultNormal_Light








;MEM>>CPU
	opt	at+
	addi	t0,zero,5
	ble	a2,t0,.Singlecheck2
	opt	at-

	lui	t6,0x3400      ; T6 = CODE for a PolyGt3
	lui	t7,0x00ff
	ori	t7,t7,0xffff   ; T7 = Mask for coords
	lw	t0,0(a0)
	lw	t1,4(a0)
	lw	t2,8(a0)
	lw	t3,12(a0)
	lw	t4,16(a0)
	lw	t5,20(a0)
;CPU>>GTEin
	mtc2	t0,C2_VXY0
	mtc2	t1,C2_VZ0
	mtc2	t2,C2_VXY1
	mtc2	t3,C2_VZ1
	mtc2	t4,C2_VXY2
	mtc2	t5,C2_VZ2
	addiu	a0,a0,24	; Update a0 to point to next set of 3 verts
	addi	a2,a2,-3	; Decrement the count by 3
	nNCDT

.Loop1
;MEMin>>CPU
	lw	t0,0(a0)	; Read 3 verts into t0-t5
	lw	t1,4(a0)
	lw	t2,8(a0)
	lw	t3,12(a0)
	lw	t4,16(a0)
	lw	t5,20(a0)
	addiu	a0,a0,24	; Update a0 to point to next set of 3 verts
;CPU>>GTEin
	mfc2	zero,C2_VXY0	; Dummy read to force the GTE to finish doing the NCT
	mtc2	t0,C2_VXY0	; Load 3 verts to GTE
	mtc2	t1,C2_VZ0
	mtc2	t2,C2_VXY1
	mtc2	t3,C2_VZ1
	mtc2	t4,C2_VXY2
	mtc2	t5,C2_VZ2
;GTEout>>CPU
	mfc2	t0,C2_RGB0	; Pull previous answers
	mfc2	t1,C2_RGB1	; Pull previous answers
	mfc2	t2,C2_RGB2	; Pull previous answers

;CPU>>MEMout

	nNCDT

	and	t0,t0,t7
	and	t1,t1,t7
	and	t2,t2,t7
	or	t0,t0,t6
	sw	t0,0(a1)	; Write previous result into output structure
	or	t1,t1,t6
	sw	t1,4(a1)
	or	t2,t2,t6
	sw	t2,8(a1)
	addiu	a1,a1,12 	; Update a1 to point to next set of 3 verts
	addi    a2,a2,-3
	bgtz	a2,.Loop1
	nop
;Take last set of three from GTE and store in memory

;GTEout>>CPU
	mfc2	t0,C2_RGB0	; Pull previous answers
	mfc2	t1,C2_RGB1
	mfc2	t2,C2_RGB2

;CPU>>MEMout
	and	t0,t0,t7
	and	t1,t1,t7
	and	t2,t2,t7
	or	t0,t0,t6
	or	t1,t1,t6
	sw	t0,0(a1)	; Write previous result into output structure
	or	t2,t2,t6
	sw	t1,4(a1)
	sw	t2,8(a1)
	addiu	a1,a1,12 	; Update a1 to point to next set of 3 verts

.Singlecheck2
	beq	a2,zero,.DoneAll
	nop
	addiu	a2,a2,3
.Singles
	lw  t0,0(a0)
	lw  t1,4(a0)
	mtc2    t0,C2_VXY0
	mtc2    t1,C2_VZ0
	addiu   a0,a0,8
	nop
	nNCDS
	mfc2    t1,C2_RGB2
	and	t0,t0,t7
	or	t0,t0,t6
	sw      t1,0(a1)
	addi    a2,a2,-1
	addiu   a1,a1,4
	bne     a2,zero,.Singles
	nop
.DoneAll
	addu   v0,zero,a1       ; Return the pointer to the first free transformed point slot.



	ENDFUNC	TransMultNormal_Light

gt3tag	equ	0
gt3r0	equ	4
gt3x0	equ	8
gt3u0	equ	12
gt3r1	equ	16
gt3x1	equ	20
gt3u1	equ	24
gt3r2	equ	28
gt3x2	equ	32
gt3u2	equ	36
gt3size	equ	40


ft3tag	equ	0
ft3r0	equ	4
ft3x0	equ	8
ft3u0	equ	12
ft3x1	equ	16
ft3u1	equ	20
ft3x2	equ	24
ft3u2	equ	28
ft3size	equ	32



mVert0	equ	0
mVert1	equ	2
mVert2	equ	4
mNorm0	equ	6
mNorm1	equ	8
mNorm2	equ	10
mGTriSize	equ	12



mUV0	equ	0
mUV1	equ	4
mUV2	equ	8
mTTrisize	equ	12

;======================================================================
;Function Name: MakePolysFromList
;Entry Params : POLY_GT3 *p3,TRI *tri,SVECTOR*vc,long *vn,int count, unsigned long *otptr,
;Exit Params  : Returns updated p3 pointer
;Description  :
;
;
;
;
;======================================================================
OffScreenMask	dw	0
GouraudDepth	dw	0
	xdef	OffScreenMask
	xdef	GouraudDepth

	opt	at+

	FUNC	MakePolysFromList
	lui	s4,0x007f
	addi	s4,s4,0x7f7f   ; S4 is the colour mask for underwater vertices.



	add	v0,zero,a0     ; Setup return value in V0
	lw	v1,GouraudDepth
	lw	t7,OffScreenMask
	lw	t5,16(sp)      ; T5 is triangle count.
	sw	s1,16(sp)      ; Store S1 on Stack for later
	lw	s1,20(sp)      ; S1 is OT pointer.
	sw	s2,20(sp)      ; Store S2 on stack for later
	lw	t0,24(sp)
	push	s4,s5,s6,s7,s8

	add	s8,zero,t0

	lui	s2,0x0900      ; Length of a GT3 (for later...)
.loop
	lh	t0,0(a1)       ; Get first vertex number (shifted already)
	lh	t1,2(a1)       ; Get Second vertex number (shifted already)
	lh	t2,4(a1)       ; Get third vertex number (shifted already)


	sll	t0,t0,3
	sll	t1,t1,3
	sll	t2,t2,3

	addu	t0,t0,a2       ; Add base address for vertex info
	addu	t1,t1,a2       ; Add base address for vertex info
	lw	s5,4(t0)       ; Get Z coord of first vertex
	addu	t2,t2,a2       ; Add base address for vertex info
	mtc2	s5,C2_SZ1      ; Setup SZ1 for AVSZ3
	lw	s6,4(t1)       ; Get Z coord of second vertex
	lw	s7,4(t2)       ; Get Z coord of third vertex
	mtc2	s6,C2_SZ2      ; Setup SZ2 for AVSZ3
	mtc2	s7,C2_SZ3      ; Setup SZ3 for AVSZ3


	lw	t0,0(t0)       ; Load XY coord of first vertex
	lw	t1,0(t1)       ; Load XY coord of second vertex
	AVSZ3                   ; Trigger off the Z averaging...
	mtc2	t0,C2_SXY0     ; Put first coord into GTE
	lw	t2,0(t2)       ; Load XY coord of third vertex
	mtc2	t1,C2_SXY1     ; Put second coord into GTE
	mtc2	t2,C2_SXY2     ; Put third coord into GTE

;-Do offscreen check here!!!
	and	at,t0,t1
	and	at,at,t2       ; Now have offscreen flags...
	and	at,at,t7       ; Mask off flags
	bne	at,zero,.skip  ; Offscreen!


	mfc2	t4,C2_OTZ      ; Average OT Z pos should be ready now...

	nop
	andi	t4,t4,0xfffc   ; Mask off the bottom two bits of the OT pos (div 4,mul 4)
	if MESHZ
	addi	t4,t4,MESHZ    ; Add the Z offset for meshes
	endif
	NCLIP                   ; Start GTE doing an NCLIP test...
	bgeu	t4,850<<2,.skip  ; Off end of OTZ range

	blt	t4,v1,.makegt3

		               ; distant TRI, so just make flat textured...
.makeft3



	sw	t0,ft3x0(v0)   ; Put first coord to RAM
	sw	t1,ft3x1(v0)   ; Put second coord to RAM

	mfc2	t3,C2_MAC0     ; Get result of NCLIP function
	sll	t4,t4,1        ; *EXTRA SHIFT DUE TO NTAGS BEING 8 BYTES*
	blez	t3,.skip       ; If back face, don't go any further...
	lh	t0,mNorm0(a1)  ; Get first Colour number (shifted already)
	sw	t2,ft3x2(v0)   ; Put third coord to RAM (use delay slot)
	add	t0,t0,a3       ; Add base address of Normal colours

	lw	t0,0(t0)       ; Pull first vertex colour


	add	t4,t4,s1       ; T4 points to OT entry to put this triangle into



	sll	t0,t0,8;
	srl	t0,t0,8;

	lui	t6,0x2400      ; Code for FT3
	or	t0,t0,t6


	lw         t3,0(t4)

	sw	t0,ft3r0(v0)   ; Store first colour to RAM
	lw	t0,mUV0(s8)    ; Get first UV coord
	lw	t1,mUV1(s8)    ; Get second UV coord
	lw	t2,mUV2(s8)    ; Get third UV coord
	sw	t0,ft3u0(v0)   ; Store first UV coord
	sw	t1,ft3u1(v0)   ; Store second UV coord
	sw	t2,ft3u2(v0)   ; Store third UV coord

	lui	t2,0x0700      ; Length of a FT3 (for later...)

	beq      t3,zero,.flab1
	sw       v0,0(t4)

	or       t2,t3,t2

	b        .flab2
.flab1	sw	t2,0(v0)       ; Set prim type
	sw       v0,4(t4)
.flab2
	b	.skip
	addiu	v0,v0,ft3size  ; point v0 to another free f3 slot.


.makegt3


	sw	t0,gt3x0(v0)   ; Put first coord to RAM
	sw	t1,gt3x1(v0)   ; Put second coord to RAM
	sw	t2,gt3x2(v0)   ; Put third coord to RAM (use delay slot)

	mfc2	t3,C2_MAC0     ; Get result of NCLIP function
	sll	t4,t4,1        ; *EXTRA SHIFT DUE TO NTAGS BEING 8 BYTES*
	blez	t3,.skip       ; If back face, don't go any further...
	lh	t0,mNorm0(a1)  ; Get first Colour number (shifted already)
	lh	t1,mNorm1(a1)  ; Get Second colour number (shifted already)
	lh	t2,mNorm2(a1)  ; Get third normal number (shifted already)
	add	t0,t0,a3       ; Add base address of Normal colours
	add	t1,t1,a3       ; Add base address of Normal colours
	add	t2,t2,a3       ; Add base address of Normal colours
	lw	t0,0(t0)       ; Pull first vertex colour


	add	t4,t4,s1       ; T4 points to OT entry to put this triangle into



	lw	t1,0(t1)       ; Pull second vertex colour
	lw	t2,0(t2)       ; Pull third vertex colour (Delay slot....)

.nowater3
	lui	t6,0x3400      ; Code to OR with the masked RGB value...
	or	t0,t0,t6       ; OR on the code for GT3 (0x34)
	lw       t3,0(t4)
	sw	t1,gt3r1(v0)   ; Store second colour to RAM
	sw	t0,gt3r0(v0)   ; Store first colour to RAM
	sw	t2,gt3r2(v0)   ; Store third colour to RAM

	if 1
	lw	t0,mUV0(s8)    ; Get first UV coord
	lw	t1,mUV1(s8)    ; Get second UV coord
	lw	t2,mUV2(s8)    ; Get third UV coord
	sw	t0,gt3u0(v0)   ; Store first UV coord
	sw	t1,gt3u1(v0)   ; Store second UV coord
	sw	t2,gt3u2(v0)   ; Store third UV coord
	else
		               ; A0 will hold the CLUT variable (high word)
		               ; 31 - - - | - - - - 0
	lw	t2,0(s8)       ; Get UV0  |  UV2 pair
	lw	t1,4(s8)       ; Get TPAGE|  UV1 pair
	srl	t0,t2,16       ; T0 is UV0
	or	t0,t0,a0       ; OR with clut value
	sw	t2,gt3u2(v0)   ; Store UV2 pair (top part ignored)
	sw	t0,gt3u0(v0)   ; Store UV0 pair | Clut
	sw	t1,gt3u1(v0)   ; Store UV1 pair | TPAGE
	endif
; Now add the prim into the OT list...

	lui	t2,0x0900      ; Length of a GT3 (for later...)
	beq      t3,zero,.lab1
	sw       v0,0(t4)
	or       t2,t3,t2
	b	.lab2
.lab1	sw	t2,0(v0)       ; Set prim type
	sw       v0,4(t4)
.lab2

; NEW END  (NTAG)



	addiu	v0,v0,gt3size  ; point v0 to another free p3 slot.

.skip
	addi	t5,t5,-1       ; Decrement triangle count
	addiu	a1,a1,mGTriSize ; point to next triangle in list
	bne	t5,zero,.loop
	if	1
	addiu	s8,s8,mTTrisize
	else

	endif
	pop	s4,s5,s6,s7,s8
	lw	s1,16(sp)      ; Restore s1
	lw	s2,20(sp)      ; Restore s2
	ENDFUNC	MakePolysFromList














