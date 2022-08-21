#include	"Common.h"

#if TARGET == PSX

#include	"PSXFrontEndThing.h"
#include	"Globals.h"
#include	"FrontEndCommon.h"

//(JCL) this has got a bit hacked to support both characters and flags...

//******************************************************************************************
void	CFrontEndThing::Reset(SINT c)
{
	mYaw = G0;
	mChar = c;
	mWhite = FALSE;
	mBlack = FALSE;
	
	if (c < 6)
		SetAnimMode(AM_FRONT_END1);	  // character
	else
	{
		SetAnimMode(AM_WALK);		  // flag
		mFrame = (G(c - 6)) / 5; 	  // offset animations
	}

	mPos = ZERO_GVECTOR;
	mOrientation = ID_GMATRIX;
}

//******************************************************************************************
void	CFrontEndThing::SetAnimMode(EAnimMode am)
{
	mAnimMode = am;
	mFrame = G0;

	CPSXMesh *m;
	m = ENGINE.GetNumberedMesh(FEPSM_ANGEL + mChar);
	if (m)
	{
		mFrameInc = m->GetFrameIncrement(mAnimMode, &mAnimIndex);
	};
}

//******************************************************************************************
void	CFrontEndThing::Process()
{
	// rotate?
	switch (mAnimMode)
	{
	case AM_FRONT_END2:
		if (mFrame != G1)
			mYaw = G_PI - ((G_PI - mYaw) * G(0, 50000));
		else
			mYaw += G(0, 2500);
		break;
		
	case AM_WALK:
		break; // do nothing..

	default:
		mYaw += G(0, 2500);
		break;
	};
	mOrientation.MakeRotationYaw(mYaw);

	// animate
	mFrame += mFrameInc;

	// animation done?
	if (mFrame > G1)
	{
		switch (mAnimMode)
		{
		case AM_FRONT_END2:
//			mFrame -= G1;
//			SetAnimMode(AM_FRONT_END1);
			mFrame = G1;
			break;
		case AM_FRONT_END1:
			mFrame -= G1;
			if ((rand() & 7) == 0)
				SetAnimMode(AM_FRONT_END3);
			break;
		case AM_FRONT_END3:
			SetAnimMode(AM_FRONT_END1);
			break;
		default:
			mFrame -= G1;
			break;
		}
	};
}

//******************************************************************************************
void	CFrontEndThing::Select()
{
	SetAnimMode(AM_FRONT_END2);

	while (mYaw > G_2PI)
		mYaw -= G_2PI;
}



#endif
