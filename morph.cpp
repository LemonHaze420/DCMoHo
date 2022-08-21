#include	"Common.h"

#include	"Morph.h"
#include	"Globals.h"

#define 	PLAY_RUMBLE		SOUND.PlaySample(SOUND_RUMBLE, this, G1, TRUE, TRUE, G(0, 30000))
#define 	FADE_RUMBLE		SOUND.FadeTo(SOUND_RUMBLE, G0, G(0, 3000), this)

//******************************************************************************************
//******************************************************************************************
void	CMorph::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mLife = 0;
	VERIFY_COORDS(CCellRef(mPos));
	
	PLAY_RUMBLE;
}

//******************************************************************************************
//******************************************************************************************
void	CMPBomb::Process()
{
	SINT	c0, c1;
	SINT	px = mPos.X.Whole();
	SINT	py = mPos.Y.Whole();
	SINT	size = 5;

	GINT	mag = G0;

	if (mLife < 10)
		mag = G(0, 19660);
//	else if (mLife > 130)
//		mag = -G(0, 19660);

	if (mag != G0)
	{
		for (c0 = px - size; c0 <= px + size; c0 ++)
			for (c1 = py - size; c1 <= py + size; c1 ++)
			{
				if ((c0 > 0) && (c1 > 0) && (c0 < MAP_WIDTH) && (c1 < MAP_HEIGHT))
				{
					GINT val = GCOS(G(ABS(c0 - px)) / G(size) * G_PI / G(2)) * 
							   GCOS(G(ABS(c1 - py)) / G(size) * G_PI / G(2));

					if (val > G0)
					{
						val *= mag;
						MAP.GetCell(CCellRef(c0, c1))->AddDisplacement(val);
					}
				}
			}
	}

	mLife++;
	
	if (mLife == 10)
		FADE_RUMBLE;

	if (mLife == 141)
		Suicide();

	SUPERTYPE::Process();
}

//******************************************************************************************
//******************************************************************************************

void	CMPAverage::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	GINT	cell_weight = G0;
	mAverageHeight = G0;

	SINT c0, c1;
	SINT	px = mPos.X.Whole();
	SINT	py = mPos.Y.Whole();

	for (c0 = px - MPAVERAGE_SIZE; c0 <= px + MPAVERAGE_SIZE; c0 ++)
		for (c1 = py - MPAVERAGE_SIZE; c1 <= py + MPAVERAGE_SIZE; c1 ++)
		{
			if ((c0 > 0) && (c1 > 0) && (c0 < MAP_WIDTH) && (c1 < MAP_HEIGHT))
			{
				GINT val = GCOS(G(ABS(c0 - px)) / G(MPAVERAGE_SIZE) * G_PI / G(2)) * 
						   GCOS(G(ABS(c1 - py)) / G(MPAVERAGE_SIZE) * G_PI / G(2));

				if (val > G0)
				{
					cell_weight += val;
					GINT h = MAP.GetCell(CCellRef(c0, c1))->GetUnderlyingHeight();
					mAverageHeight += (h) * val;
				}
			}
		}

	mAverageHeight /= cell_weight;
	
	PLAY_RUMBLE;
}

//******************************************************************************************
#define	MPAVERAGE_TIME	70

void	CMPAverage::Process()
{
	SINT	c0, c1;
	SINT	px = mPos.X.Whole();
	SINT	py = mPos.Y.Whole();

	GINT	mag = G0;

	if (mLife < 10)
		mag =  G(0, 6554);
//	else if (mLife > MPAVERAGE_TIME - 11)
//		mag = -G(0, 6554);

	if (mag != G0)
	{
		for (c0 = px - MPAVERAGE_SIZE; c0 <= px + MPAVERAGE_SIZE; c0 ++)
			for (c1 = py - MPAVERAGE_SIZE; c1 <= py + MPAVERAGE_SIZE; c1 ++)
			{
				if ((c0 > 0) && (c1 > 0) && (c0 < MAP_WIDTH) && (c1 < MAP_HEIGHT))
				{
					GINT val = GCOS(G(ABS(c0 - px)) / G(MPAVERAGE_SIZE) * G_PI / G(2)) * 
							   GCOS(G(ABS(c1 - py)) / G(MPAVERAGE_SIZE) * G_PI / G(2));

					if (val > G0)
					{
						val *= mag;

						GINT v = MAP.GetCell(CCellRef(c0, c1))->GetUnderlyingHeight();
						GINT d = mAverageHeight - v;
						d *= val;

						MAP.GetCell(CCellRef(c0, c1))->AddDisplacement(d);
					}
				}
			}
	}

	mLife++;

	if (mLife == 10)
		FADE_RUMBLE;

	if (mLife == MPAVERAGE_TIME)
		Suicide();

	SUPERTYPE::Process();
}

//******************************************************************************************

//******************************************************************************************
//******************************************************************************************

void	CMPLower::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mLowerHeight = GINT_MIN;

	SINT c0, c1;
	SINT	px = mPos.X.Whole();
	SINT	py = mPos.Y.Whole();

	for (c0 = px - MPLOWER_SIZE; c0 <= px + MPLOWER_SIZE; c0 ++)
		for (c1 = py - MPLOWER_SIZE; c1 <= py + MPLOWER_SIZE; c1 ++)
		{
			if ((c0 > 0) && (c1 > 0) && (c0 < MAP_WIDTH) && (c1 < MAP_HEIGHT))
			{
				GINT val = GCOS(G(ABS(c0 - px)) / G(MPLOWER_SIZE) * G_PI / G(2)) * 
						   GCOS(G(ABS(c1 - py)) / G(MPLOWER_SIZE) * G_PI / G(2));

				if (val > G0)
				{
					GINT	h = MAP.GetCell(CCellRef(c0, c1))->GetUnderlyingHeight();
					if (mLowerHeight < h)
						mLowerHeight = h;
				}
			}
		}

	PLAY_RUMBLE;
}

//******************************************************************************************
#define	MPLower_TIME	140

void	CMPLower::Process()
{
	SINT	c0, c1;
	SINT	px = mPos.X.Whole();
	SINT	py = mPos.Y.Whole();

	GINT	mag = G0;

	if (mLife < 10)
		mag =  G(0, 6554);
//	else if (mLife > MPLower_TIME - 11)
//		mag = -G(0, 6554);

	if (mag != G0)
	{
		for (c0 = px - MPLOWER_SIZE; c0 <= px + MPLOWER_SIZE; c0 ++)
			for (c1 = py - MPLOWER_SIZE; c1 <= py + MPLOWER_SIZE; c1 ++)
			{
				if ((c0 > 0) && (c1 > 0) && (c0 < MAP_WIDTH) && (c1 < MAP_HEIGHT))
				{
					GINT val = GCOS(G(ABS(c0 - px)) / G(MPLOWER_SIZE) * G_PI / G(2)) * 
							   GCOS(G(ABS(c1 - py)) / G(MPLOWER_SIZE) * G_PI / G(2));

					if (val > G0)
					{
						val *= mag;

						GINT v = MAP.GetCell(CCellRef(c0, c1))->GetUnderlyingHeight();
						GINT d = mLowerHeight - v;
						d *= val;

						MAP.GetCell(CCellRef(c0, c1))->AddDisplacement(d);
					}
				}
			}
	}

	mLife++;

	if (mLife == 10)
		FADE_RUMBLE;

	if (mLife == MPLower_TIME)
		Suicide();

	SUPERTYPE::Process();
}

//******************************************************************************************
