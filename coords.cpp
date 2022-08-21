#include 	"Common.h"

#include	"Coords.h"

#if TARGET == PSX
#include <gtemac.h>
#include <mwinline.h>
#endif

//****************************************************************************************************
BOOL	GVector::InsideConvexPoly(PointList& poly)
{
	ASSERT (poly.Size() >= 2)

	// points in list for convex poly must be defined clockwise

	GVector* p2, *p1 = poly.First() ;
	GVector* first = p1 ;
	while ((p2 = poly.Next()) != NULL)
	{
		GVector p1_to_p2 = *p2 - *p1 ;
		GVector u_to_p1 = *p1 - (*this);
		u_to_p1.Z = G(0) ;
		// calculate perpendicular
		GVector perp_p1_p2 = GVector(-p1_to_p2.Y, p1_to_p2.X,G(0) ) ;
		GINT dot = perp_p1_p2* u_to_p1;
		if ( dot < G(0) ) return FALSE;
	    p1=p2 ;
	}

	p2 = first ;
	GVector p1_to_p2 = *p2 - *p1 ;
	GVector u_to_p1 = *p1 - (*this) ;
	u_to_p1.Z = G(0) ;
	// calculate perpendicular
	GVector perp_p1_p2 = GVector(-p1_to_p2.Y, p1_to_p2.X,G(0) ) ;
	if ( (perp_p1_p2* u_to_p1 ) < G(0) ) return FALSE;

	return TRUE ;
}


//****************************************************************************************************
//****************************************************************************************************
#if TARGET == PSX
// T0,T1,T2 are the matrix row currently needed.
// T3,T4,T5 are the input vector....
// T6,T7,T8 are the output vector.... (uses T9 as workspace)

GVector	asm GMatrix::operator*( const GVector& a ) const
{

	lw		t0,0(a1)                    // Row[0].X
	lw		t3,0(a2)	                // a.X
	lw		t1,4(a1)                    // Row[0].Y
	mult	t0,t3                       // Row[0].X * a.X
	lw		t2,8(a1)                    // Row[0].Z
	lw		t4,4(a2)                    // a.Y
	mflo	v0
	mfhi	v1
	mult	t1,t4                       // Row[0].Y * a.Y
	srl		v0,v0,16
	sll		v1,v1,16
	or		t6,v0,v1                    // T6 = Row[0].X* a.X
	lw		t5,8(a2)                    // a.Z
	mflo	v0
	mfhi	v1
	mult	t2,t5                       // Row[0].Z * a.Z
	lw		t0,12(a1)                   // Row[1].X
	srl		v0,v0,16
	sll		v1,v1,16
	or		t9,v0,v1                    // T9 = Row[0].Y * a.Y
	add		t6,t6,t9
	mflo	v0
	mfhi	v1
	mult	t0,t3                       // Row[0].X * a.X
	lw		t1,16(a1)                   // Row[1].Y
	srl		v0,v0,16
	sll		v1,v1,16
	or		t9,v0,v1                    // T9 = Row[0].Z * a.Z
	add		t6,t6,t9                    // T6 = (Row[0].X*a.X) + (Row[0].Y*a.Y) + (Row[0].Z*a.Z)
	mflo	v0
	mfhi	v1
	mult	t1,t4                       // Row[1].Y * a.Y
	lw		t2,20(a1)                   // Row[1].Z
	srl		v0,v0,16
	sll		v1,v1,16
	or		t7,v0,v1                    // T7 = Row[1].X* a.X
	mflo	v0
	mfhi	v1
	mult	t2,t5                       // Row[1].Z * a.Z
	lw		t0,24(a1)                   // Row[2].X
	srl		v0,v0,16
	sll		v1,v1,16
	or		t9,v0,v1                    // T9 = Row[1].Y* a.Y
	add		t7,t7,t9
	mflo	v0
	mfhi	v1
	mult	t0,t3                       // Row[2].X * a.X
	lw		t1,28(a1)                   // Row[2].Y
	srl		v0,v0,16
	sll		v1,v1,16
	or		t9,v0,v1                    // T9 = Row[1].Z* a.Z
	add		t7,t7,t9                    // T7 = (Row[1].X*a.X) + (Row[1].Y*a.Y) + (Row[1].Z*a.Z)
	mflo	v0
	mfhi	v1
	mult	t1,t4                       // Row[2].Y * a.Y
	lw		t2,32(a1)                   // Row[2].Z
	srl		v0,v0,16
	sll		v1,v1,16
	or		t8,v0,v1                    // T8 = Row[2].X* a.X
	mflo	v0
	mfhi	v1
	mult	t2,t5                       // Row[2].Z * a.Z
	sw		t6,0(a0)                    // !!!STORE FIRST RESULT!!!
	srl		v0,v0,16
	sll		v1,v1,16
	or		t9,v0,v1                    // T9 = Row[1].Z* a.Z
	add		t8,t8,t9
	mflo	v0
	mfhi	v1
	sw		t7,4(a0)                    // !!!STORE SECOND RESULT!!!
	srl		v0,v0,16
	sll		v1,v1,16
	or		t9,v0,v1                    // T9 = Row[1].Z* a.Z
	add		t8,t8,t9
	jr		ra
	sw		t8,8(a0)                    // !!!STORE THIRD RESULT!!!

}



#else
GVector	GMatrix::operator*( const GVector& a ) const
{
	return	GVector( (Row[0].X*a.X) + (Row[0].Y*a.Y) + (Row[0].Z*a.Z) ,
					 (Row[1].X*a.X) + (Row[1].Y*a.Y) + (Row[1].Z*a.Z) ,
					 (Row[2].X*a.X) + (Row[2].Y*a.Y) + (Row[2].Z*a.Z) );
}
#endif

//****************************************************************************************************
// JCL 20/6 - fix up matrix errors by normalising each row.
// (This is, of course, complete bollocks.)
void	GMatrix::Normalise()
{
	// 0 most dominant
	Row[0].Normalise();
	Row[1].Normalise();
	GVector::Cross3(Row[0], Row[1], Row[2]);
	Row[2].Normalise();
	GVector::Cross3(Row[2], Row[0], Row[1]);
}

//****************************************************************************************************
#if TARGET == PSX
inline void gte_stclmvG(SLONG *ptr)
{
	register x,y,z;
	asm volatile("	mfc2	%0,$25
					mfc2    %1,$26
					mfc2	%2,$27
					sll		%0,%0,4
					sll		%1,%1,4
					sll		%2,%2,4
					sw		%0,0(%3)
					sw		%1,4(%3)
					sw		%2,8(%3)":"=r"(x),"=r"(y),"=r"(z):"r"(ptr):);
}
inline void gte_ldiri(SLONG a,SLONG x,SLONG y,SLONG z)
{
	asm volatile("	mtc2	%0,$8
					mtc2	%1,$9
					mtc2	%2,$10
					mtc2	%3,$11"::"r"(a),"r"(x),"r"(y),"r"(z):);
}
inline void gte_ldiri_nops(SLONG a,SLONG x,SLONG y,SLONG z)
{
	asm volatile("	nop
					nop
					mtc2	%0,$8
					mtc2	%1,$9
					mtc2	%2,$10
					mtc2	%3,$11"::"r"(a),"r"(x),"r"(y),"r"(z):);
}

GMatrix	asm GMatrix::operator*( const GMatrix& a ) const
{

	lw	v0,0(a1)
	lw	t0,0(a2)
	lw	t1,4(a2)
	lw	t2,8(a2)
	sra	v0,v0,4
	sra	t0,t0,4
	sra	t1,t1,4
	sra	t2,t2,4
	mtc2	v0,C2_IR0
	mtc2	t0,C2_IR1
	mtc2	t1,C2_IR2
	mtc2	t2,C2_IR3

	lw	v0,4(a1)
	lw	t3,12(a2)
	GPF(1)
	lw	t4,16(a2)
	lw	t5,20(a2)
	sra	v0,v0,4
	sra	t3,t3,4
	sra	t4,t4,4
	sra	t5,t5,4
	mtc2	v0,C2_IR0
	mtc2	t3,C2_IR1
	mtc2	t4,C2_IR2
	mtc2	t5,C2_IR3


	lw	v0,8(a1)
	lw	t6,24(a2)
	GPL(1)
	lw	t7,28(a2)
	lw	t8,32(a2)
	sra	v0,v0,4
	sra	t6,t6,4
	sra	t7,t7,4
	sra	t8,t8,4
	mtc2	v0,C2_IR0
	mtc2	t6,C2_IR1
	mtc2	t7,C2_IR2
	mtc2	t8,C2_IR3
	lw	v0,12(a1)
	GPL(1)
	mfc2	v1,C2_MAC1
	mfc2	at,C2_MAC2
	mfc2	a2,C2_MAC3
	sll		v1,v1,4
	sll		at,at,4
	sll		a2,a2,4
	sw		v1,0(a0)
	sw		at,4(a0)
	sw		a2,8(a0)

	sra		v0,v0,4
	mtc2	v0,C2_IR0
	mtc2	t0,C2_IR1
	mtc2	t1,C2_IR2
	mtc2	t2,C2_IR3
	lw	v0,16(a1)
	GPF(1)
	sra		v0,v0,4

	mfc2	zero,C2_MAC0                // Force wait for GTE
	mtc2	v0,C2_IR0
	mtc2	t3,C2_IR1
	mtc2	t4,C2_IR2
	mtc2	t5,C2_IR3
	lw	v0,20(a1)
	GPL(1)
	sra	v0,v0,4
	mfc2	zero,C2_MAC0                // Force wait for GTE
	mtc2	v0,C2_IR0
	mtc2	t6,C2_IR1
	mtc2	t7,C2_IR2
	mtc2	t8,C2_IR3
	nop
	nop
	GPL(1)
	lw	v0,24(a1)
	mfc2	v1,C2_MAC1
	mfc2	at,C2_MAC2
	mfc2	a2,C2_MAC3
	sll		v1,v1,4
	sll		at,at,4
	sll		a2,a2,4
	sw		v1,12(a0)
	sw		at,16(a0)
	sw		a2,20(a0)
	//////////
	sra		v0,v0,4
	mtc2	v0,C2_IR0
	mtc2	t0,C2_IR1
	mtc2	t1,C2_IR2
	mtc2	t2,C2_IR3
	lw	v0,28(a1)
	GPF(1)
	sra		v0,v0,4

	mfc2	zero,C2_MAC0                // Force wait for GTE
	mtc2	v0,C2_IR0
	mtc2	t3,C2_IR1
	mtc2	t4,C2_IR2
	mtc2	t5,C2_IR3
	lw	v0,32(a1)
	nop
	sra	v0,v0,4
	GPL(1)
	mfc2	zero,C2_MAC0                // Force wait for GTE
	mtc2	v0,C2_IR0
	mtc2	t6,C2_IR1
	mtc2	t7,C2_IR2
	mtc2	t8,C2_IR3
	nop
	nop
	GPL(1)
	mfc2	v1,C2_MAC1
	mfc2	at,C2_MAC2
	mfc2	a2,C2_MAC3
	sll		v1,v1,4
	sll		at,at,4
	sll		a2,a2,4
	sw		v1,24(a0)
	sw		at,28(a0)
	sw		a2,32(a0)
	jr	ra
	add		v0,zero,a0
}
#else
GMatrix	GMatrix::operator*( const GMatrix& a ) const
{
	GMatrix	m;

	m.Row[0].X = Row[0].X*a.Row[0].X+Row[0].Y*a.Row[1].X+Row[0].Z*a.Row[2].X;
	m.Row[0].Y = Row[0].X*a.Row[0].Y+Row[0].Y*a.Row[1].Y+Row[0].Z*a.Row[2].Y;
	m.Row[0].Z = Row[0].X*a.Row[0].Z+Row[0].Y*a.Row[1].Z+Row[0].Z*a.Row[2].Z;
	m.Row[1].X = Row[1].X*a.Row[0].X+Row[1].Y*a.Row[1].X+Row[1].Z*a.Row[2].X;
	m.Row[1].Y = Row[1].X*a.Row[0].Y+Row[1].Y*a.Row[1].Y+Row[1].Z*a.Row[2].Y;
	m.Row[1].Z = Row[1].X*a.Row[0].Z+Row[1].Y*a.Row[1].Z+Row[1].Z*a.Row[2].Z;
	m.Row[2].X = Row[2].X*a.Row[0].X+Row[2].Y*a.Row[1].X+Row[2].Z*a.Row[2].X;
	m.Row[2].Y = Row[2].X*a.Row[0].Y+Row[2].Y*a.Row[1].Y+Row[2].Z*a.Row[2].Y;
	m.Row[2].Z = Row[2].X*a.Row[0].Z+Row[2].Y*a.Row[1].Z+Row[2].Z*a.Row[2].Z;

	return m;
}

#endif
//****************************************************************************************************
void	GMatrix::MakeRotationYaw(GINT yaw)
{
	GINT 	cs = GCOS(yaw);
	GINT 	sn = GSIN(yaw);

    Row[0].X =  cs;
    Row[0].Y = -sn;
    Row[0].Z =  G0;
    Row[1].X =  sn;
    Row[1].Y =  cs;
    Row[1].Z =  G0;
    Row[2].X =  G0;
    Row[2].Y =  G0;
    Row[2].Z =  G1;
}


//****************************************************************************************************
void	GMatrix::MakeRotationYawSC(GINT sn, GINT cs)
{
    Row[0].X =  cs;
    Row[0].Y = -sn;
    Row[0].Z =  G0;
    Row[1].X =  sn;
    Row[1].Y =  cs;
    Row[1].Z =  G0;
    Row[2].X =  G0;
    Row[2].Y =  G0;
    Row[2].Z =  G1;
}

//****************************************************************************************************
void	GMatrix::MakeRotationPitch(GINT pitch)
{
//    Row[0] = GVector( G1 , G0 , G0 );
//    Row[1] = GVector( G0 , GCOS(pitch) , -GSIN(pitch) );
//    Row[2] = GVector( G0 , GSIN(pitch) , GCOS(pitch) );

	GINT 	cs = GCOS(pitch);
	GINT 	sn = GSIN(pitch);

    Row[0].X =  G1;
    Row[0].Y =  G0;
    Row[0].Z =  G0;
    Row[1].X =  G0;
    Row[1].Y =  cs;
    Row[1].Z = -sn;
    Row[2].X =  G0;
    Row[2].Y =  sn;
    Row[2].Z =  cs;

}

//****************************************************************************************************
void	GMatrix::MakeRotationRoll(GINT roll)
{
//  Row[0] = GVector( GCOS(roll), G0, GSIN(roll));
//	Row[1] = GVector( G0      , G1, G0      );
//	Row[2] = GVector(-GSIN(roll), G0, GCOS(roll));

	GINT 	cs = GCOS(roll);
	GINT 	sn = GSIN(roll);

    Row[0].X =  cs;
    Row[0].Y =  G0;
    Row[0].Z =  sn;
    Row[1].X =  G0;
    Row[1].Y =  G1;
    Row[1].Z =  G0;
    Row[2].X = -sn;
    Row[2].Y =  G0;
    Row[2].Z =  cs;
}

//****************************************************************************************************
void	GMatrix::MakeRotationRollSC(GINT sn, GINT cs)
{
    Row[0].X =  cs;
    Row[0].Y =  G0;
    Row[0].Z =  sn;
    Row[1].X =  G0;
    Row[1].Y =  G1;
    Row[1].Z =  G0;
    Row[2].X = -sn;
    Row[2].Y =  G0;
    Row[2].Z =  cs;
}

//****************************************************************************************************
void	GMatrix::MakeGeneralAxisRotation(GVector u, GINT theta)
// rotates about an arbitrary unit vector axis u, (RH coords)
//	theta11 = angle of clockwise rotation [0..TRIG_DEGREES]
{
	//! I'm sure this code doesn't work...

	GINT
			s	= GSIN(theta),
			c	= GCOS(theta),
			mc	= (G1) - c,
			uxx = u.X*u.X,
			uyy = u.Y*u.Y,
			uzz = u.Z*u.Z,
			uxs	= u.X*s,
			uys	= u.Y*s,
			uzs	= u.Z*s,
			uxymc = (u.X*u.Y)*mc,
			uyzmc = (u.Y*u.X)*mc,
			uzxmc = (u.Z*u.Z)*mc,
			c_1muxx = c*((G1)-uxx),
			c_1muyy = c*((G1)-uyy),
			c_1muzz = c*((G1)-uzz);

	// done : 15 mul, 13 adds, 2 table lookups
	Row[0].X=uxx+c_1muxx;	Row[0].Y=uxymc-uzs;		Row[0].Z=uzxmc+uys;
	Row[1].X=uxymc+uzs;		Row[1].Y=uyy+c_1muyy;	Row[1].Z=uyzmc-uxs;
	Row[2].X=uzxmc-uys;		Row[2].Y=uyzmc+uxs;		Row[2].Z=uzz+c_1muzz;
}

//****************************************************************************************************
void	GMatrix::MakeGeneralAxisRotationSC(GVector u, GINT s, GINT c)
// rotates about an arbitrary unit vector axis u, (RH coords)
//	theta11 = angle of clockwise rotation [0..TRIG_DEGREES]
{
	GINT
			mc	= G1 - c,
			uxx = u.X*u.X,
			uyy = u.Y*u.Y,
			uzz = u.Z*u.Z,
			uxs	= u.X*s,
			uys	= u.Y*s,
			uzs	= u.Z*s,
			uxymc = (u.X*u.Y)*mc,
			uyzmc = (u.Y*u.X)*mc,
			uzxmc = (u.Z*u.Z)*mc,
			c_1muxx = c*(G1 - uxx),
			c_1muyy = c*(G1 - uyy),
			c_1muzz = c*(G1 - uzz);

	// done : 15 mul, 13 adds, 2 table lookups
	Row[0].X=uxx+c_1muxx;	Row[0].Y=uxymc-uzs;		Row[0].Z=uzxmc+uys;
	Row[1].X=uxymc+uzs;		Row[1].Y=uyy+c_1muyy;	Row[1].Z=uyzmc-uxs;
	Row[2].X=uzxmc-uys;		Row[2].Y=uyzmc+uxs;		Row[2].Z=uzz+c_1muzz;
}

//****************************************************************************************************
void	GMatrix::MakeRotation( GINT yaw , GINT pitch , GINT roll )
{
#if TARGET == PSX
	// JCL  - my attempt!
    GINT cy=GCOS(yaw);
    GINT sy=GSIN(yaw);
    GINT cr=GCOS(roll);
    GINT sr=GSIN(roll);
    GINT cp=GCOS(pitch);
    GINT sp=GSIN(pitch);

    Row[0].X = ((cy * cr) - (sy * sp * sr));
    Row[0].Y = -(sy * cp),
    Row[0].Z = ((cy * sr) + (sy * sp * cr));
    Row[1].X = ((sy * cr) + (cy * sp * sr));
    Row[1].Y =  (cy * cp),
    Row[1].Z = ((sy * sr) - (cy * sp * cr));
    Row[2].X = -(cp * sr);
    Row[2].Y =   sp;
    Row[2].Z =  (cp * cr);

#else
    Row[0] = GVector( GCOS(roll), G0, GSIN(roll));
	Row[1] = GVector( G0      , G1, G0      );
	Row[2] = GVector(-GSIN(roll), G0, GCOS(roll));

	GMatrix	temp;
	temp.Row[0] = GVector( G1 , G0 , G0 );
	temp.Row[1] = GVector( G0 , GCOS(pitch) , -GSIN(pitch) );
	temp.Row[2] = GVector( G0 , GSIN(pitch) , GCOS(pitch) );

	*this = temp*(*this);

	temp.Row[0] = GVector( GCOS(yaw) , -GSIN(yaw) , G0 );
	temp.Row[1] = GVector( GSIN(yaw) , GCOS(yaw) , G0 );
	temp.Row[2] = GVector( G0 , G0 , G1 );

	*this = temp*(*this);
#endif
};

//****************************************************************************************************
void	GMatrix::MakeRotationYRP( GINT yaw , GINT roll , GINT pitch )
{
#if TARGET == PSX
	// JCL  - my attempt!
    GINT cy=GCOS(yaw);
    GINT sy=GSIN(yaw);
    GINT cr=GCOS(roll);
    GINT sr=GSIN(roll);
    GINT cp=GCOS(pitch);
    GINT sp=GSIN(pitch);

    Row[0].X =  (cy * cr);
    Row[0].Y = ((cy * sp * sr) - (sy * cp)),
    Row[0].Z = ((cy * cp * sr) + (sy * sp));
    Row[1].X =  (sy * cr);
    Row[1].Y = ((sy * sp * sr) + (cy * cp)),
    Row[1].Z = ((sy * cp * sr) - (cy * sp));
    Row[2].X =  -sr;
    Row[2].Y =  (sp * cr);
    Row[2].Z =  (cp * cr);

#else
	Row[0] = GVector( G1 , G0 , G0 );
	Row[1] = GVector( G0 , GCOS(pitch) , -GSIN(pitch) );
	Row[2] = GVector( G0 , GSIN(pitch) , GCOS(pitch) );

	GMatrix	temp;
    temp.Row[0] = GVector( GCOS(roll), G0, GSIN(roll));
	temp.Row[1] = GVector( G0      , G1, G0      );
	temp.Row[2] = GVector(-GSIN(roll), G0, GCOS(roll));

	*this = temp*(*this);

	temp.Row[0] = GVector( GCOS(yaw) , -GSIN(yaw) , G0 );
	temp.Row[1] = GVector( GSIN(yaw) , GCOS(yaw) , G0 );
	temp.Row[2] = GVector( G0 , G0 , G1 );

	*this = temp*(*this);
#endif
};

//****************************************************************************************************
#if TARGET != PC

inline	GINT GVector::Magnitude() const
	{
		return GROOT((X*X)+(Y*Y)+(Z*Z));
	}

#endif


GINT GVector::BigMagnitude() const
{

	GINT X1 = X >> 2 ;
	GINT Y1 = Y >> 2 ;
	GINT Z1 = Z >> 2 ;

	return (GROOT((X1*X1)+(Y1*Y1)+(Z1*Z1))) << 2;
}
//****************************************************************************************************
GVector	ZERO_GVECTOR = GVector(G0, G0, G0);
GMatrix	ID_GMATRIX = GMatrix(GVector(G1, G0, G0),
							 GVector(G0, G1, G0),
							 GVector(G0, G0, G1));
