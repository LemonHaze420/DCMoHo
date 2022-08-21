// cpu.h

// cpu identification
// **** NB: ASSUMES AT LEAST 486 ****

// safe cycle counting support with calibration (if supported)

// IKS Mar 97

#ifndef CPU_H
#define CPU_H

#ifndef USEMMTIMER
#ifdef _MSC_VER		// Microsoft Visual C++
//	#define USEMMTIMER	1	// needs link with "winmm.lib"
	#define USEMMTIMER	0
	#define USE_FTIME	0
#else // __WATCOMC__
	#define USEMMTIMER	0	
	#define USE_FTIME	0
#endif	
#endif

unsigned long RDTSC32(void);
// unchecked 32bit access to time stamp counter
	

// **************************** ProcessorID ***********************

class CPU	// SIMPLE CPU IDENTIFICATION + cycle counting support
{
public:
	CPU()									{ detect(); /* instantaneous */ }
	// just declare a variable of this class, then call the functions below for results
	// assumes at least 486, and only registers FPU through CPUID_features (compatible processors from late 486s onward)

	int getFamily() const					{ return (version==0) ? 4 : ((version>>8)&0xf); }
	// returns 4=486, 5=Intel Pentium/Cyrix 6x86/AMD K5, 6=Intel PentiumPro/Cyrix M2/AMD K6
	// **** NB: ASSUMES AT LEAST 486 ****

	void getVendor(char name[13]) const		{ for (int i=13; --i>=0; ) name[i]=vendor[i]; }
	// returns 12char+null terminator string: "GenuineIntel","AuthenticAMD" or "CyrixInstead"

	int hasMMX() const						{ return (features>>23)&1; } 
	// returns 1=MMX available, 0=not

	int hasRDTSC() const					{ return rdtscON; }
	// returns true if RDTSC instruction is supported on the processor 
	// AND available at the current privilege level

	unsigned long RDTSC() const				{ return (rdtscON) ? RDTSC32() : 0; }
	// returns lowest 32bit value of the time stamp counter (in processor clocks)
	// or safely returns 0 if RDTSC instruction cannot be used

	int clocks_per_millisec(int test_millisec=1000);
	// test runs for test_millisec msecs (default 1second)
	// returns clocks in one msec, e.g. approx 200000 for PentiumPro 200Mhz
	// NB: this is approximate, best with accurate MMTIMERS
	// typically accurate to +-2% for 0.5 sec test time

protected:	// can derive a class and access this if you must
			// see cpu.cpp / Processor manuals for individual feature/version bits
	void	detect();	// instantaneous detect of family/vendor/rdtsc
	int		version;	// type13:12,family11:8,model7:4,stepping3:0
	int		features;	// FPU0,TSC4,CMOV15
	int		rdtscON;	// 1=can use, 0=can't due to privilege level or processor doesn't support it
	char	vendor[16];	// actually always 12chars+null, 3 pad
};


// ************************ Cycle Counting Class ***********************
/* Use e.g.

		CycleCounter C;	// automatically started
			... code to time
		C.stop();
			... more code not timed 
		printf("Time:%d\n",C.clocks());
*/

class CycleCounter
// safe use of RDTSC instruction for precise timing 
// always returns 0 if RDTSC instruction is invalid (on CPU or at current privilege level)
// see CPU::clock_per_millisec for calibration
{
	unsigned long t;
	unsigned long allowed;
public:
	CycleCounter()		{ CPU cpu; allowed = cpu.hasRDTSC(); t=0; start(); }
	// NB: constructor calls start() automatically
	
	void	start()		{ if (allowed) t=RDTSC32(); }
	// starts the clock (NB: this was done automatically by constructor anyway)

	unsigned long stop(){ if (allowed) t=RDTSC32()-t; return t; }
	// stops clock, and returns time (in CPU clocks) between start() and stop()
	// NB: use of ULONGs means the difference value wraps correctly

	unsigned long clocks()	{ return t; }
	// returns time (in CPU clocks) between start() and stop()
};


// **************************** RDTSC Instruction ***********************
// This instruction can only be used if 
//		1) instruction supported on the CPU (usually Pentium or later)
// and	2) valid for the current OS privilege level
// *** See CPU::hasRDTSC() or CycleCounter class above for easy and safe use

#ifdef _MSC_VER		// Microsoft Visual C++

	#pragma warning( disable : 4035 )	
	// stop warning of no return value : edx:eax is valid
	inline __int64 RDTSC64()
	// returns value of processor cycle counter (full 64bits)
	{	__asm
		{
			xor		edx,edx		// compiler bug fixup:otherwise doesn't understand edx corrupted
			_emit	0x0f		// rdtsc instruction
			_emit	0x31		//	sets edx:eax to current 64bit cycle count
			// return is in edx:eax
		}
	}
	#pragma warning( default : 4035 )

	inline unsigned long RDTSC32(void)
	{	return unsigned long(RDTSC64());
	}

#elif defined(__WATCOMC__)
	unsigned long RDTSC32(void);
	#pragma aux RDTSC32 =		 \
		".586					"\
		"	rdtsc				"\
		modify[eax edx]			 \
		value[eax]

#else	
	Error	Compiler inline assembler not supported 
#endif



#endif