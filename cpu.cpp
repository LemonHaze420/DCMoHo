// cpu.cpp

// cpu identification
// assumes at least 486 
// safe cycle counting support with calibration (if supported)

// IKS Mar 97

#include "common.h"

#if TARGET == PC

#include "cpu.h"

#if USEMMTIMER
	// MUST LINK WITH winmm.lib
	#include <windows.h>
	#include <mmsystem.h>
#elif USE_FTIME
	#include <sys/types.h>	//	for _ftime
	#include <sys/timeb.h>	//	
#else
	#include <time.h>
#endif

// ************************** CPUID_valid() assembly **********************

#ifdef _MSC_VER		// Microsoft Visual C++
	int CPUID_valid()
	// returns true if processor has CPUID instruction
	// this is done by testing eflags
	{
		long valid=0;
		__asm
		{
			// check for set/clear bit 21 ID flag -> CPUID instruction valid
			pushfd				// push eflags
			pop		eax			// get eflags
			mov		ecx,eax		// save
			xor		eax,200000h	// attempt flip ID in eflags
			push	eax
			popfd
			pushfd
			pop eax
			xor		eax,ecx		// examine flip
			je		end			// haven't got CPUID
			mov		valid,1		// successfully detected CPUID
			end:
		}
		return valid;
	}

#elif defined(__WATCOMC__)

	int CPUID_valid();	// see Microsoft version above for comments
	#pragma aux CPUID_valid =	 \
		"	xor		ebx,ebx		"\
		"	pushfd				"\
		"	pop		eax			"\
		"	mov		ecx,eax		"\		
		"	xor		eax,200000h	"\
		"	push	eax			"\
		"	popfd				"\
		"	pushfd				"\
		"	pop		eax			"\
		"	xor		eax,ecx		"\
		"	je		end			"\
		"	mov		ebx,1		"\
		"end:					"\
		modify[eax ecx ebx]		 \
		value[eax]

#else	
	Error	Compiler inline assembler not supported 
#endif

// ************************** CPUID() assembly **********************

#ifdef _MSC_VER		// Microsoft Visual C++

	void CPUID(long &EAX, long &EBX, long &ECX, long &EDX)
	// assuming the instruction is valid, calls it 
	// parameter : EAX 
	// returns   : EAX..EDX
	{
		long va,vb,vc,vd;	// rename to avoid assembler name clash
		va=EAX;	
		__asm
		{
			pushad		// NB: compiler doesn't understand CPUID corrupts edx
			mov		eax,va	
			_emit	0x0f				//	CPUID
			_emit	0xa2
			mov		va,eax
			mov		vb,ebx
			mov		vc,ecx
			mov		vd,edx
			popad
		}
		EAX=va;
		EBX=vb;
		ECX=vc;
		EDX=vd;
	}
#elif defined(__WATCOMC__)

	static long CPUID_asm_EBX,CPUID_asm_ECX,CPUID_asm_EDX; 
	// for return values
	long CPUID_asm(long eax);
	#pragma aux CPUID_asm =			 \
		".586						"\
		"	cpuid					"\
		"	mov	CPUID_asm_EBX,ebx	"\
		"	mov	CPUID_asm_ECX,ecx	"\
		"	mov	CPUID_asm_EDX,edx	"\
		parm[eax]					 \
		modify[eax ebx ecx edx]		 \
		value[eax]

	void CPUID(long &EAX, long &EBX, long &ECX, long &EDX)
	// see Microsoft version above for comments
	{
		EAX = CPUID_asm(EAX);
		EBX = CPUID_asm_EBX;
		ECX = CPUID_asm_ECX;
		EDX = CPUID_asm_EDX;
	}

#else	
	Error	Compiler inline assembler not supported 
#endif

// ************************** getCR4() assembly **********************

#if 0	
// needs CPL=0 to do this, else WinNT Privilege Violation
// Win95->does nothing
long getCR4()
// instruction only valid on pentium and above
// returns value of control register 4 (for TSC/PMC accessibility)
{
	#ifdef _MSC_VER		// Microsoft Visual C++
		long CR4val=0;
		__asm 
		{
			pushad			// compiler doesn't understand emits affect registers
mov		eax,0ffff0000h	// debug
//			_emit	00fh
//			_emit	020h	//  mov	eax,CR4
//			_emit	0e0h
			mov		CR4val,eax
			popad
		}
		return CR4val;
	#else	
		Error	Compiler inline assembler not supported 
	#endif
}
#endif

// ***************************** Feature / Version Bits **********************

/* Advanced use only, see Intel processor handbooks
 CPUID feature bits (all 0 if no CPUID instruction on processor)
			0 FPU
			1 VME
			2 DE
			3 PSE
			4 TSC
			5 MSR
			6 PAE
			7 MCE
			8 CX8
			9 APIC
			10,11 reserved
			12 MTRR
			13 PGE
			14 MCA
			15 CMOV
			16-22 reserved
			23 MMX
			24-31 reserved
	*/

/* Advanced use only, see Intel processor handbooks
 CPUID version bits (all 0 if no CPUID instruction on processor)
		0..3	stepping
		4..7	model
		8..11	family, 4=486, 5=Intel Pentium/Cyrix 6x86/AMD K5, 6=Intel PentiumPro/Cyrix M2/AMD K6
		12..13	type	0=OEM chip,1=overdrive,2=dual processor,3=reserved
 */

// ***************************** Detection Procedure **********************


void CPU::detect()
{
	static const char anonvendor[16] = "AnonymousCPU";	// always 12chars+null

	for (int i=13; --i>=0; ) vendor[i]=anonvendor[i];

	rdtscON=0;
	version=0;		// type13:12,family11:8,model7:4,stepping3:0
	features=0;		// FPU0,TSC4,CMOV15
	if (CPUID_valid())	// 1=CPUID instruction supported, 0=isn't
	{
		long eax,ebx,ecx,edx;
		eax=0;
		CPUID(eax,ebx,ecx,edx);		// CPUID(eax=0) -> maxcalls + getvendor
		// returned eax is max value can call CPUID with
		*((long*)&vendor[0])=ebx;	//    "GenuineIntel"
		*((long*)&vendor[4])=edx;	// or "AuthenticAMD"
		*((long*)&vendor[8])=ecx;	// or "CyrixInstead"

		// get version+features (eax=1)
		eax=1;
		CPUID(eax,ebx,ecx,edx);
		version=eax;	// family from version bits11:8
		features=edx;	// MMX = features bit 23

		// NB: MMX can theorectically be falsely detected if FPU emulation is on (CR0.EM[bit2]=1)
		// in which case FPU/MMX instructions generate Int 7 for emulation purposes
		// I doubt this will ever happen since emulation is a 386 kind of thing

		// NB: Pentium Pro can call CPUID (eax=2) for cache/TLB info
		// not implemented

		if (getFamily()>=5)
		{
			// RDTSC detection
			// features bit 4=1 -> got RDTSC instruction and CR4 to check privilege
			// RDTSC enabled at CPL=3 if TimeStampDisable bit in CR4.TSD[bit2]=0
			if ((features&0x10)) rdtscON=1;
			//if ((features&0x10) && ((getCR4()&4)==0)) rdtscON=1;

			// RDPMC detection commented out
			// int rdpmcON=0; // 1=can use RDPMC (but beware CPL=0 needed to program the counters)
			// CR4.PCE[bit8]=1:can use RDPMC,PCE=0:restricted to CPL=0
			//eax = getCR4();
			//if (eax&0x100) rdpmcON=1;	// can use RDPMC
		}
	}
}


int CPU::clocks_per_millisec(int test_millisec)
// test runs for test_millisec msecs (default 1second)
// returns clockrate, e.g. approx 200000000 for PentiumPro 200Mhz
// NB: this is approximate, best with accurate MMTIMERS
// typically accurate to +-2% for 0.5 sec test time
{
	if (rdtscON==0) return 0;	// NB: return 0 might lead to divide by zero

#if USEMMTIMER
	// MUST LINK WITH winmm.lib
	// use accurate Multimedia timers, Win95/WinNT
	unsigned long ta,tb,ra,rb,wait,dt,clockspermsec;
	ta = timeGetTime(); 
	ra = RDTSC();
	wait=ta+test_millisec;
	do
	{	
		tb = timeGetTime(); 
		rb = RDTSC();
	} while (tb<wait);
	dt = tb-ta;
	clockspermsec = (rb-ra)/dt;
#elif USE_FTIME
	unsigned long rb,rd,bt,dt,clockspermsec;
	// use _ftime (Win95/NT)
	_timeb a,b,c,d;
	_ftime(&a);
	do { _ftime(&b); rb = RDTSC(); } while (b.millitm==a.millitm);
	bt = b.time*1000+b.millitm;
	do { _ftime(&c); } while ((c.time*1000+c.millitm) - bt < testtime);
	do { _ftime(&d); rd = RDTSC(); } while (d.millitm==c.millitm);
	dt = (d.millitm+1000*d.time)-bt;
	clockspermsec = (rd-rb)/dt;
#else	// use <time.h>::clock()
	unsigned long rb,rc,clockspermsec,a,b,c,wait;
	a = clock();
	do { b=clock(); rb=RDTSC(); } while (b==a);
//	wait = b+1+(test_millisec*CLOCKS_PER_SEC)/1000;
	wait = b+1+CLOCKS_PER_SEC;
	do { c=clock(); rc=RDTSC(); } while (c<wait);
	//do { c=clock(); rc=RDTSC(); } while (c==b);
	double rate = (double(rc-rb)*double(CLOCKS_PER_SEC)) / double((c-b)*1000);
	clockspermsec = (unsigned long)rate;
#endif
	return clockspermsec;
}

#endif
