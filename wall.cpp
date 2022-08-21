#include	"Common.h"

#include	"Wall.h"
#include	"Globals.h"

/*
//******************************************************************************************
void	CWall::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();

//	mSpringArray = new CSpringArray(12, 3, 3);
//	mSpringArray = new CSpringArray(6, 3, 3);
//	ASSERT(mSpringArray);
}

//******************************************************************************************
void	CWall::Shutdown()
{
//	if (mSpringArray)
//	{
//		delete mSpringArray;
//
//		mSpringArray = NULL;
//	}

	SUPERTYPE::Shutdown();
}

//******************************************************************************************

#define WALL_FORCE GINT(0, 39321)

void	CWall::Process()
{
//	SINT t = GAME.GetGameTurn();
//
//	if ((t & 0x7f) < 15)
//	{
//		GVector force = GVector(G0, -WALL_FORCE, G0);
//		if (t & 0x80)
//			force = -force;
//
//		mSpringArray->AddForce(5, 0,  0, force);
//		mSpringArray->AddForce(5, 0,  1, force);
//		mSpringArray->AddForce(5, 0,  2, force);
//		mSpringArray->AddForce(6, 0,  0, force);
//		mSpringArray->AddForce(6, 0,  1, force);
//		mSpringArray->AddForce(6, 0,  2, force);
//		mSpringArray->AddForce(5, 1,  0, force);
//		mSpringArray->AddForce(5, 1,  1, force);
//		mSpringArray->AddForce(5, 1,  2, force);
//		mSpringArray->AddForce(6, 1,  0, force);
//		mSpringArray->AddForce(6, 1,  1, force);
//		mSpringArray->AddForce(6, 1,  2, force);
//		mSpringArray->AddForce(5, 2,  0, force);
//		mSpringArray->AddForce(5, 2,  1, force);
//		mSpringArray->AddForce(5, 2,  2, force);
//		mSpringArray->AddForce(6, 2,  0, force);
//		mSpringArray->AddForce(6, 2,  1, force);
//		mSpringArray->AddForce(6, 2,  2, force);
//	}

#if TARGET == PC
	//!PSX   rather slow....
	mSpringArray->Process();
#endif

	SUPERTYPE::Process();
}
*/
