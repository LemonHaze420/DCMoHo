#ifndef UTILS2_H
#define UTILS2_H

// random useful floating point functions

inline	void	GVectorToFVector(const GVector *g, FVector *f)
{
	f->X = GToF(g->X);
	f->Y = GToF(g->Y);
	f->Z = GToF(g->Z);
}

inline	void	FVectorToGVector(const FVector *f, GVector *g)
{
	g->X = FToG(f->X);
	g->Y = FToG(f->Y);
	g->Z = FToG(f->Z);
}

inline	FVector	GVectorToFVector(const GVector &g)
{
	FVector f;

	f.X = GToF(g.X);
	f.Y = GToF(g.Y);
	f.Z = GToF(g.Z);

	return f;
}

inline	GVector	FVectorToGVector(const FVector &f)
{
	GVector g;

	g.X = FToG(f.X);
	g.Y = FToG(f.Y);
	g.Z = FToG(f.Z);

	return g;
}

inline	void	GMatrixToFMatrix(const GMatrix *g, FMatrix *f)
{
	GVectorToFVector(&g->Row[0], &f->Row[0]);
	GVectorToFVector(&g->Row[1], &f->Row[1]);
	GVectorToFVector(&g->Row[2], &f->Row[2]);
}

inline	FMatrix	GMatrixToFMatrix(const GMatrix &g)
{
	FMatrix f;
	GVectorToFVector(&g.Row[0], &f.Row[0]);
	GVectorToFVector(&g.Row[1], &f.Row[1]);
	GVectorToFVector(&g.Row[2], &f.Row[2]);

	return f;
}

inline	GMatrix	FMatrixToGMatrix(const FMatrix &f)
{
	GMatrix g;
	FVectorToGVector(&f.Row[0], &g.Row[0]);
	FVectorToGVector(&f.Row[1], &g.Row[1]);
	FVectorToGVector(&f.Row[2], &g.Row[2]);

	return g;
}

inline	float	FABS(float a) 
{
	return (a > 0) ? a : -a;
}


#endif