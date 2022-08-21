#include	"Common.h"

#include	"Globals.h"
#include	"Torso.h"

//******************************************************************************************
void	CTorso::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	if (i->mAngleType == AT_YPR)
	{
		mYaw = i->mYaw;
		mPitch = i->mPitch;
		mRoll = i->mRoll;
	}
	else
	{
		mYaw = G0;
		mPitch = G0;
		mRoll = G0;
	}

	mDestYaw = mYaw;
//	mDestPitch = mPitch;

	if (mPitch < G0)
	{
		mDestPitch = -G_PI_2;
	}
	else
	{
		mDestPitch = G_PI_2 ;
	}
	mDestRoll = mRoll;
	mBodyFlamer = NULL ;
	mSpinamount = G(0,40000) ;
	mSpin = FALSE ;
}

//******************************************************************************************
void	CTorso::Process()
{
	// ****************************

	// handle rotation

	// ****************************




	// yaw
	GINT	delta_yaw = mDestYaw - mYaw;
	if (mSpin)
	{
		mDestYaw+=mSpinamount ;
		mSpinamount*=G(0,60000) ;
	}

	//	while (delta_yaw > G_PI)
	//		delta_yaw -= G_PI * 2;
	//
	//	while (delta_yaw < -G_PI)
	//		delta_yaw += G_PI * 2;


	GINT	delta_yaw2 = delta_yaw;
	mYaw += delta_yaw2 * G(0, 5000);

	if (mYaw > G_PI)
	{
		mYaw		-= G_PI * 2;
		mDestYaw	-= G_PI * 2;
	}
	if (mYaw < -G_PI)
	{
		mYaw		+= G_PI * 2;
		mDestYaw	+= G_PI * 2;
	}



	// if not knocked out or dead then update pitch value.
	// Note: if it is knocked out or dead then the state function
	// takes over pitch control.

	// ****************************
	// Pitch
	GINT	delta_pitch = mDestPitch - mPitch;

	while (delta_pitch > G_PI)
		delta_pitch -= G_PI * 2;

	while (delta_pitch < -G_PI)
		delta_pitch += G_PI * 2;

	GINT	delta_pitch2 = delta_pitch;
	mPitch += delta_pitch2 * G(0, 5000);


	if (mPitch > G_PI)
		mPitch -= G_PI * 2;
	if (mPitch < -G_PI)
		mPitch += G_PI * 2;
	
	
	// ****************************
	// Roll
	GINT	delta_roll = mDestRoll - mRoll;

	while (delta_roll > G_PI)
			delta_roll -= G_PI * 2;

	while (delta_roll < -G_PI)
			delta_roll += G_PI * 2;

	GINT	delta_roll2 = delta_roll;
	mRoll += delta_roll2 * G(0, 5000);

	if (mRoll > G_PI)
		mRoll -= G_PI * 2;
	if (mRoll < -G_PI)
		mRoll += G_PI * 2;


	// and build the matrices
	GMatrix foo;

	// pitch
	mOrientation.MakeRotationPitch(mPitch);

	// roll
	foo.MakeRotationRoll(mRoll);
	mOrientation = foo * mOrientation;

	// yaw
	foo.MakeRotationYaw(mYaw);
	mOrientation = foo * mOrientation;

	//********************************************************************
	//** Other Stuff

	mLife --;

	SUPERTYPE::Process();

	// hm.... actor clears this flag :(
	
	if (mLife < 30) mFlags |= TF_FLASHING;
	

	if (mBodyFlamer && mLife < 20 )
	{
		mBodyFlamer->Die() ;
		mBodyFlamer = NULL ;
	}


	// move flames if we are on fire 
	if (mBodyFlamer)
	{
		// keep it flamming if allive
		mBodyFlamer->SetParticleFrequency(5) ;
		GVector pos = mPos  + GVector(G0,G0,-G(0,15433)) ;
		mBodyFlamer->MoveStreamerAndParticles(&pos);
	}
}

