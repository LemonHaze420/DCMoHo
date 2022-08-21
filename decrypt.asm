
	opt		at-
	opt		c+
	opt		n-
	opt		h-

	section	.text

	xdef Decrypt


Decrypt:

		mfc0	r12,$3
		or		r13,r0,r4
		sll		r12,r12,16
		srl		r14,r12,16
		or		r12,r12,r14
		or		r14,r0,512

loop:

		lw		r15,0(r13)
		sub		r14,r14,1
		xor		r15,r15,r12
		sw		r15,0(r13)
		add		r13,r13,4
		bne		r14,zero,loop
		nop
		jr		ra
		nop
