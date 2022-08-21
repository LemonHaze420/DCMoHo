
; CompactOT.s		Jeppe Oland (JeppeO@stormfront.com)
;	8-nov-98	v1.0	First implementation.
;	12-nov-98	v1.1	Added Div8 functions. 17-21% faster but with almost twice the code size so the cache penalty could be worse.

	                        SECTION .text

	     if	0
;-----------------------------------------------------------------------------------------------------------------------------------------------------
;	void CompactDiv8OTag(unsigned long int *pOT, int iNumEntries);
				GLOBAL  CompactDiv8OTag
	module
CompactDiv8OTag:		andi	a1,$FFFFFFF8							; Make sure we only deal with DIV8 lengths
				beq	a1,zero,.Used_Done						; If the count was less than 8 return
				lw	t0,(a0)								; Preload ot registers
				lw	t1,4(a0)							;
				lw	t2,8(a0)							;
				lw	t3,12(a0)							;
				lw	t4,16(a0)							;
				lw	t5,20(a0)							;
				lw	t6,24(a0)							;
				li	a2,$00FFFFFF							; Fix OT address to low memory region
				and	a0,a2								;
				sll	a1,2								; a1 is the end-address of the OT
				addu	a1,a0								;
				move	a2,a0								; Initialize last used slot (if all empty)

.Unused_Test0_EndCheck:		beq	a0,a1,.Unused_Done						; End when we reach the unrolled endpointer
				addiu	a0,4								; Increase ot pointer
.Unused_Test0_Loop:		beq	t0,a0,.Unused_Test1						; Still unused - no state change
				lw	t7,24(a0)							; Preload "next" ot
				sw	t0,(a2)								; Store address of this in first empty slot
.Used_Test1:			addiu	a0,4								; Increase ot pointer
				bne	t1,a0,.Used_Test2						; Still used - no state change
				lw	t0,24(a0)							; Preload "next" ot
				addiu	a2,a0,-4							; Calculate adress of first unused ot entry
.Unused_Test2:			addiu	a0,4								; Increase ot pointer
				beq	t2,a0,.Unused_Test3						; Still unused - no state change
				lw	t1,24(a0)							; Preload "next" ot
				sw	t2,(a2)								; Store address of this in first empty slot
.Used_Test3:			addiu	a0,4								; Increase ot pointer
				bne	t3,a0,.Used_Test4						; Still used - no state change
				lw	t2,24(a0)							; Preload "next" ot
				addiu	a2,a0,-4							; Calculate adress of first unused ot entry
.Unused_Test4:			addiu	a0,4								; Increase ot pointer
				beq	t4,a0,.Unused_Test5						; Still unused - no state change
				lw	t3,24(a0)							; Preload "next" ot
				sw	t4,(a2)								; Store address of this in first empty slot
.Used_Test5:			addiu	a0,4								; Increase ot pointer
				bne	t5,a0,.Used_Test6						; Still used - no state change
				lw	t4,24(a0)							; Preload "next" ot
				addiu	a2,a0,-4							; Calculate adress of first unused ot entry
.Unused_Test6:			addiu	a0,4								; Increase ot pointer
				beq	t6,a0,.Unused_Test7						; Still unused - no state change
				lw	t5,24(a0)							; Preload "next" ot
				sw	t6,(a2)								; Store address of this in first empty slot
.Used_Test7:			addiu	a0,4								; Increase ot pointer
				bne	t7,a0,.Used_Test0_EndCheck					; Still used - no state change
				lw	t6,24(a0)							; Preload "next" ot
				addiu	a2,a0,-4							; Calculate adress of first unused ot entry

				bne	a0,a1,.Unused_Test0_Loop					; Loop if not at unrolled end
				addiu	a0,4								; Increase ot pointer

.Unused_Done:			jr	ra								; Return, and remember to store the last empty
				sw	t0,(a2)								; slot pointer (only if list was longer)

.Used_Test0_EndCheck:		beq	a0,a1,.Used_Done						; End when we reach the unrolled endpointer
				addiu	a0,4								; Increase ot pointer
.Used_Test0_Loop:		bne	t0,a0,.Used_Test1						; Still used - no state change
				lw	t7,24(a0)							; Preload "next" ot
				addiu	a2,a0,-4							; Calculate adress of first unused ot entry
.Unused_Test1:			addiu	a0,4								; Increase ot pointer
				beq	t1,a0,.Unused_Test2						; Still unused - no state change
				lw	t0,24(a0)							; Preload "next" ot
				sw	t1,(a2)								; Store address of this in first empty slot
.Used_Test2:			addiu	a0,4								; Increase ot pointer
				bne	t2,a0,.Used_Test3						; Still used - no state change
				lw	t1,24(a0)							; Preload "next" ot
				addiu	a2,a0,-4							; Calculate adress of first unused ot entry
.Unused_Test3:			addiu	a0,4								; Increase ot pointer
				beq	t3,a0,.Unused_Test4						; Still unused - no state change
				lw	t2,24(a0)							; Preload "next" ot
				sw	t3,(a2)								; Store address of this in first empty slot
.Used_Test4:			addiu	a0,4								; Increase ot pointer
				bne	t4,a0,.Used_Test5						; Still used - no state change
				lw	t3,24(a0)							; Preload "next" ot
				addiu	a2,a0,-4							; Calculate adress of first unused ot entry
.Unused_Test5:			addiu	a0,4								; Increase ot pointer
				beq	t5,a0,.Unused_Test6						; Still unused - no state change
				lw	t4,24(a0)							; Preload "next" ot
				sw	t5,(a2)								; Store address of this in first empty slot
.Used_Test6:			addiu	a0,4								; Increase ot pointer
				bne	t6,a0,.Used_Test7						; Still used - no state change
				lw	t5,24(a0)							; Preload "next" ot
				addiu	a2,a0,-4							; Calculate adress of first unused ot entry
.Unused_Test7:			addiu	a0,4								; Increase ot pointer
				beq	t7,a0,.Unused_Test0_EndCheck					; Still unused - no state change
				lw	t6,24(a0)							; Preload "next" ot
				sw	t7,(a2)								; Store address of this in first empty slot

				bne	a0,a1,.Used_Test0_Loop						; Loop if not at unrolled end
				addiu	a0,4								; Increase ot pointer

.Used_Done:			jr	ra								; Otherwise we're done! We don't have to do
				nop									; anything special as last used fixed it!!!
	modend
	endif
;-----------------------------------------------------------------------------------------------------------------------------------------------------
;	void CompactDiv8OTagR(unsigned long int *pOT, int iNumEntries);
				GLOBAL  CompactDiv8OTagR

CompactDiv8OTagR:		andi	a1,$FFFFFFF8							; Make sure we only deal with DIV8 lengths
				beq	a1,zero,.Used_Done						; If the count was less than 8 return
				sll	a1,2								; a0 points after table
				addu	a0,a1								;
				lw	t0,-4(a0)							; Preload ot registers
				lw	t1,-8(a0)							;
				lw	t2,-12(a0)							;
				lw	t3,-16(a0)							;
				lw	t4,-20(a0)							;
				lw	t5,-24(a0)							;
				lw	t6,-28(a0)							;
				li	a2,$00FFFFFF							; Fix OT address to low memory region
				and	a0,a2								;
				subiu	a0,4								; a0 points to last OT entry
				subu	a1,a0,a1							; a1 is the end-address of the OT
				move	a2,a0								; Initialize last used slot (if all empty)

.Unused_Test0_EndCheck:		beq	a0,a1,.Unused_Done						; End when we reach the unrolled endpointer
				addiu	a0,-4								; Increase ot pointer
.Unused_Test0_Loop:		beq	t0,a0,.Unused_Test1						; Still unused - no state change
				lw	t7,-24(a0)							; Preload "next" ot
				sw	t0,(a2)								; Store address of this in first empty slot
.Used_Test1:			addiu	a0,-4								; Increase ot pointer
				bne	t1,a0,.Used_Test2						; Still used - no state change
				lw	t0,-24(a0)							; Preload "next" ot
				addiu	a2,a0,4								; Calculate adress of first unused ot entry
.Unused_Test2:			addiu	a0,-4								; Increase ot pointer
				beq	t2,a0,.Unused_Test3						; Still unused - no state change
				lw	t1,-24(a0)							; Preload "next" ot
				sw	t2,(a2)								; Store address of this in first empty slot
.Used_Test3:			addiu	a0,-4								; Increase ot pointer
				bne	t3,a0,.Used_Test4						; Still used - no state change
				lw	t2,-24(a0)							; Preload "next" ot
				addiu	a2,a0,4								; Calculate adress of first unused ot entry
.Unused_Test4:			addiu	a0,-4								; Increase ot pointer
				beq	t4,a0,.Unused_Test5						; Still unused - no state change
				lw	t3,-24(a0)							; Preload "next" ot
				sw	t4,(a2)								; Store address of this in first empty slot
.Used_Test5:			addiu	a0,-4								; Increase ot pointer
				bne	t5,a0,.Used_Test6						; Still used - no state change
				lw	t4,-24(a0)							; Preload "next" ot
				addiu	a2,a0,4								; Calculate adress of first unused ot entry
.Unused_Test6:			addiu	a0,-4								; Increase ot pointer
				beq	t6,a0,.Unused_Test7						; Still unused - no state change
				lw	t5,-24(a0)							; Preload "next" ot
				sw	t6,(a2)								; Store address of this in first empty slot
.Used_Test7:			addiu	a0,-4								; Increase ot pointer
				bne	t7,a0,.Used_Test0_EndCheck					; Still used - no state change
				lw	t6,-24(a0)							; Preload "next" ot
				addiu	a2,a0,4								; Calculate adress of first unused ot entry

				bne	a0,a1,.Unused_Test0_Loop					; Loop if not at unrolled end
				addiu	a0,-4								; Increase ot pointer

.Unused_Done:			jr	ra								; Return, and remember to store the last empty
				sw	t0,(a2)								; slot pointer (only if list was longer)

.Used_Test0_EndCheck:		beq	a0,a1,.Used_Done						; End when we reach the unrolled endpointer
				addiu	a0,-4								; Increase ot pointer
.Used_Test0_Loop:		bne	t0,a0,.Used_Test1						; Still used - no state change
				lw	t7,-24(a0)							; Preload "next" ot
				addiu	a2,a0,4								; Calculate adress of first unused ot entry
.Unused_Test1:			addiu	a0,-4								; Increase ot pointer
				beq	t1,a0,.Unused_Test2						; Still unused - no state change
				lw	t0,-24(a0)							; Preload "next" ot
				sw	t1,(a2)								; Store address of this in first empty slot
.Used_Test2:			addiu	a0,-4								; Increase ot pointer
				bne	t2,a0,.Used_Test3						; Still used - no state change
				lw	t1,-24(a0)							; Preload "next" ot
				addiu	a2,a0,4								; Calculate adress of first unused ot entry
.Unused_Test3:			addiu	a0,-4								; Increase ot pointer
				beq	t3,a0,.Unused_Test4						; Still unused - no state change
				lw	t2,-24(a0)							; Preload "next" ot
				sw	t3,(a2)								; Store address of this in first empty slot
.Used_Test4:			addiu	a0,-4								; Increase ot pointer
				bne	t4,a0,.Used_Test5						; Still used - no state change
				lw	t3,-24(a0)							; Preload "next" ot
				addiu	a2,a0,4								; Calculate adress of first unused ot entry
.Unused_Test5:			addiu	a0,-4								; Increase ot pointer
				beq	t5,a0,.Unused_Test6						; Still unused - no state change
				lw	t4,-24(a0)							; Preload "next" ot
				sw	t5,(a2)								; Store address of this in first empty slot
.Used_Test6:			addiu	a0,-4								; Increase ot pointer
				bne	t6,a0,.Used_Test7						; Still used - no state change
				lw	t5,-24(a0)							; Preload "next" ot
				addiu	a2,a0,4								; Calculate adress of first unused ot entry
.Unused_Test7:			addiu	a0,-4								; Increase ot pointer
				beq	t7,a0,.Unused_Test0_EndCheck					; Still unused - no state change
				lw	t6,-24(a0)							; Preload "next" ot
				sw	t7,(a2)								; Store address of this in first empty slot

				bne	a0,a1,.Used_Test0_Loop						; Loop if not at unrolled end
				addiu	a0,-4								; Increase ot pointer

.Used_Done:			jr	ra								; Otherwise we're done! We don't have to do
				nop									; anything special as last used fixed it!!!

		if	0

;-----------------------------------------------------------------------------------------------------------------------------------------------------
;	void CompactOTag(unsigned long int *pOT, int iNumEntries);
				GLOBAL  CompactOTag

CompactOTag:			lw	t0,(a0)								; Preload OT registers
				lw	t1,4(a0)							;
				li	t2,$00FFFFFF							; Fix OT address to low memory region
				and	a0,t2								;
				sll	a1,2								; a1 is the end-address of the OT
				addu	a1,a0								;
				move	a2,a0								; Initialize last used slot (if all empty)
				addiu	a0,4								; Adjust pointer for easy comparison

.Unused1:			beq	a0,a1,.DoneUnused2						; Quit if at end of OT
				lw	t2,4(a0)							; Preload next OT value
.Unused1_Loop:			beq	t0,a0,.Unused2							; This was an unused pointer
				addiu	a0,4								; Increase pointer
				sw	t0,(a2)								; Store this (used) address in last slot

.Used2:				beq	a0,a1,.DoneUsed							; Quit if at end of OT
				lw	t0,4(a0)							; Preload next OT value
				bne	t1,a0,.Used3							; This was a used pointer
				addiu	a0,4								; Increase pointer
				addiu	a2,a0,-8							; Update last slot address

.Unused3:			beq	a0,a1,.DoneUnused1						; Quit if at end of OT
				lw	t1,4(a0)							; Preload next OT value
				beq	t2,a0,.Unused1							; This was an unused pointer
				addiu	a0,4								; Increase pointer
				sw	t2,(a2)								; Store this (used) address in last slot

.Used1:				beq	a0,a1,.DoneUsed							; Quit if at end of OT
				lw	t2,4(a0)							; Preload next OT value
				bne	t0,a0,.Used2							; This was a used pointer
				addiu	a0,4								; Increase pointer
				addiu	a2,a0,-8							; Update last slot address

.Unused2:			beq	a0,a1,.DoneUnused3						; Quit if at end of OT
				lw	t0,4(a0)							; Preload next OT value
				beq	t1,a0,.Unused3							; This was an unused pointer
				addiu	a0,4								; Increase pointer
				sw	t1,(a2)								; Store this (used) address in last slot

.Used3:				beq	a0,a1,.DoneUsed							; Quit if at end of OT
				lw	t1,4(a0)							; Preload next OT value
				bne	t2,a0,.Used1							; This was a used pointer
				addiu	a0,4								; Increase pointer
				addiu	a2,a0,-8							; Update last slot address

.Unused1_LoopStart:		bne	a0,a1,.Unused1_Loop						; Loop if NOT at end of OT
				lw	t2,4(a0)							; Preload next OT value

.DoneUnused2:			sw	t0,(a2)								; Store address of termination entry
				jr	ra								; Return
				nop									;
.DoneUnused1:			sw	t2,(a2)								; Store address of termination entry
				jr	ra								; Return
				nop									;
.DoneUnused3:			sw	t1,(a2)								; Store address of termination entry
.DoneUsed:			jr	ra								; Return
				nop									;

;-----------------------------------------------------------------------------------------------------------------------------------------------------
;	void CompactOTagR(unsigned long int *pOT, int iNumEntries);
				GLOBAL  CompactOTagR

CompactOTagR:			sll	a1,2								; a0 points to the end of the table
				addu	a0,a1								;
				lw	t0,-4(a0)							; Preload OT registers
				lw	t1,-8(a0)							;
				li	t2,$00FFFFFF							; Fix OT address to low memory region
				and	a0,t2								;
				subiu	a0,4								;
				move	a2,a0								; Initialize last used slot (if all empty)
				subu	a1,a0,a1							; a1 is the end-address of the OT
				subiu	a0,4								; Adjust pointer for easy comparison

.Unused1:			beq	a0,a1,.DoneUnused2						; Quit if at end of OT
				lw	t2,-4(a0)							; Preload next OT value
.Unused1_Loop:			beq	t0,a0,.Unused2							; This was an unused pointer
				addiu	a0,-4								; Increase pointer
				sw	t0,(a2)								; Store this (used) address in last slot

.Used2:				beq	a0,a1,.DoneUsed							; Quit if at end of OT
				lw	t0,-4(a0)							; Preload next OT value
				bne	t1,a0,.Used3							; This was a used pointer
				addiu	a0,-4								; Increase pointer
				addiu	a2,a0,8								; Update last slot address

.Unused3:			beq	a0,a1,.DoneUnused1						; Quit if at end of OT
				lw	t1,-4(a0)							; Preload next OT value
				beq	t2,a0,.Unused1							; This was an unused pointer
				addiu	a0,-4								; Increase pointer
				sw	t2,(a2)								; Store this (used) address in last slot

.Used1:				beq	a0,a1,.DoneUsed							; Quit if at end of OT
				lw	t2,-4(a0)							; Preload next OT value
				bne	t0,a0,.Used2							; This was a used pointer
				addiu	a0,-4								; Increase pointer
				addiu	a2,a0,8								; Update last slot address

.Unused2:			beq	a0,a1,.DoneUnused3						; Quit if at end of OT
				lw	t0,-4(a0)							; Preload next OT value
				beq	t1,a0,.Unused3							; This was an unused pointer
				addiu	a0,-4								; Increase pointer
				sw	t1,(a2)								; Store this (used) address in last slot

.Used3:				beq	a0,a1,.DoneUsed							; Quit if at end of OT
				lw	t1,-4(a0)							; Preload next OT value
				bne	t2,a0,.Used1							; This was a used pointer
				addiu	a0,-4								; Increase pointer
				addiu	a2,a0,8								; Update last slot address

.Unused1_LoopStart:		bne	a0,a1,.Unused1_Loop						; Loop if NOT at end of OT
				lw	t2,-4(a0)							; Preload next OT value

.DoneUnused2:			sw	t0,(a2)								; Store address of termination entry
				jr	ra								; Return
				nop									;
.DoneUnused1:			sw	t2,(a2)								; Store address of termination entry
				jr	ra								; Return
				nop									;
.DoneUnused3:			sw	t1,(a2)								; Store address of termination entry
.DoneUsed:			jr	ra								; Return
				nop									;
;-----------------------------------------------------------------------------------------------------------------------------------------------------
	endif
