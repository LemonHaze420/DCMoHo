#ifndef PSXMAP_H
#define PSXMAP_H

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


class   CPSXMap : public CMap
{
	// NO VIRTUAL FUNCTIONS!!!!!!!
public:
	BOOL		Init();
	BOOL		Resize(SINT xs, SINT ys);

	void		InitPlatform(void)			{};
	void		ShutdownPlatform(void)		{};

	// Rendering
    void		DrawPickupBeam(CPlayer *pl);
	void		DrawLaserBeam() {};
	void		DrawMapWhoDebug() {};
	void		Render();
    void		Render4x4();
    void		Render2x2();
	void		DrawVisible4x4s();
	void		DrawShadow(IRenderableThing *t);
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

private:
};



#endif




