#include	"Common.h"

#include	"Globals.h"
#include	"Golf.h"


//******************************************************************************************
void	CGolfArrow::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	Align();
}

//******************************************************************************************
void	CGolfArrow::Process()
{
	Align();

	SUPERTYPE::Process();
}

//******************************************************************************************
void	CGolfArrow::Align()
{
	// align the arrow to the ball.
	//! should know which player I belong to!

	GVector		v = WORLD.GetPrimaryPosition() - PLAYER(0)->GetAnalogCursor();
	v.Z = G0;
	GINT	dist;
	dist = v.Magnitude();
//	v.Normalise();
	v /= dist;
	
	dist *= G(0, 40000);
	dist += G1;

//	Move(WORLD.GetPrimaryPosition() + v * G(2)/* + GVector(G0, G0, -G1)*/);
	Move(WORLD.GetPrimaryPosition() + v * dist/* + GVector(G0, G0, -G1)*/);

	mOrientation.MakeRotationYawSC(-v.X, v.Y);

}

//******************************************************************************************
GINT	CGolfArrow::GetRenderScale()
{
	GINT	size;
	if (PLAYER(0)->GetPickupBeamState() != NO_BEAM)
		size = PLAYER(0)->GetPickupBeamSize();
	else
		size = G0;
		
	return (size * G(0, 26214) / 10) + G(0, 52428);  // 0.4 & 0.8
//	return (GSIN(G(GAME.GetGameTurn()) / 5) / 10) + G1;
}

//******************************************************************************************
DWORD	CGolfArrow::GetRenderColour()
{
	GINT	size;
	if (PLAYER(0)->GetPickupBeamState() != NO_BEAM)
		size = PLAYER(0)->GetPickupBeamSize();
	else
		size = G0;
		
	SINT	val = (size * 18).Whole() + 0;  //goes from 0 - 10
	
	return (val << 16) + (val << 8) + val;
}
