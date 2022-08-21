;/*
;
;
;	VLC decoding via bits...
;	Copyright SCEE
;
;	C Form DecDCTvlc3( unsigned long *bs,unsigned long *rl )
;
;
;	05/8/98 Version 1.0	Colin
;
;		bitbased decoder with 2 stage pipeline for memory fetch
;		Type 2 bs stream
;
;
;	20/8/98 Version 1.1	Colin
;		Fixed byte swapping problem caused by long word read
;		(May implement Type 4 & 5 BS with correct ordering to increase speed)
;
;	29/8/98 Version 1.2 Colin
;		Implement type 3 bs stream decoding
;		Add setup routine to use tables from Scratchpad
;
;	10/9/98 Version 1.3 Colin
;		Fixed bug with type 3 Cr DC delta
;		Change table to more economical storage
;
;	11/9/98 Version 1.4 Colin
;		Add break point support
;		Note: Implementation is different to previous versions as check only occurs after macroblock
;;
;*/

 	include "gtereg.h"
	include "inline_a.h"

;#include <asm.h>



;/*
;
;	Bit stream in v0
;	Return code in t0
;	Bit length in t1
;
;*/

#define rl( r,l ) or code,zero,(((r)<<10)|((0 l)&0x3ff))
;;//#define rld( r,l ) .short (((r)<<10)|((0 l)&0x3ff))
#define rld( r,l ) .byte (r),(0 l)



;/*
;	Tables..
;
;	These are relative to tablebase so I can move them to scratchpad ram
;
;*/

	.data

tablebase:

#Offset   0 is 0001xxx table with 8 entries

table7:
	rld( 7,+1 )
;//	rld( 7,-1 )
	rld( 6,+1 )
;//	rld( 6,-1 )
	rld( 1,+2 )
;//	rld( 1,-2 )
	rld( 5,+1 )
;//	rld( 5,-1 )

#Offset  16 is 00001xxx table with 8 entries

table8:
	rld( 2,+2 )
;//	rld( 2,-2 )
	rld( 9,+1 )
;//	rld( 9,-1 )
	rld( 0,+4 )
;//	rld( 0,-4 )
	rld( 8,+1 )
;//	rld( 8,-1 )

#Offset  32 is 0000001xxxx table with 16 entries

table11:
	rld( 16,+1 )
;//	rld( 16,-1 )
	rld( 5,+2 )
;//	rld( 5,-2 )
	rld( 0,+7 )
;//	rld( 0,-7 )
	rld( 2,+3 )
;//	rld( 2,-3 )
	rld( 1,+4 )
;//	rld( 1,-4 )
	rld( 15,+1 )
;//	rld( 15,-1 )
	rld( 14,+1 )
;//	rld( 14,-1 )
	rld( 4,+2 )
;//	rld( 4,-2 )

#Offset  64 is 00000001xxxxx table with 32 entries

table13:
	rld( 0,+11 )
;//	rld( 0,-11 )
	rld( 8,+2 )
;//	rld( 8,-2 )
	rld( 4,+3 )
;//	rld( 4,-3 )
	rld( 0,+10 )
;//	rld( 0,-10 )
	rld( 2,+4 )
;//	rld( 2,-4 )
	rld( 7,+2 )
;//	rld( 7,-2 )
	rld( 21,+1 )
;//	rld( 21,-1 )
	rld( 20,+1 )
;//	rld( 20,-1 )
	rld( 0,+9 )
;//	rld( 0,-9 )
	rld( 19,+1 )
;//	rld( 19,-1 )
	rld( 18,+1 )
;//	rld( 18,-1 )
	rld( 1,+5 )
;//	rld( 1,-5 )
	rld( 3,+3 )
;//	rld( 3,-3 )
	rld( 0,+8 )
;//	rld( 0,-8 )
	rld( 6,+2 )
;//	rld( 6,-2 )
	rld( 17,+1 )
;//	rld( 17,-1 )

#Offset 128 is 000000001xxxxx table with 32 entries

table14:
	rld( 10,+2 )
;//	rld( 10,-2 )
	rld( 9,+2 )
;//	rld( 9,-2 )
	rld( 5,+3 )
;//	rld( 5,-3 )
	rld( 3,+4 )
;//	rld( 3,-4 )
	rld( 2,+5 )
;//	rld( 2,-5 )
	rld( 1,+7 )
;//	rld( 1,-7 )
	rld( 1,+6 )
;//	rld( 1,-6 )
	rld( 0,+15 )
;//	rld( 0,-15 )
	rld( 0,+14 )
;//	rld( 0,-14 )
	rld( 0,+13 )
;//	rld( 0,-13 )
	rld( 0,+12 )
;//	rld( 0,-12 )
	rld( 26,+1 )
;//	rld( 26,-1 )
	rld( 25,+1 )
;//	rld( 25,-1 )
	rld( 24,+1 )
;//	rld( 24,-1 )
	rld( 23,+1 )
;//	rld( 23,-1 )
	rld( 22,+1 )
;//	rld( 22,-1 )

#Offset 192 is 0000000001xxxxx table with 32 entries

table15:
	rld( 0,+31 )
;//	rld( 0,-31 )
	rld( 0,+30 )
;//	rld( 0,-30 )
	rld( 0,+29 )
;//	rld( 0,-29 )
	rld( 0,+28 )
;//	rld( 0,-28 )
	rld( 0,+27 )
;//	rld( 0,-27 )
	rld( 0,+26 )
;//	rld( 0,-26 )
	rld( 0,+25 )
;//	rld( 0,-25 )
	rld( 0,+24 )
;//	rld( 0,-24 )
	rld( 0,+23 )
;//	rld( 0,-23 )
	rld( 0,+22 )
;//	rld( 0,-22 )
	rld( 0,+21 )
;//	rld( 0,-21 )
	rld( 0,+20 )
;//	rld( 0,-20 )
	rld( 0,+19 )
;//	rld( 0,-19 )
	rld( 0,+18 )
;//	rld( 0,-18 )
	rld( 0,+17 )
;//	rld( 0,-17 )
	rld( 0,+16 )
;//	rld( 0,-16 )

#Offset 256 is 00000000001xxxxx table with 32 entries

table16:
	rld( 0,+40 )
;//	rld( 0,-40 )
	rld( 0,+39 )
;//	rld( 0,-39 )
	rld( 0,+38 )
;//	rld( 0,-38 )
	rld( 0,+37 )
;//	rld( 0,-37 )
	rld( 0,+36 )
;//	rld( 0,-36 )
	rld( 0,+35 )
;//	rld( 0,-35 )
	rld( 0,+34 )
;//	rld( 0,-34 )
	rld( 0,+33 )
;//	rld( 0,-33 )
	rld( 0,+32 )
;//	rld( 0,-32 )
	rld( 1,+14 )
;//	rld( 1,-14 )
	rld( 1,+13 )
;//	rld( 1,-13 )
	rld( 1,+12 )
;//	rld( 1,-12 )
	rld( 1,+11 )
;//	rld( 1,-11 )
	rld( 1,+10 )
;//	rld( 1,-10 )
	rld( 1,+9 )
;//	rld( 1,-9 )
	rld( 1,+8 )
;//	rld( 1,-8 )

#Offset 320 is 00000000001xxxxx table with 32 entries

table17:
	rld( 1,+18 )
;//	rld( 1,-18 )
	rld( 1,+17 )
;//	rld( 1,-17 )
	rld( 1,+16 )
;//	rld( 1,-16 )
	rld( 1,+15 )
;//	rld( 1,-15 )
	rld( 6,+3 )
;//	rld( 6,-3 )
	rld( 16,+2 )
;//	rld( 16,-2 )
	rld( 15,+2 )
;//	rld( 15,-2 )
	rld( 14,+2 )
;//	rld( 14,-2 )
	rld( 13,+2 )
;//	rld( 13,-2 )
	rld( 12,+2 )
;//	rld( 12,-2 )
	rld( 11,+2 )
;//	rld( 11,-2 )
	rld( 31,+1 )
;//	rld( 31,-1 )
	rld( 30,+1 )
;//	rld( 30,-1 )
	rld( 29,+1 )
;//	rld( 29,-1 )
	rld( 28,+1 )
;//	rld( 28,-1 )
	rld( 27,+1 )
;//	rld( 27,-1 )


#Offset 384 is 00100xxxx table with 16 entries

table9:
	rld( 13,+1 )
;//	rld( 13,-1 )
	rld( 0,+6 )
;//	rld( 0,-6 )
	rld( 12,+1 )
;//	rld( 12,-1 )
	rld( 11,+1 )
;//	rld( 11,-1 )
	rld( 3,+2 )
;//	rld( 3,-2 )
	rld( 1,+3 )
;//	rld( 1,-3 )
	rld( 0,+5 )
;//	rld( 0,-5 )
table6:					#First 2 entries are invalid codes
	rld( 10,+1 )
;//	rld( 10,-1 )


#Offset 416 is 001xxx table with 6 entries

	rld( 0,+3 )
;//	rld( 0,-3 )
	rld( 4,+1 )
;//	rld( 4,-1 )
	rld( 3,+1 )
;//	rld( 3,-1 )


#End at offset 428

tableend:


/*
	Relative table offsets

*/

#define offset7 0
#define offset8 16
#define offset11 32
#define offset13 64
#define offset14 128
#define offset15 192
#define offset16 256
#define offset17 320
#define offset9 384
#define offset6 412


/*
	Register equates

*/

#define vlc0 v0
#define vlc1 v1
#define code t0
#define len t1
#define vlcptr a0
#define rlptr a1
#define vlcnew a2
#define tableptr a3

#define bitcount t2
#define quantise t3
#define breakptr t4
#define endptr t5

#define temp t6
#define cb t7
#define cr t8
#define lum t9



	.data
	.align 2

break3_cnt:
	.word 0xffffff		#Default length to decode

break3_vars:
	.word 0				#vlc0 save
	.word 0				#vlc1 save
	.word 0				#code save
	.word 0				#vlcptr save
	.word 0				#rlptr save
	.word 0				#vlcnew save
	.word 0				#bitcount save
	.word 0				#quantise save
	.word 0				#endptr save
	.word 0				#cb save
	.word 0				#cr save
	.word 0				#lum save
	.word 0				#return save (0 is type 3)

	.text

	.globl DecDCTvlcBuild3

DecDCTvlcBuild3:

	.set noreorder

	lui a0,0x1f80		#Scratchpad at 0x1f800000
	la a1,tablebase
	or t0,zero,tableend-tablebase

$copy:
	lbu t1,(a1)			#Run
	lbu t2,1(a1)		#Level
	sll t1,t1,10
	sub t3,zero,t2		#Negative level
	and t3,t3,0x3ff
	or t2,t1,t2
	sh t2,0(a0)
	or t3,t1,t3
	sh t3,2(a0)

	add a1,a1,2
	add a0,a0,4
	bgez t0,$copy
	sub t0,t0,2

	j ra
	nop

	.globl DecDCTvlcSize3

DecDCTvlcSize3:

	la t0,break3_cnt	#Variable holding current length

	bgtz a0,$set
	sll a0,a0,1

	lui a0,0xff			#Any negative or zero sets default
	or a0,a0,0xffff

$set:
	lw v0,(t0)			#Return old version
	sw a0,(t0)			#Set new version
	j ra
	nop


	.globl DecDCTvlc3

DecDCTvlc3:

	.set noreorder
	.set noat

	sub sp,sp,4			#Reserve space for return address
	sw ra,(sp)

#Fetch breakpoint address

	la temp,break3_cnt
	lw breakptr,(temp)


	lui tableptr,0x1f80 #Use table based on scratchpad

	bnez vlcptr,$newcall
	sll breakptr,breakptr,1	#Make break length into byte offset

#Restore variables....

	la temp,break3_vars

	lw vlc0,(temp)
	lw vlc1,4(temp)
	lw code,8(temp)
	lw vlcptr,12(temp)
	lw rlptr,16(temp)
	lw vlcnew,20(temp)
	lw bitcount,24(temp)
	lw quantise,28(temp)
	lw endptr,32(temp)
	lw cb,36(temp)
	lw cr,40(temp)
	lw lum,44(temp)
	lw ra,48(temp)		#Return address is only needed for type 2

	add breakptr,breakptr,rlptr


	bnez ra,$continue2
	nop
	b $continue3
	nop

$newcall:
	add breakptr,breakptr,rlptr

	lw code,(vlcptr)	#Fetch header
	lw quantise,4(vlcptr) #Quantise & type(high 16bits)

	lw vlc0,8(vlcptr)	#Fill bitstream pipe
	lw vlc1,12(vlcptr)
	lw vlcnew,16(vlcptr)

	or bitcount,zero,zero			#Initial bit shift
	or cb,zero,zero					#Initial DC for cb
	or cr,zero,zero					#Initial DC for cr
	or lum,zero,zero				#Initial DC for lum


#Word swap...

#if 1
	sll AT,vlc0,16	#Swap shorts
	srl vlc0,vlc0,16
	or vlc0,vlc0,AT

	sll AT,vlc1,16	#Swap shorts
	srl vlc1,vlc1,16
	or vlc1,vlc1,AT


#endif

	sw code,(rlptr)

#Work out end address...

	add rlptr,rlptr,4
	add vlcptr,vlcptr,20

	and code,code,0xffff
	sll code,code,2
	add endptr,rlptr,code

	srl AT,quantise,16				#Check type
	xor AT,AT,3

	and quantise,quantise,63
	beqz AT,$type3
	sll quantise,quantise,10		#Quantise in 16-10


#Default type is type 2....
$type2:

	srl code,vlc0,32-10	#10 bits for first DC value
	or code,code,quantise
	jal $done
	or len,zero,10

#Handle break check here.....


#Type 2 EOB is here...

	srl code,code,32-10 #10 bits for next DC/term

	xor AT,code,0x1ff
	beqz AT,$finished
	subu AT,rlptr,breakptr	#Break check


	bgez AT,$abort2
	or code,code,quantise

$continue2:

	b $done
	or len,zero,12		#DC constant plus 2 EOB


#Bit decoding loop here...


$decodebit:

	bgez vlc0,$b0		#1st bit test
	sll AT,vlc0,1		#2nd bit

#Code 1xx

	bltz AT,$b11x		#Code 11x
	sll AT,vlc0,2		#3rd bit


#Code 10 ( EOB )

	or code,zero,0xfe00	#End of block code
	sh code,(rlptr)		#Store output value
	add rlptr,rlptr,2

	j ra
	sll code,vlc0,2		#Lose 2 bits of EOB

$b11x:


	or len,zero,3		#3 bit codes

	bgez AT,$done		#Code 110
	rl( 0,+1 )

#Code 111

	rl( 0,-1 )

$done:

#Completed value

	sh code,(rlptr)		#Store output value
	add rlptr,rlptr,2

#Remove bits from input stream

	sll vlc0,vlc0,len
	sub AT,zero,len
	srl AT,vlc1,AT
	or vlc0,vlc0,AT


	add bitcount,bitcount,len
	sub AT,bitcount,32
	bltz AT,$decodebit
	sll vlc1,vlc1,len	#Delay slot

#new lw of data


	sub bitcount,bitcount,32	#Adjust length
	sub AT,zero,bitcount


#Load new word and shift

#if 1
	sll code,vlcnew,16	#Swap shorts
	srl vlcnew,vlcnew,16
	or vlcnew,vlcnew,code
#endif

	bnez AT,$shift
	srl AT,vlcnew,AT

	or AT,zero,zero			#Shift right zero is actually 32!!!!!
$shift:

	sll vlc1,vlcnew,bitcount
	lw vlcnew,(vlcptr)
	or vlc0,vlc0,AT
	b $decodebit
	add vlcptr,vlcptr,4

$b0:

	bgez AT,$b00
	sll AT,vlc0,2		#3rd bit

#Code 01xx

	bgez AT,$b010
	sll AT,vlc0,3		#4th bit

#Code 011x

	or len,zero,4		#4 bit codes
	bgez AT,$done		#Code 0110
	rl( 1,+1 )

	b $done				#Code 0111
	rl( 1,-1 )

$b010:
	or len,zero,5		#5 bit codes
	bltz AT,$b0101		#Code 0101x
	sll AT,vlc0,4		#5th bit

	bgez AT,$done		#Code 01000
	rl( 0,+2 )

	b $done				#Code 01001
	rl( 0,-2 )

$b0101:
	bgez AT,$done		#Code 01010
	rl( 2,+1 )

	b $done				#Code 01011
	rl( 2,-1 )



$b00:
	bgez AT,$b000x		#Code 000xxx
	sll AT,vlc0,3		#4th bit

	srl AT,AT,30
	beqz AT,$b00100

#Codes 001xxx ( but not 00100x )

	srl AT,vlc0,31-6
	and AT,AT,7*2

	add AT,AT,tableptr
;//	lh code,table6-tablebase(AT)
	lh code,offset6(AT)
	b $done
	or len,zero,6		#6 bit codes

$b00100:
	srl AT,vlc0,31-9
	and AT,AT,15*2
	add AT,AT,tableptr
;//	lh code,table9-tablebase(AT)
	lh code,offset9(AT)
	b $done
	or len,zero,9		#9 bit codes

/*
	All tables from now on are 000..01xxxx format

*/

$b000x:
	mtc2 vlc0,C2_LZCS
	srl AT,vlc0,32-6	#Check for 000001
	sub AT,AT,1
	beqz AT,$escape
	mfc2 len,C2_LZCR

	bltz AT,$long
	sll AT,vlc0,len		#Make 1xxx....

/*
	0001xxx & 00001xxx 8 entry tables
*/

	srl AT,AT,32-5		#Make ...1xxx0
	and AT,AT,7*2
	sub code,len,3		#Make len 0/1
	sll code,code,4		#0/16 ( 8 shorts )
	add AT,AT,code
	add AT,AT,tableptr
	lh code,(AT)
	b $done
	add len,len,4		#Code length


$long:

	sub code,len,6		#Check for only 16 entry section
	blez code,$eleven
	sll code,code,6		#*64 (32 shorts per table )

	srl AT,AT,32-7		#Make ...1xxxxx0
	and AT,AT,31*2
	add AT,AT,code
	add AT,AT,tableptr
	lh code,(AT)
	b $done
	add len,len,6

$eleven:

	srl AT,AT,32-6		#Make ...1xxxx0
	and AT,AT,15*2
	add AT,AT,tableptr
;//	lh code,table11-tablebase(AT)
	lh code,offset11(AT)
	b $done
	add len,zero,11		#11 bit code

$escape:
	srl code,vlc0,32-6-16	#Get literal 16 bit code
	and code,code,0xffff
	b $done
	add len,zero,22






#Type 3 code will go here, to ensure cache coherency with bit decode loop



#Subroutine to decode luminance block dc

$luminance:


	bltz code,$lum1x		#Code is bitstream stripped of previous EOB code
	sll AT,code,1


	bgez AT,$lum00
	sll AT,AT,1

#Code 01 2 bits DC

	or len,zero,4+2			#4 bits + 2 EOB
	srl code,AT,30			#2 bits of DC
	bltz AT,$lumzero
	add lum,lum,code		#Apply delta
	b $lumzero
	sub lum,lum,3			#Adjust for negative

#Code 00 1 bit DC

$lum00:
	or len,zero,3+2			#3 bits + 2 EOB
	bltz AT,$lumzero
	add lum,lum,1
	b $lumzero
	sub lum,lum,2

$lum1x:
	mtc2 code,C2_LZCS
	bltz AT,$lum11x
	sll temp,AT,1			#Keep AT for later

	bgez temp,$lumzero		#Code 100 is zero value
	or len,zero,5

$lum101:
	or len,zero,6+2			#6 bits + 2 EOB
	sll AT,code,3			#Lose len code
	srl code,AT,32-3
	bltz AT,$lumzero
	add lum,lum,code		#Apply delta
	b $lumzero
	sub lum,lum,7			#Adjust for negative


$lum11x:
	mfc2 len,C2_LZCR

	sub temp,zero,2			#Need delay cycle, so prepare -2 constant
	sll AT,AT,len			#Lose DC len code bits ( AT is already shifted left by 1 bit )

	sub len,temp,len		#Use -2-len (0-len-2) for DC bits

	bltz AT,$lumpos
	srl code,AT,len			#Positive offset

	srl AT,temp,len			#Negative is -(2^n)-1
	sub code,code,AT

$lumpos:
	sub len,zero,len		#lzc+2
	add len,len,len			#code + dc bits = lzc*2+4
	add len,len,1			#Adjust for correct bit length ( remember EOB )


	add lum,lum,code		#Apply delta to luminance

$lumzero:
	sll code,lum,2			#Shift and merge with quantise value
	and code,code,0x3ff
	j $done					#Place DC value and decode rest of macroblock
	or code,code,quantise





$type3:



#Decode cb block


	mtc2 vlc0,C2_LZCS
	bltz vlc0,$cb1xx
	sll AT,vlc0,1

	bgez AT,$cbzero			#Code 00 is zero
	or len,zero,2

#Code 01 is 1 bit

	sll AT,AT,1
	or len,zero,3
	bltz AT,$cbzero
	add cb,cb,1				#Code 011 is +1
	b $cbzero
	sub cb,cb,2				#Code 010 is -1


$cb1xx:

	mfc2 len,C2_LZCR			#Number of leading ones
	sub temp,zero,1			#Need delay cycle, so prepare -1 constant
	sll AT,AT,len			#Lose DC len code bits

	sub len,temp,len		#Use -1-len (0-len-1) for DC bits

	bltz AT,$cbpos
	srl code,AT,len			#Positive offset

	srl AT,temp,len			#Negative is -(2^n)-1
	sub code,code,AT

$cbpos:
	sub len,zero,len		#lzc+1
	add len,len,len			#code + dc bits


	add cb,cb,code			#Apply delta to cb

$cbzero:
	sll code,cb,2			#Shift and merge with quantise value
	and code,code,0x3ff
	jal $done				#Place DC value and decode rest of macroblock
	or code,code,quantise


#Decode cr block...Previous EOB hasnt been flushed















#




	mtc2 code,C2_LZCS		#Code is vlc0 with EOB flushed
	bltz code,$cr1xx
	sll AT,code,1

	bgez AT,$crzero			#Code 00 is zero
	or len,zero,4			#2+2 bits for EOB

#Code 01 is 1 bit

	sll AT,AT,1
	or len,zero,5			#3+2 bits for EOB
	bltz AT,$crzero
	add cr,cr,1				#Code 011 is +1
	b $crzero
	sub cr,cr,2				#Code 010 is -1


$cr1xx:

	mfc2 len,C2_LZCR			#Number of leading ones
	sub temp,zero,1			#Need delay cycle, so prepare -1 constant
	sll AT,AT,len			#Lose DC len code bits

	sub len,temp,len		#Use -1-len (0-len-1) for DC bits

	bltz AT,$crpos
	srl code,AT,len			#Positive offset

	srl AT,temp,len			#Negative is -(2^n)-1
	sub code,code,AT

$crpos:
	sub len,zero,len		#lzc+1
	add len,len,len			#code + dc bits
	add len,len,2			#Add 2 bits for EOB


	add cr,cr,code			#Apply delta to cb

$crzero:
	sll code,cr,2			#Shift and merge with quantise value
	and code,code,0x3ff
	jal $done				#Place DC value and decode rest of macroblock
	or code,code,quantise


#Now process 4 luminance blocks...

	jal $luminance
	nop

	jal $luminance
	nop

	jal $luminance
	nop

	jal $luminance
	nop

#Now check for end of BS stream, and lose 2 bits

#Handle break check here.....

	subu AT,rlptr,breakptr
	bgez AT,$abort3


	srl code,code,32-10		#Check for end of file
$continue3:
	xor code,code,0x3ff

	sll vlc0,vlc0,2			#Lose 2 bits
	srl AT,vlc1,32-2
	or vlc0,vlc0,AT
	sll vlc1,vlc1,2

	bnez code,$type3
	add bitcount,bitcount,2



$finished:

	or code,zero,0xfe00		#Fill with null code

	lw ra,(sp)				#Restore return address
	add sp,sp,4

$fill:
	subu AT,rlptr,endptr
	bgez AT,$exit
	nop

	sh code,(rlptr)
	b $fill
	add rlptr,rlptr,2

$exit:
	j ra
	or v0,zero,zero


$abort2:
	b $abort
	or AT,zero,ra			#Need return address for type 2

$abort3:
	or AT,zero,zero

$abort:

	lw ra,(sp)				#Restore return address
	add sp,sp,4

	la temp,break3_vars

	sw vlc0,(temp)
	sw vlc1,4(temp)
	sw code,8(temp)
	sw vlcptr,12(temp)
	sw rlptr,16(temp)
	sw vlcnew,20(temp)
	sw bitcount,24(temp)
	sw quantise,28(temp)
	sw endptr,32(temp)
	sw cb,36(temp)
	sw cr,40(temp)
	sw lum,44(temp)
	sw AT,48(temp)

	j	ra
	or v0,zero,1 			#Return 1 to indicate decoding is incomplete

