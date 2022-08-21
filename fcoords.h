#ifndef	_FCOORDS_H
#define	_FCOORDS_H

#include	<math.h>

//#define	PI	3.1415926535897932384626433832795
#define	PI	3.141592653f

inline	float	FSIN(SINT th) {return float(sin(float(th) / 2048 * PI));};
inline	float	FCOS(SINT th) {return float(cos(float(th) / 2048 * PI));};

//****************************************************************************************************
class	FVector
{
public:
	// Public data
	float X, Y, Z;

	// Construction
	FVector() {};
	FVector( float x , float y , float z ) : X(x),Y(y),Z(z) {};

	// Set
	void	Set( float x , float y , float z ) { X = x; Y = y; Z = z; };

	// Arithmetic
	FVector	operator+( const FVector& a ) const { return FVector(X+a.X,Y+a.Y,Z+a.Z); };
	FVector	operator-( const FVector& a ) const { return FVector(X-a.X,Y-a.Y,Z-a.Z); };
	FVector	operator*( float a ) const { return FVector(X*a,Y*a,Z*a); };
	FVector	operator/( float a ) const { return FVector(X/a,Y/a,Z/a); };
	void	operator+=( const FVector& a ) { X+=a.X; Y+=a.Y; Z+=a.Z; };
	void	operator-=( const FVector& a ) { X-=a.X; Y-=a.Y; Z-=a.Z; };
	void	operator*=( float a ) { X*=a; Y*=a; Z*=a; };
	void	operator/=( float a ) { X/=a; Y/=a; Z/=a; };

	// Unary minus
	FVector	operator-() const { return FVector(-X,-Y,-Z); };

	// Scalar (dot) product
	float	operator*( const FVector& a ) const { return ((X*a.X)+(Y*a.Y)+(Z*a.Z)); };

	// Vector (cross) product - NB: Careful of operator precedence!
	FVector	operator^( const FVector& a ) const
		{ return FVector( Y*a.Z-Z*a.Y , Z*a.X-X*a.Z , X*a.Y-Y*a.X ); };

	// (JCL) Magnitude
	inline	float	Magnitude() const
	{
		return float(sqrt((X*X)+(Y*Y)+(Z*Z)));
	}
	inline	float	MagnitudeXY() const
	{
		return float(sqrt((X*X)+(Y*Y)));
	}

	// (JCL) 
	inline	float	MagnitudeSq() const
	{
		return float((X*X)+(Y*Y)+(Z*Z));
	}
	inline	float	MagnitudeSqXY() const
	{
		return float((X*X)+(Y*Y));
	}
	// (JCL)
	inline	float	Manhattan() const
	{
		return float(fabs(X)+fabs(Y)+fabs(Z));
	}
	inline	float	ManhattanXY() const
	{
		return float(fabs(X)+fabs(Y));
	}
	
	// Normalisation
	void	Normalise()
	{
		float	scale = float(sqrt((X*X)+(Y*Y)+(Z*Z)));
		if ( scale != 0.0f )
		{
			scale = 1.f / scale;
			X *= scale;
			Y *= scale;
			Z *= scale;
		}
	};

	FVector	NormaliseAndScale(float scale)
	{
		Normalise();
		(*this) *= scale;
		return *this;
	}

	void	MakeLook( SINT yaw , SINT pitch )
	{
		X = FSIN(yaw)*FCOS(pitch);
		Y = -FCOS(yaw)*FCOS(pitch);
		Z = FSIN(pitch);
	};

	void	MakeLook( SINT yaw )
	{
		X = FSIN(yaw);
		Y = -FCOS(yaw);
		Z = 0.0f;
	};

	void	MakeStrafe( SINT yaw , SINT pitch , SINT roll )
	{
		X = FCOS(roll)*FCOS(yaw) - FSIN(roll)*FSIN(pitch)*FSIN(yaw);
		Y = FSIN(roll)*FSIN(pitch)*FCOS(yaw) + FCOS(roll)*FSIN(yaw);
		Z = FSIN(roll)*FCOS(pitch);
	};

	void	MakeStrafe( SINT yaw )
	{
		X = FCOS(yaw);
		Y = FSIN(yaw);
		Z = 0.0f;
	};

	void	MakeUp( SINT yaw , SINT pitch , SINT roll )
	{
		X = FSIN(roll)*FCOS(yaw) - FCOS(roll)*FSIN(pitch)*FSIN(yaw);
		Y = FCOS(roll)*FSIN(pitch)*FCOS(yaw) + FSIN(roll)*FSIN(yaw);
		Z = FCOS(roll)*FCOS(pitch);
	};

	// (JCL) bizarre combination function that adds the force to the vector such that
	// the vector is not larger than the force in that direction if you see what I mean.
	void	PushAndClip(FVector &force)
	{

		float	prop=1 - ((*this*force) / force.MagnitudeSq() );
		if (prop<=0) return;
		*this+=force*prop;
	};

	// Sort of backwards version of the above. "this" is the force this time, and is clipped
	// such that when added to "vel", it won't cause the projection to be greater than clipval
	// this one also clips prop to <=1

	// I'm pretty sure this doesn't work.  oh well
/*	void	InverseClip(FVector &vel, float clipval)
	{

		float	prop=1 - ((*this*vel) / clipval );
		if (prop<=0  ) return;
		if (prop>=1.f) prop=1.f;
		*this*=prop;
	};*/

	void	Clip(const FVector &v)
	{
		float p = (*this * v) / v.MagnitudeSq();
		if (p > 1.f)
		{
			*this -= v * (p - 1.f);
		}
	}
	
	void	Clip(float mag)
	{
		float m=MagnitudeSq();
		float n=mag*mag;

		if (m>n)
		{
			*this*=float(sqrt(n/m));
		};
	};

	void	ClipXY(float mag)
	{
		float m=MagnitudeSqXY();
		float n=mag*mag;

		if (m>n)
		{
			float v = float(sqrt(n/m));
			this->X *= v;
			this->Y *= v;
		};
	};

	void	ReflectInNormal(FVector n)
	{
		// Assumes n is normalised

		FVector m = - n * (*this * n);
		FVector r = *this + m;

		*this = r + r - *this;
	};

	void	ReflectInNormal(FVector n, float damp)
	{
		// Assumes n is normalised
		// damp is percentage of velocity || to normal reflected.

		FVector m = - n * (*this * n);
		FVector r = *this + m;

		*this = r + r - *this - m * (1.f - damp);
	};

	// Comparison
	BOOL	operator==( const FVector& a ) const { return ((X==a.X)&&(Y==a.Y)&&(Z==a.Z)); };
	BOOL	operator!=( const FVector& a ) const { return ((X!=a.X)||(Y!=a.Y)||(Z!=a.Z)); };

	// Trace
	void	Trace() const {};
};

//****************************************************************************************************
//****************************************************************************************************
class	FMatrix
{
public:
	// Public data
	FVector	Row[3];

	// Construction
	FMatrix() {};
	FMatrix( const FVector& row0 , const FVector& row1 , const FVector& row2 )
	{
		Row[0] = row0; Row[1] = row1; Row[2] = row2;
	};

	FMatrix( SINT yaw , SINT pitch , SINT roll )
	{
		MakeRotation(yaw,pitch,roll);
	};

	FMatrix( float yaw, float pitch, float roll )
	{
		MakeRotationF(yaw,pitch,roll);
	};

	// Comparison
	BOOL	operator==( const FMatrix& a ) const 
	{
		return ((Row[0]==a.Row[0]) && (Row[1]==a.Row[1]) && (Row[2]==a.Row[2]));
	};
	BOOL	operator!=( const FMatrix& a ) const 
	{
		return ((Row[0]!=a.Row[0]) || (Row[1]!=a.Row[1]) || (Row[2]!=a.Row[2]));
	};

	// Basic arithmetic and scalar multiples
	FMatrix	operator+( const FMatrix& a ) const
		{ return FMatrix(Row[0]+a.Row[0],Row[1]+a.Row[1],Row[2]+a.Row[2]); };
	FMatrix	operator-( const FMatrix& a ) const
		{ return FMatrix(Row[0]-a.Row[0],Row[1]-a.Row[1],Row[2]-a.Row[2]); };
	FMatrix	operator*( float a ) const
		{ return FMatrix(Row[0]*a,Row[1]*a,Row[2]*a); };
	FMatrix operator/( float a ) const
		{ return FMatrix(Row[0]/a,Row[1]/a,Row[2]/a); };
	void	operator+=( const FMatrix& a )
		{ Row[0]+=a.Row[0]; Row[1]+=a.Row[1]; Row[2]+=a.Row[2]; };
	void	operator-=( const FMatrix& a )
		{ Row[0]-=a.Row[0]; Row[1]-=a.Row[1]; Row[2]-=a.Row[2]; };
	void	operator*=( float a )
		{ Row[0]*=a; Row[1]*=a; Row[2]*=a; };
	void	operator/=( float a )
		{ Row[0]/=a; Row[1]/=a; Row[2]/=a; };

	// Column extraction
	FVector	Column0() const { return FVector(Row[0].X,Row[1].X,Row[2].X); };
	FVector	Column1() const { return FVector(Row[0].Y,Row[1].Y,Row[2].Y); };
	FVector	Column2() const { return FVector(Row[0].Z,Row[1].Z,Row[2].Z); };

	// and similar ones for the rows, for tidiness
	const FVector &Row0() const { return Row[0]; }
	const FVector &Row1() const { return Row[1]; }
	const FVector &Row2() const { return Row[2]; }

	// Multiplication with vector
	FVector	operator*( const FVector& a ) const
	{
		return	FVector( (Row[0].X*a.X) + (Row[0].Y*a.Y) + (Row[0].Z*a.Z) ,
						 (Row[1].X*a.X) + (Row[1].Y*a.Y) + (Row[1].Z*a.Z) ,
						 (Row[2].X*a.X) + (Row[2].Y*a.Y) + (Row[2].Z*a.Z) );
	}
	//FVector	operator*( const FVector& a ) const
	//	{ return FVector(Row[0]*a,Row[1]*a,Row[2]*a); };

	// Multiplication of transpose with vector
	FVector	operator|( const FVector& a ) const
	{
		return FVector(  Row[0].X*a.X+Row[1].X*a.Y+Row[2].X*a.Z,
						 Row[0].Y*a.X+Row[1].Y*a.Y+Row[2].Y*a.Z,
						 Row[0].Z*a.X+Row[1].Z*a.Y+Row[2].Z*a.Z);
	};

	// Transpose
	FMatrix	Transpose() const { return FMatrix(Column0(),Column1(),Column2()); };
	void	TransposeInPlace()
	{
		float	t;
		t = Row[0].Y; Row[0].Y = Row[1].X; Row[1].X = t;
		t = Row[0].Z; Row[0].Z = Row[2].X; Row[2].X = t;
		t = Row[1].Z; Row[1].Z = Row[2].Y; Row[2].Y = t;
	};

	// Matrix multiply
	FMatrix	operator*( const FMatrix& a ) const
	{
		return FMatrix( FVector( Row[0].X*a.Row[0].X+Row[0].Y*a.Row[1].X+Row[0].Z*a.Row[2].X,
						         Row[0].X*a.Row[0].Y+Row[0].Y*a.Row[1].Y+Row[0].Z*a.Row[2].Y,
								 Row[0].X*a.Row[0].Z+Row[0].Y*a.Row[1].Z+Row[0].Z*a.Row[2].Z),
						FVector( Row[1].X*a.Row[0].X+Row[1].Y*a.Row[1].X+Row[1].Z*a.Row[2].X,
						         Row[1].X*a.Row[0].Y+Row[1].Y*a.Row[1].Y+Row[1].Z*a.Row[2].Y,
								 Row[1].X*a.Row[0].Z+Row[1].Y*a.Row[1].Z+Row[1].Z*a.Row[2].Z),
						FVector( Row[2].X*a.Row[0].X+Row[2].Y*a.Row[1].X+Row[2].Z*a.Row[2].X,
						         Row[2].X*a.Row[0].Y+Row[2].Y*a.Row[1].Y+Row[2].Z*a.Row[2].Y,
								 Row[2].X*a.Row[0].Z+Row[2].Y*a.Row[1].Z+Row[2].Z*a.Row[2].Z) );
	};

	// Determinant
	float	Determinant() const
	{
		float	result;

		result =  Row[0].X * ((Row[1].Y * Row[2].Z) - (Row[1].Z * Row[2].Y));
		result -= Row[0].Y * ((Row[1].X * Row[2].Z) - (Row[1].Z * Row[2].X));
		result += Row[0].Z * ((Row[1].X * Row[2].Y) - (Row[1].Y * Row[2].X));

		return result;
	};

	// Adjoint
	FMatrix	Adjoint() const
	{
		FMatrix result;

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
	FMatrix	Inverse() const
	{
		FMatrix result = Adjoint();
		result.TransposeInPlace();
		result /= Determinant();

		return result;
	};

	void	MakeRotationYaw(float yaw)
	{
		Row[0] = FVector( float(cos(yaw)) , -float(sin(yaw)) , 0.0f );
		Row[1] = FVector( float(sin(yaw)) , float(cos(yaw)) , 0.0f );
		Row[2] = FVector( 0.0f , 0.0f , 1.0f );
	};

	void	MakeRotationYawF(float yaw)
	{
		Row[0] = FVector( float(cos(yaw)) , -float(sin(yaw)) , 0.0f );
		Row[1] = FVector( float(sin(yaw)) , float(cos(yaw)) , 0.0f );
		Row[2] = FVector( 0.0f , 0.0f , 1.0f );
	};

	void	MakeRotationRollF(float roll)
	{
		Row[0] = FVector( (float)cos(roll) , 0.0f , (float)sin(roll) );
		Row[1] = FVector( 0.0f , 1.0f , 0.0f );
		Row[2] = FVector( -(float)sin(roll) , 0.0f , (float)cos(roll) );
	};

	void	MakeRotation( SINT yaw , SINT pitch , SINT roll )
	{
		Row[0] = FVector( FCOS(roll) , 0.0f , FSIN(roll) );
		Row[1] = FVector( 0.0f , 1.0f , 0.0f );
		Row[2] = FVector( -FSIN(roll) , 0.0f , FCOS(roll) );

		FMatrix	temp;
		temp.Row[0] = FVector( 1.0f , 0.0f , 0.0f );
		temp.Row[1] = FVector( 0.0f , FCOS(pitch) , -FSIN(pitch) );
		temp.Row[2] = FVector( 0.0f , FSIN(pitch) , FCOS(pitch) );

		*this = temp*(*this);

		temp.Row[0] = FVector( FCOS(yaw) , -FSIN(yaw) , 0.0f );
		temp.Row[1] = FVector( FSIN(yaw) , FCOS(yaw) , 0.0f );
		temp.Row[2] = FVector( 0.0f , 0.0f , 1.0f );

		*this = temp*(*this);
	};

	void	MakeRotationF( float yaw , float pitch , float roll )
	{
		Row[0] = FVector( (float)cos(roll) , 0.0f , (float)sin(roll) );
		Row[1] = FVector( 0.0f , 1.0f , 0.0f );
		Row[2] = FVector( -(float)sin(roll) , 0.0f , (float)cos(roll) );

		FMatrix	temp;
		temp.Row[0] = FVector( 1.0f , 0.0f , 0.0f );
		temp.Row[1] = FVector( 0.0f , (float)cos(pitch) , -(float)sin(pitch) );
		temp.Row[2] = FVector( 0.0f , (float)sin(pitch) , (float)cos(pitch) );

		*this = temp*(*this);

		temp.Row[0] = FVector( (float)cos(yaw) , -(float)sin(yaw) , 0.0f );
		temp.Row[1] = FVector( (float)sin(yaw) , (float)cos(yaw) , 0.0f );
		temp.Row[2] = FVector( 0.0f , 0.0f , 1.0f );

		*this = temp*(*this);
	};

	void	MakeInvertRotation( SINT yaw , SINT pitch , SINT roll )
	{
		yaw = -yaw; pitch = -pitch; roll = -roll;

		Row[0] = FVector( FCOS(yaw) , -FSIN(yaw) , 0.0f );
		Row[1] = FVector( FSIN(yaw) , FCOS(yaw) , 0.0f );
		Row[2] = FVector( 0.0f , 0.0f , 1.0f );

		FMatrix temp;
		temp.Row[0] = FVector( 1.0f , 0.0f , 0.0f );
		temp.Row[1] = FVector( 0.0f , FCOS(pitch) , FSIN(pitch) );
		temp.Row[2] = FVector( 0.0f , -FSIN(pitch) , FCOS(pitch) );

		*this = temp*(*this);

		temp.Row[0] = FVector( FCOS(roll) , 0.0f , FSIN(roll) );
		temp.Row[1] = FVector( 0.0f , 1.0f , 0.0f );
		temp.Row[2] = FVector( -FSIN(roll) , 0.0f , FCOS(roll) );

		*this = temp*(*this);
	}

	// (JCL 20/6) Shamelessly nicked off Ian
	void	MakeGeneralAxisRotation(FVector u, SINT theta)
	// rotates about an arbitrary unit vector axis u, (RH coords)
	//	theta11 = angle of clockwise rotation [0..TRIG_DEGREES]
	{
		float
				s	= FSIN(theta),
				c	= FCOS(theta),
				mc	= (1.0f) - c,
				uxx = u.X*u.X,
				uyy = u.Y*u.Y,
				uzz = u.Z*u.Z,
				uxs	= u.X*s,
				uys	= u.Y*s,		
				uzs	= u.Z*s,
				uxymc = (u.X*u.Y)*mc,
				uyzmc = (u.Y*u.X)*mc,
				uzxmc = (u.Z*u.Z)*mc,
				c_1muxx = c*((1.0f)-uxx),
				c_1muyy = c*((1.0f)-uyy),
				c_1muzz = c*((1.0f)-uzz);

		// done : 15 mul, 13 adds, 2 table lookups
		Row[0].X=uxx+c_1muxx;	Row[0].Y=uxymc-uzs;		Row[0].Z=uzxmc+uys;
		Row[1].X=uxymc+uzs;		Row[1].Y=uyy+c_1muyy;	Row[1].Z=uyzmc-uxs;
		Row[2].X=uzxmc-uys;		Row[2].Y=uyzmc+uxs;		Row[2].Z=uzz+c_1muzz;
	}

	void	MakeGeneralAxisRotationF(FVector u, float theta)
	// rotates about an arbitrary unit vector axis u, (RH coords)
	//	theta11 = angle of clockwise rotation [0..TRIG_DEGREES]
	{
		float
				s	= float(sin(theta)),
				c	= float(cos(theta)),
				mc	= (1.0f) - c,
				uxx = u.X*u.X,
				uyy = u.Y*u.Y,
				uzz = u.Z*u.Z,
				uxs	= u.X*s,
				uys	= u.Y*s,		
				uzs	= u.Z*s,
				uxymc = (u.X*u.Y)*mc,
				uyzmc = (u.Y*u.X)*mc,
				uzxmc = (u.Z*u.Z)*mc,
				c_1muxx = c*((1.0f)-uxx),
				c_1muyy = c*((1.0f)-uyy),
				c_1muzz = c*((1.0f)-uzz);

		// done : 15 mul, 13 adds, 2 table lookups
		Row[0].X=uxx+c_1muxx;	Row[0].Y=uxymc-uzs;		Row[0].Z=uzxmc+uys;
		Row[1].X=uxymc+uzs;		Row[1].Y=uyy+c_1muyy;	Row[1].Z=uyzmc-uxs;
		Row[2].X=uzxmc-uys;		Row[2].Y=uyzmc+uxs;		Row[2].Z=uzz+c_1muzz;
	}

	void	MakeGeneralAxisRotationSC(FVector u, float s, float c)
	// rotates about an arbitrary unit vector axis u, (RH coords)
	//	theta11 = angle of clockwise rotation [0..TRIG_DEGREES]
	{
		float
				mc	= (1.0f) - c,
				uxx = u.X*u.X,
				uyy = u.Y*u.Y,
				uzz = u.Z*u.Z,
				uxs	= u.X*s,
				uys	= u.Y*s,		
				uzs	= u.Z*s,
				uxymc = (u.X*u.Y)*mc,
				uyzmc = (u.Y*u.X)*mc,
				uzxmc = (u.Z*u.Z)*mc,
				c_1muxx = c*((1.0f)-uxx),
				c_1muyy = c*((1.0f)-uyy),
				c_1muzz = c*((1.0f)-uzz);

		// done : 15 mul, 13 adds, 2 table lookups
		Row[0].X=uxx+c_1muxx;	Row[0].Y=uxymc-uzs;		Row[0].Z=uzxmc+uys;
		Row[1].X=uxymc+uzs;		Row[1].Y=uyy+c_1muyy;	Row[1].Z=uyzmc-uxs;
		Row[2].X=uzxmc-uys;		Row[2].Y=uyzmc+uxs;		Row[2].Z=uzz+c_1muzz;
	}
	
	// when linearly interpolating matrixes, this is all you have to do to make the result orthonormal
	void NormaliseVectorLengthsOnly()
	{
		Row[0].Normalise();
		Row[1].Normalise();
		Row[2].Normalise();
	}

	// JCL 20/6 - fix up matrix errors by normalising each row.
	// (This is, of course, complete bollocks.)
	void	Normalise()
	{
		// 0 most dominant
		Row[0].Normalise();
		Row[1].Normalise();
//		Row[2].Normalise();
		Row[2]=Row[0]^Row[1];
		Row[2].Normalise();
		Row[1] = Row[2] ^ Row[0];
	}

	void	Normalise2()
	{	
		// 2 most dominant, then 0 then 1
		Row[0].Normalise();
		Row[2].Normalise();
		Row[1]=Row[2]^Row[0];
		Row[1].Normalise();
		Row[0]=Row[1]^Row[2];		
		Row[0].Normalise();
	}

	void	Normalise3()
	{	
		// 2 most dominant, then 1 then 0
		Row[1].Normalise();
		Row[2].Normalise();
		Row[0]=Row[1]^Row[2];
		Row[0].Normalise();
		Row[1]=Row[2]^Row[0];		
		Row[1].Normalise();
	}
	
	// Some useful matrices
	void	MakeNull() { Row[0]=Row[1]=Row[2]=FVector(0.0f,0.0f,0.0f); };
	void	MakeID() { Row[0] = FVector(1.0f,0.0f,0.0f); Row[1] = FVector(0.0f,1.0f,0.0f); Row[2] = FVector(0.0f,0.0f,1.0f); };	

	// Trace
	void	Trace() const {};

	// look matrix
	//!! Optimise this
	void	BuildLookMatrix(FVector v)
	{
		v.Normalise();  // is this needed?

		SINT	axis;
		float	max;

		// Find the largest component of the surface normal
		max = (float)fabs(v.X);
		axis = 0;
		if (fabs(v.Y) > max)
		{
			axis = 1;
			max = float(fabs(v.Y));
		}
		if (fabs(v.Z) > max)
			axis = 2;

		FVector	vec1,vec2;

		switch( axis)
		{
		case 0:
			// Its X, so cross with Y
			vec1 = FVector( 0, 0, 1) ^ v;
			vec1.Normalise();
			vec2 = v ^ vec1;
			vec2.Normalise();
			break;

		case 1:
			// Its Y, so ok to use X
			vec1 = FVector( 1, 0, 0) ^ v;
			vec1.Normalise();
			vec2 = v ^ vec1;
			vec2.Normalise();
			break;

		case 2:
			// Its Z, so ok to use X
			vec1 = FVector( 1, 0, 0) ^ v;
			vec1.Normalise();
			vec2 = v ^ vec1;
			vec2.Normalise();
			break;
		}

		// Build matrix from vectors
		FVector	newnormal = -v;
//		newnormal.Z = -newnormal.Z;

//		mOrientation = FMatrix( vec2, vec1, newnormal);
		Row[0] = vec2;
		Row[1] = vec1;
		Row[2] = newnormal;

		TransposeInPlace();
	}

/*	void	BuildLookMatrix(FVector v)
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

	void	BuildLookMatrix(FVector v, FVector l)  // assumes l is normalised
	{
		v.Normalise();

		FVector r =v ^ l;

		if (r.MagnitudeSq() < 0.0001)
		{
			Row[0] = FVector(1.0f,0.0f,0.0f);
			Row[1] = FVector(0.0f,1.0f,0.0f);
			Row[2] = FVector(0.0f,0.0f,1.0f);
			return;
		}
		r.Normalise();

		// calculate angle
		float	dot = v * l;
		if (dot > 1.f)
			dot = 1.f;
		if (dot < -1.f)
			dot = -1.f; // Hmmm...

		float theta = float(acos(dot));  // mag v * mag l which are both 1

		MakeGeneralAxisRotationF(r, -theta);
	}
};

//****************************************************************************************************

const	FVector	ZERO_FVECTOR(0.0f,0.0f,0.0f);
//const	FMatrix	ID_FMATRIX(FVector(1.0f,0.0f,0.0f),FVector(0.0f,1.0f,0.0f),FVector(0.0f,0.0f,1.0f));
#define ID_FMATRIX (FMatrix(FVector(1.0f,0.0f,0.0f),FVector(0.0f,1.0f,0.0f),FVector(0.0f,0.0f,1.0f)))

//****************************************************************************************************
#endif
