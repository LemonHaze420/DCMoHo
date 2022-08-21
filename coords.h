// $Header$

// $Log$
// Revision 1.6  2000-09-27 15:10:06+01  jjs
// AddedFASTFTOL to PC version.
//
// Revision 1.5  2000-08-07 15:46:38+01  jjs
// DC and PC merged code.
//
// Revision 1.4  2000-07-10 15:19:29+01  sds
// SDS - #if TARGET == DC stuff e.t.c.
//
// Revision 1.3  2000-06-30 16:27:53+01  sds
// <>
//
// Revision 1.2  2000-06-30 16:27:15+01  sds
// <>
//

#ifndef	_COORDS_H
#define	_COORDS_H

#if TARGET == PC || TARGET == DC
#include	<math.h>
#elif TARGET == PSX
//!PSX   math headers here?
//#include <mwinline.h>

#endif











//#define	G_PI	GINT(3, 9279)
//#define	G_PI_2	(GINT(3, 9279) / 2)
//#define	G_PI_4	(GINT(3, 9279) / 4)


#define	G_2PI	GINT(411774)
#define	G_PI	GINT(205887)
#define	G_PI_2	GINT(102943)
#define	G_PI_4	GINT(51471)



#if TARGET == PC || TARGET == DC
inline	GINT	GSIN(GINT th) {return DToG(sin(GToF(th)));};
inline	GINT	GCOS(GINT th) {return DToG(cos(GToF(th)));};
inline	SINT	BSIN(SINT th) {return (int)(sin(float(th)*0.0015339807878856412297180875894917f)*65536);};
inline	SINT	BCOS(SINT th) {return (int)(cos(float(th)*0.0015339807878856412297180875894917f)*65536);};

#if TARGET == PC
inline	SINT	rsin(SINT th) 
{
	int t1;
	FASTFTOL(&t1,sin(float(th) / 4096 * 6.283185307179586476925286766559f) * 4096.f);
	return t1;
}

inline	SINT	rcos(SINT th) 
{
	int t1;
	FASTFTOL(&t1,cos(float(th) / 4096 * 6.283185307179586476925286766559f) * 4096.f);
	return t1;
}
#else
inline	SINT	rsin(SINT th) {return (int)(sin(float(th) / 4096 * 6.283185307179586476925286766559f) * 4096.f);};
inline	SINT	rcos(SINT th) {return (int)(cos(float(th) / 4096 * 6.283185307179586476925286766559f) * 4096.f);};
#endif
// that's PI/2048 but PI doesn't seem to be defined here...
inline  GINT	GATAN2(GINT x, GINT y) {return DToG(atan2(GToF(x), GToF(y)));};
inline	SINT	BATAN(SINT x, SINT y) {return int(atan2(x,y)*651.898646904403295309347894773819f);};
#elif TARGET == PSX
//!PSX  sin & cos functions here  // convert to 0 - 4095
//inline	GINT	GSIN(GINT th) {GINT g; g.SetVal(rsin((th / G(100, 34797)).GetVal() & 4095) << (GINT_SHIFT - 12)); return g;};


inline	GINT	GSIN(GINT th) {return GINT((csin((th * GINT(652)).mVal)/* & 4095*/) << (GINT_SHIFT - 12));};
inline	GINT	GCOS(GINT th) {return GINT((ccos((th * GINT(652)).mVal)/* & 4095*/) << (GINT_SHIFT - 12));};


//inline	GINT	GSIN(GINT th) {GINT g; g.SetVal(csin((th * G(0, 652)).GetVal()/* & 4095*/) << (GINT_SHIFT - 12)); return g;};
//inline	GINT	GCOS(GINT th) {GINT g; g.SetVal(ccos((th * G(0, 652)).GetVal()/* & 4095*/) << (GINT_SHIFT - 12)); return g;};


inline	SINT	BSIN(SINT th) {return (csin((th) & 4095)<<(GINT_SHIFT-12));}
inline	SINT	BCOS(SINT th) {return (ccos((th) & 4095)<<(GINT_SHIFT-12));}



inline	GINT	GATAN2(GINT x, GINT y) {return (GINT(MUL64((ratan2(x.mVal, y.mVal) << (GINT_SHIFT - 12)),d_2pi)));};
//inline	GINT	GATAN2(GINT x, GINT y) {return (GINT((ratan2(x.mVal, y.mVal) << (GINT_SHIFT - 12))*d_pi*2));};


//inline	GINT	GATAN2(GINT x, GINT y) {GINT g; g.SetVal(ratan2(x.GetVal(), y.GetVal()) << (GINT_SHIFT - 12)); return g * G_PI * 2;};
inline	SINT	BATAN(SINT x, SINT y) {return ratan2(x, y);};

#endif

#define MAX_POINT_LIST_SIZE 10

class GVector ;

typedef CListArray<GVector, MAX_POINT_LIST_SIZE> PointList ;

//****************************************************************************************************
class	GVector
{
public:
	// Public data
	GINT	X, Y, Z;

	// Construction
	INLINE 	GVector() {};
	INLINE 	GVector(const GINT &x, const GINT &y, const GINT &z ) : X(x), Y(y), Z(z) {};

	//INLINE	GVector(const GVector &v) : X(v.X), Y(v.Y), Z(v.Z) {}
/*	INLINE	GVector(const GVector v)
	{
		X.mVal=(v.X.mVal);
		Y.mVal=(v.Y.mVal);
		Z.mVal=(v.Z.mVal);
	};*/

	// Set
	void	Set( GINT x , GINT y , GINT z ) { X = x; Y = y; Z = z; };

// Arithmetic
//	GVector	operator+( const GVector& a ) const { return GVector(X+a.X,Y+a.Y,Z+a.Z); };
	GVector	operator+( const GVector& a ) const
	{
		return GVector(X+a.X,Y+a.Y,Z+a.Z);
		//GVector g;
		//g.X.mVal = X.mVal + a.X.mVal;
		//g.Y.mVal = Y.mVal + a.Y.mVal;
		//g.Z.mVal = Z.mVal + a.Z.mVal;
		//return g;
	};

//	GVector	operator-( const GVector& a ) const { return GVector(X-a.X,Y-a.Y,Z-a.Z); };
	GVector	operator-( const GVector& a ) const
	{
		return GVector(X-a.X,Y-a.Y,Z-a.Z);
		//GVector g;
		//g.X.mVal = X.mVal - a.X.mVal;
		//g.Y.mVal = Y.mVal - a.Y.mVal;
		//g.Z.mVal = Z.mVal - a.Z.mVal;
		//return g;
	};

	GVector	operator*( GINT a ) const { return GVector(X*a,Y*a,Z*a); };

//	GVector	operator/( GINT a ) const { return GVector(X/a,Y/a,Z/a); };
	GVector	operator/( GINT a ) const { GINT r = G1 / a; return GVector(X*r, Y*r, Z*r); };

	GVector	operator*( SINT a ) const { return GVector(X*a,Y*a,Z*a); };
	GVector	operator/( SINT a ) const { return GVector(X/a,Y/a,Z/a); };
	void	operator+=( const GVector& a ) { X.mVal += a.X.mVal; Y.mVal += a.Y.mVal; Z.mVal +=a.Z.mVal; };
	void	operator-=( const GVector& a ) { X.mVal -= a.X.mVal; Y.mVal -= a.Y.mVal; Z.mVal -=a.Z.mVal; };
	void	operator*=( GINT a ) { X*=a; Y*=a; Z*=a; };
	void	operator/=( GINT a ) { X/=a; Y/=a; Z/=a; };
	void	operator*=( SINT a ) { X*=a; Y*=a; Z*=a; };
	void	operator/=( SINT a ) { X/=a; Y/=a; Z/=a; };

	// Unary minus
	GVector	operator-() const { return GVector(-X,-Y,-Z); };

	// Scalar (dot) product
	GINT	operator*( const GVector& a ) const { return ((X*a.X)+(Y*a.Y)+(Z*a.Z)); };

	// Vector (cross) product - NB: Careful of operator precedence!
	GVector	operator^( const GVector& a ) const
		{ return GVector( (Y*a.Z).mVal - (Z*a.Y).mVal , (Z*a.X).mVal - (X*a.Z).mVal , (X*a.Y).mVal - (Y*a.X).mVal); };

	static	void	Cross3(const GVector &a, const GVector &b, GVector &dest)
	{
		dest.X.mVal = (a.Y*b.Z).mVal - (a.Z*b.Y).mVal;
		dest.Y.mVal = (a.Z*b.X).mVal - (a.X*b.Z).mVal;
		dest.Z.mVal = (a.X*b.Y).mVal - (a.Y*b.X).mVal;
	};

	inline GVector Mult(const GVector &a) const
	{
		return GVector(X * a.X, Y * a.Y, Z * a.Z);
	}

	// (JCL) Magnitude
#if TARGET == PC
	inline	GINT	Magnitude() const
	{
		return GROOT((X*X)+(Y*Y)+(Z*Z));
	}
#else
	GINT Magnitude() const;
#endif

	GINT	BigMagnitude() const;

	inline	GINT	MagnitudeXY() const
	{
		return GROOT((X*X)+(Y*Y));
	}

	// (JCL)
	inline	GINT	MagnitudeSq() const
	{
#if TARGET==FECK //PSX
		int result;
		__evaluate (-1, (long)(&X));
		__evaluate (-2, (long)(&result));
   		__asm_start();
   		__I_lw(12,0,-1);
   		__I_lw(13,4,-1);
   		__I_lw(14,8,-1);
   		__I_sra(12,12,8);
   		__I_sra(13,13,8);
   		__I_sra(14,14,8);
   		__I_mtc2 (12,9);
   		__I_mtc2 (13,10);
   		__I_mtc2 (14,11);
   		gte_sqr12();
		__I_mfc2(12,25);
		__I_mfc2(13,26);
		__I_mfc2(14,27);
		__I_add(12,12,13);
		__I_add(12,12,14);
		__I_sll(12,12,12);
		__I_sw(12,0,-2);
		__asm_end();
		return GINT(result);
#else
		return (X*X)+(Y*Y)+(Z*Z);
#endif
	}
	inline	GINT	MagnitudeSqXY() const
	{
		return (X*X)+(Y*Y);
	}
	// (JCL)
	inline	GINT	Manhattan() const
	{
		return ((X.Abs()) + (Y.Abs())) + (Z.Abs());
	}

	inline	GINT	ManhattanXY() const
	{
		return X.Abs() + Y.Abs();
	}

	// Normalisation
	void	Normalise()
	{
		//GINT	scale = GROOT((X*X)+(Y*Y)+(Z*Z));
		GINT	scale = Magnitude();
		if (!scale.IsZero())
		{
			scale = G1 / scale;
			X *= scale;
			Y *= scale;
			Z *= scale;
		}
	};


	BOOL	InsideConvexPoly(PointList& poly) ;  // please make clockwise

	GVector	NormaliseAndScale(GINT scale)
	{
		Normalise();
		(*this) *= scale;
		return *this;
	}

	void	MakeLook( GINT yaw , GINT pitch )
	{
		X = GSIN(yaw)*GCOS(pitch);
		Y = -GCOS(yaw)*GCOS(pitch);
		Z = GSIN(pitch);
	};

	void	MakeLook( GINT yaw )
	{
		X = GSIN(yaw);
		Y = -GCOS(yaw);
		Z = G0;
	};

	void	MakeStrafe( GINT yaw , GINT pitch , GINT roll )
	{
		X = GCOS(roll)*GCOS(yaw) - GSIN(roll)*GSIN(pitch)*GSIN(yaw);
		Y = GSIN(roll)*GSIN(pitch)*GCOS(yaw) + GCOS(roll)*GSIN(yaw);
		Z = GSIN(roll)*GCOS(pitch);
	};

	void	MakeStrafe( GINT yaw )
	{
		X = GCOS(yaw);
		Y = GSIN(yaw);
		Z = G0;
	};

	void	MakeUp( GINT yaw , GINT pitch , GINT roll )
	{
		X = GSIN(roll)*GCOS(yaw) - GCOS(roll)*GSIN(pitch)*GSIN(yaw);
		Y = GCOS(roll)*GSIN(pitch)*GCOS(yaw) + GSIN(roll)*GSIN(yaw);
		Z = GCOS(roll)*GCOS(pitch);
	};

	// (JCL) bizarre combination function that adds the force to the vector such that
	// the vector is not larger than the force in that direction if you see what I mean.
	void	PushAndClip(GVector &force)
	{

		GINT	prop = G1 - ((*this*force) / force.MagnitudeSq() );
		if (prop <= G0) return;
		*this+=force*prop;
	};

	// Sort of backwards version of the above. "this" is the force this time, and is clipped
	// such that when added to "vel", it won't cause the projection to be greater than clipval
	// this one also clips prop to <=1

	// I'm pretty sure this doesn't work.  oh well
/*	void	InverseClip(GVector &vel, GINT clipval)
	{

		GINT	prop=1 - ((*this*vel) / clipval );
		if (prop<=0  ) return;
		if (prop>=1) prop=1;
		*this*=prop;
	};*/

	void	Clip(const GVector &v)
	{
		GINT p = (*this * v) / v.MagnitudeSq();
		if (p > G1)
		{
			*this -= v * (p - G1);
//			*this -= v * ((1 - p));
		}
	}

	void	Clip(GINT mag)
	{
		GINT m=MagnitudeSq();
		GINT n=mag*mag;

		if (m>n)
		{
			*this *= GROOT(n/m);
		};
	};

	void	ClipXY(GINT mag)
	{
		GINT m=MagnitudeSqXY();
		GINT n=mag*mag;

		if (m>n)
		{
			GINT v = GROOT(n/m);
			this->X *= v;
			this->Y *= v;
		};
	};

	void	ReflectInNormal(GVector n)
	{
		// Assumes n is normalised

		GVector m = - n * (*this * n);
		GVector r = *this + m;

		*this = r + r - *this;
	};

	void	ReflectInNormal(GVector n, GINT damp)
	{
		// Assumes n is normalised
		// damp is percentage of velocity || to normal reflected.

		GVector m = - n * (*this * n);
		GVector r = *this + m;

		*this = r + r - *this - m * (G1 - damp);
	};

	// Comparison
	BOOL	operator==( const GVector& a ) const { return ((X==a.X)&&(Y==a.Y)&&(Z==a.Z)); };
	BOOL	operator!=( const GVector& a ) const { return ((X!=a.X)||(Y!=a.Y)||(Z!=a.Z)); };

	// Trace
	void	Trace() const {};
};


//****************************************************************************************************
//****************************************************************************************************
class	GMatrix
{
public:
	// Public data
	GVector	Row[3];

	// Construction
	GMatrix() {};
	GMatrix( const GVector& row0 , const GVector& row1 , const GVector& row2 )
	{
		Row[0] = row0; Row[1] = row1; Row[2] = row2;
	};

	// Comparison
	BOOL	operator==( const GMatrix& a ) const
	{
		return ((Row[0]==a.Row[0]) && (Row[1]==a.Row[1]) && (Row[2]==a.Row[2]));
	};
	BOOL	operator!=( const GMatrix& a ) const
	{
		return ((Row[0]!=a.Row[0]) || (Row[1]!=a.Row[1]) || (Row[2]!=a.Row[2]));
	};

	// Basic arithmetic and scalar multiples
	GMatrix	operator+( const GMatrix& a ) const
		{ return GMatrix(Row[0]+a.Row[0],Row[1]+a.Row[1],Row[2]+a.Row[2]); };
	GMatrix	operator-( const GMatrix& a ) const
		{ return GMatrix(Row[0]-a.Row[0],Row[1]-a.Row[1],Row[2]-a.Row[2]); };
	GMatrix	operator*( GINT a ) const
		{ return GMatrix(Row[0]*a,Row[1]*a,Row[2]*a); };
	GMatrix operator/( GINT a ) const
		{ return GMatrix(Row[0]/a,Row[1]/a,Row[2]/a); };
	void	operator+=( const GMatrix& a )
		{ Row[0]+=a.Row[0]; Row[1]+=a.Row[1]; Row[2]+=a.Row[2]; };
	void	operator-=( const GMatrix& a )
		{ Row[0]-=a.Row[0]; Row[1]-=a.Row[1]; Row[2]-=a.Row[2]; };
	void	operator*=( GINT a )
		{ Row[0]*=a; Row[1]*=a; Row[2]*=a; };
	void	operator/=( GINT a )
		{ Row[0]/=a; Row[1]/=a; Row[2]/=a; };

	// Column extraction
	GVector	Column0() const { return GVector(Row[0].X,Row[1].X,Row[2].X); };
	GVector	Column1() const { return GVector(Row[0].Y,Row[1].Y,Row[2].Y); };
	GVector	Column2() const { return GVector(Row[0].Z,Row[1].Z,Row[2].Z); };

	// and similar ones for the rows, for tidiness
	const GVector &Row0() const { return Row[0]; }
	const GVector &Row1() const { return Row[1]; }
	const GVector &Row2() const { return Row[2]; }

	// Multiplication with vector
	GVector	operator*( const GVector& a ) const;

	// Transpose
	GMatrix	Transpose() const { return GMatrix(Column0(),Column1(),Column2()); };
	void	TransposeInPlace()
	{
		GINT	t;
		t = Row[0].Y; Row[0].Y = Row[1].X; Row[1].X = t;
		t = Row[0].Z; Row[0].Z = Row[2].X; Row[2].X = t;
		t = Row[1].Z; Row[1].Z = Row[2].Y; Row[2].Y = t;
	};

	// Matrix multiply
	GMatrix	operator*( const GMatrix& a ) const;

	// Determinant
	GINT	Determinant() const
	{
		GINT	result;

		result =  Row[0].X * ((Row[1].Y * Row[2].Z) - (Row[1].Z * Row[2].Y));
		result -= Row[0].Y * ((Row[1].X * Row[2].Z) - (Row[1].Z * Row[2].X));
		result += Row[0].Z * ((Row[1].X * Row[2].Y) - (Row[1].Y * Row[2].X));

		return result;
	};

	// Adjoint
	GMatrix	Adjoint() const
	{
		GMatrix result;

		result.Row[0].X =  ((Row[1].Y * Row[2].Z) - (Row[1].Z * Row[2].Y));
		result.Row[0].Y = -((Row[1].X * Row[2].Z) - (Row[1].Z * Row[2].X));
		result.Row[0].Z =  ((Row[1].X * Row[2].Y) - (Row[1].Y * Row[2].X));

		result.Row[1].X = -((Row[0].Y * Row[2].Z) - (Row[0].Z * Row[2].Y));
		result.Row[1].Y =  ((Row[0].X * Row[2].Z) - (Row[0].Z * Row[2].X));
		result.Row[1].Z = -((Row[0].X * Row[2].Y) - (Row[0].Y * Row[2].X));

		result.Row[2].X =  ((Row[0].Y * Row[1].Z) - (Row[0].Z * Row[1].Y));
		result.Row[2].Y = -((Row[0].X * Row[1].Z) - (Row[0].Z * Row[1].X));
		result.Row[2].Z =  ((Row[0].X * Row[1].Y) - (Row[0].Y * Row[1].X));

		return result;
	};

	// Inverse
	GMatrix	Inverse() const
	{
		GMatrix result = Adjoint();
		result.TransposeInPlace();
		result /= Determinant();

		return result;
	};

	void	MakeRotationYaw(GINT yaw);
	void	MakeRotationYawSC(GINT sn, GINT cs);
	void	MakeRotationPitch(GINT pitch);
	void	MakeRotationRoll(GINT roll);
	void	MakeRotationRollSC(GINT sn, GINT cs);

	void	MakeRotation( GINT yaw , GINT pitch , GINT roll );
	void	MakeRotationYRP( GINT yaw , GINT roll , GINT pitch );
	void	MakeGeneralAxisRotation(GVector u, GINT theta);
	void	MakeGeneralAxisRotationSC(GVector u, GINT s, GINT c);

	// when linearly interpolating matrixes, this is all you have to do to make the result orthonormal
	void NormaliseVectorLengthsOnly()
	{
		Row[0].Normalise();
		Row[1].Normalise();
		Row[2].Normalise();
	}

	void	Normalise();

	// Some useful matrices
	void	MakeNull() { Row[0]=Row[1]=Row[2]=GVector(G0, G0, G0); };
	void	MakeID() { Row[0] = GVector(G1, G0, G0); Row[1] = GVector(G0, G1, G0); Row[2] = GVector(G0, G0, G1); };

	// Trace
	void	Trace() const {};

	// look matrix
	//!! Optimise this
	void	BuildLookMatrix(GVector v)
	{
		v.Normalise();  // is this needed?

		SINT	axis;
		GINT	max;

		// Find the largest component of the surface normal
		max = GABS(v.X);
		axis = 0;
		if (GABS(v.Y) > max)
		{
			axis = 1;
			max = GABS(v.Y);
		}
		if (GABS(v.Z) > max)
			axis = 2;

		GVector	vec1,vec2;

		switch( axis)
		{
		case 0:
			// Its X, so cross with Y
			vec1 = GVector( G0, G0, G1) ^ v;
			vec1.Normalise();
			vec2 = v ^ vec1;
			vec2.Normalise();
			break;

		case 1:
			// Its Y, so ok to use X
			vec1 = GVector( G1, G0, G0) ^ v;
			vec1.Normalise();
			vec2 = v ^ vec1;
			vec2.Normalise();
			break;

		case 2:
			// Its Z, so ok to use X
			vec1 = GVector( G1, G0, G0) ^ v;
			vec1.Normalise();
			vec2 = v ^ vec1;
			vec2.Normalise();
			break;
		}

		// Build matrix from vectors
		GVector	newnormal = -v;
//		newnormal.Z = -newnormal.Z;

//		mOrientation = GMatrix( vec2, vec1, newnormal);
		Row[0] = vec2;
		Row[1] = vec1;
		Row[2] = newnormal;

		TransposeInPlace();
	}

/*	void	BuildLookMatrix(GVector v)
	{
		v.Normalise();

		Row[0] = v;

		Row[1].X =  v.Y;
		Row[1].Y = -v.X;
		Row[1].Z = -v.Z;

		Row[2].X = -v.X;
		Row[2].Y =  v.Z;
		Row[2].Z = -v.Y;

	}*/

/*	void	BuildLookMatrix(GVector v, GVector l)  // assumes l is normalised
	{
		v.Normalise();

		GVector r =v ^ l;

		if (r.MagnitudeSq() < GINT(0, 5))
		{
			Row[0] = GVector(G1, G0, G0);
			Row[1] = GVector(G0, G1, G0);
			Row[2] = GVector(G0, G0, G1);
			return;
		}
		r.Normalise();

		// calculate angle
		GINT	dot = v * l;
		if (dot > G1)
			dot = G1;
		if (dot < G(-1))
			dot = G(-1); // Hmmm...

		GINT theta = DToG(acos(GToF(dot)));  // mag v * mag l which are both 1

		MakeGeneralAxisRotation(r, -theta);
	}*/
};

//****************************************************************************************************

extern	GVector	ZERO_GVECTOR;
extern	GMatrix	ID_GMATRIX;

//****************************************************************************************************
#endif
