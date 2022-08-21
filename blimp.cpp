#include	"Common.h"

#if 0

#include	"Globals.h"
#include	"Blimp.h"

//******************************************************************************************
void	CBlimp::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mYaw		= G0;
	mBladeOri	= ID_GMATRIX;
	mEngineOri	= ID_GMATRIX;

	SitOnGround();  // for the editor.
}

//******************************************************************************************
#define BLIMP_SPEED				G(0, 8000)
#define BLIMP_HEIGHT			(-(G(6)))
#define BLIMP_ANGULAR_ROTATION	G(0, 900)
#define BLIMP_BLADE_ROTATION	G(0, 50000)

void	CBlimp::Process()
{
	mYaw += BLIMP_ANGULAR_ROTATION;
	mOrientation.MakeRotationYaw(mYaw);

	GINT hd = BLIMP_HEIGHT + MAP.GetMapHeight(mPos) - mPos.Z;
	mVelocity = mOrientation * GVector(G0, BLIMP_SPEED, hd * G(0, 20000));

	GMatrix	m;
	m.MakeRotationRoll(BLIMP_BLADE_ROTATION);
	mBladeOri = mBladeOri * m;
	mBladeOri.Normalise();

	m.MakeRotationPitch(BLIMP_BLADE_ROTATION / 30);
	mEngineOri = mEngineOri * m;
	mEngineOri.Normalise();

	SUPERTYPE::Process();
}

//******************************************************************************************
GMatrix	CBlimp::GetRenderOrientation(SINT no)
{
	switch (no)
	{
	case 1:
	case 3:
		return mEngineOri;
	case 2:
	case 4:
		return mBladeOri;
	default:
		return SUPERTYPE::GetRenderOrientation(no);
	};
}

//******************************************************************************************
//******************************************************************************************
void	CFlyDroid::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mYaw		= G0;
	mFan1Ori	= ID_GMATRIX;
	mFan2Ori	= ID_GMATRIX;
	mRadarOri	= ID_GMATRIX;

	SitOnGround();  // for the editor.
}

//******************************************************************************************
#define FLYDROID_SPEED				G(0, 8000)
#define FLYDROID_HEIGHT				(-(G(9)))
#define FLYDROID_ANGULAR_ROTATION	G(0, 900)

//#define FLYDROID_FAN_ROTATION		G(0, 12353)
#define FLYDROID_FAN_ROTATION		G(0, 17000)
#define FLYDROID_RADAR_ROTATION		G(0, 4118)

void	CFlyDroid::Process()
{
	mYaw += FLYDROID_ANGULAR_ROTATION;
	mOrientation.MakeRotationYaw(mYaw);

	GINT hd = FLYDROID_HEIGHT + MAP.GetMapHeight(mPos) - mPos.Z;
	mVelocity = mOrientation * GVector(G0, FLYDROID_SPEED, hd * G(0, 20000));

	GMatrix	m;
	m.MakeRotationYaw(FLYDROID_FAN_ROTATION);
	mFan1Ori = mFan1Ori * m;
	mFan1Ori.Normalise();
	mFan2Ori = mFan2Ori * m.Transpose();
	mFan2Ori.Normalise();

	m.MakeRotationYaw(FLYDROID_RADAR_ROTATION);
	mRadarOri = mRadarOri * m;
	mRadarOri.Normalise();

	SINT	gt = GAME.GetGameTurn();

	if ((gt >= 256) && (gt < 256 + 32 * 3) && ((gt & 31) == 0))
	{
		CThing * t = SpawnThing(OID_CSoldier1);
		if (t)
		{
			CInitThing	i;
			i.mPos = mPos;
			t->Init(&i);
			t->Move(mPos + GVector(G0, G0, G(2))); // because init does SitOnGround()
		}
	}


	SUPERTYPE::Process();
}

//******************************************************************************************
GMatrix	CFlyDroid::GetRenderOrientation(SINT no)
{
	switch (no)
	{
	case 4:		return mFan1Ori;
	case 5:		return mFan2Ori;
	case 6:		return mRadarOri;
	default:	return SUPERTYPE::GetRenderOrientation(no);
	};
}

//******************************************************************************************
#ifdef LINK_EDITOR

void	CFlyDroid::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CSoldier1);
}
#endif

#endif