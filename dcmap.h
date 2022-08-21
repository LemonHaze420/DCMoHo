#ifndef DCMAP_H
#define DCMAP_H

#include	"Map.h"

#define WC_NEVER	0
#define WC_ALWAYS	1
#define WC_CHECK	2
#define WC_MASK 	3

#define F_4x4_1 4
#define F_4x4_2 8
#define F_4x4_MASK 12

#define	F_4x4_ALWAYS4	16
#define	F_4x4_NEVER		32

#define	F_OTHER_MASK	(32 + 16)


class   CDCMap : public CMap
{
	// NO VIRTUAL FUNCTIONS!!!!!!!
public:
	BOOL		Init();
	BOOL		Resize(SINT xs, SINT ys);

	void		InitPlatform(void)			{};
	void		ShutdownPlatform(void)		{};

	void 		InitMappy();

	// Rendering
    void		DrawPickupBeam(CPlayer *pl);
	void		DrawLaserBeam() {};
	void		DrawMapWhoDebug() {};
	void		Render();
	void 		DcRender4x4(int x,int y);
	void 		DcRender2x2(int x,int y);
	void 		DcRender1x1(int x,int y);
	void 		DcRender4x3(int x,int y);	
	void 		DcRender3x4(int x,int y);	
	void 		DcRender3x3(int x,int y);	
	void 		CalcDeltaColours(void);	
    void		Render4x4();
    void		Render2x2();
	void		DrawVisible4x4s();
	void		DrawShadow(IRenderableThing *t, STextureRefOther *tr);
	BOOL		Read(CMEMBUFFER &mem);
	struct	S4x4
	{
		 unsigned char 	tex1,tex2,tex3;
		 unsigned char	mFlags;
		 SLONG		mMinHeight;
		 SWORD		mMaxHeight;
		 //unsigned char mMix[5][5];
	}Texnums[32*16];

	void		PostLoadProcess(); // setup water height check etc..

	void		SetRowWCCheckAndMaxHeight(SINT r, GINT h);
	void		SetRowWCAlwaysAndMinHeight(SINT r, GINT h);

	void		ClearOtherFlags();
	void		SetOtherFlags(const CCellRef &c, SINT flags);
	
	void		AddWaterParticles(SINT	start_x, SINT start_y);

	struct	SVert
	{
		FVector	mPos;
		float	mU, mV;
	};

	void		CDSQ(STextureRefOther *tr, const FVector &p1, const FVector &p2, const FVector &p3,
				 	 float sx, float sy, float ex, float ey);
	void		ClipAndDrawShadowQuad(STextureRefOther *tr, float sx, float sy, float ex, float ey);

	
	void		CDSP(STextureRefOther *tr, FVector p1, FVector p2, FVector p3, SVert v1, SVert v2, SVert v3);
	void		ClipAndDrawShadowPoly(STextureRefOther *tr, SVert v1, SVert v2, SVert v3);
	
	void		DrawCellType1(SINT x, SINT y, SINT type);
	void		DrawCellType2(SINT x, SINT y, SINT type);	
	
	void		RenderWater();
	
	void		TransformAll();
	void		TransformSome(SINT sx, SINT sy, SINT nx, SINT ny, SINT stride);

private:
};



#endif




