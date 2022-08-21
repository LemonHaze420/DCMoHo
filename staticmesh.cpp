#include	"Common.h"

#include	"StaticMesh.h"
#include	"Globals.h"

//******************************************************************************************
void	CStaticMesh::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();
}

//******************************************************************************************
void	CStaticMesh::Process()
{
	// rotate a bit
//	mYaw += 0.05f;

//	mOrientation.MakeRotationYawF(mYaw);

//	StickToGround();  // nyet!
#if TARGET == PSX
	mPos.Z = MAP.GetMapHeightDifference(mPos) ;
#endif
	SUPERTYPE::Process();
}



//******************************************************************************************
void	CTVCamera::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);
	if (i->mAngleType == AT_YPR)
		mYaw = i->mYaw;
	else		
		mYaw = G0;

	mPitch = G0 ;

	mOrientation2.MakeRotationYaw(mYaw);
}

//******************************************************************************************
void	CTVCamera::Process()
{

	GVector d = (WORLD.GetPrimaryPosition() - (mPos+GVector(G0,G0,-G(6))));

	GINT	v = d.MagnitudeXY();


	// do yaw ;

	GINT RequiredYaw = GATAN2(d.Y, d.X) - G_PI_2;

	if (GABS(AngleDifference(mYaw, RequiredYaw)) < G(0,3000) )
	{
		mYaw = RequiredYaw;
	}
	else
	{
		// go left or right ?
		GVector otho1 = mOrientation2 * GVector(G1,G0,G0)  ;
		GINT dot = d * otho1 ;

		if (dot >=0)
		{
			mYaw-=G(0,3000) ;
			if (mYaw < -G_PI) mYaw+=(G_PI*2); 
		}
		else
		{
			mYaw+=G(0,3000) ;
			if (mYaw > G_PI) mYaw-=(G_PI*2); 
		}
	}

	// do pitch

	GINT RequiredPitch = GATAN2(v, -d.Z) - G_PI_2;

	if (GABS(RequiredPitch) < (G_PI / 4))
	{
		if (GABS(RequiredPitch-mPitch) < G(0,1000) )
		{
			mPitch = RequiredPitch ;
		}
		else
		{
			if (RequiredPitch > mPitch)
			{
				mPitch+=G(0,1000) ;
			}
			else
			{
				mPitch-=G(0,1000) ;
			}
		}
	}

	mOrientation2.MakeRotation(mYaw, mPitch, G0);




/*


	GVector d = -(WORLD.GetPrimaryPosition() - mPos);

	GINT	v = d.MagnitudeXY();
	if (v > GINT(0, 100))
	{
		GINT	sn =  d.X / v;
		GINT	cs = -d.Y / v;  // Meshes face wrong way in max....

		mOrientation2.MakeRotationYawSC(sn, cs);
	}*/


	SUPERTYPE::Process();
}

