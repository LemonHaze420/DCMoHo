
#include "Common.h"
#include "Globals.h"
#include "SeedUniformRandomNumberStream.h"


SINT CSeedUniformRandomNumberStream::mA=48271;
SINT CSeedUniformRandomNumberStream::mM=214783647;


//******************************************************************************************
CSeedUniformRandomNumberStream::CSeedUniformRandomNumberStream(SINT initial_seed)
{
    mInitialSeed = initial_seed ;
    mCurrentSeed = initial_seed ;
}


//******************************************************************************************

SINT CSeedUniformRandomNumberStream::Next()
{
	static SINT q = mM / mA ;
	static SINT r = mM % mA ;
	SINT low ;
	SINT high ;
	SINT temp ;
	
	high = mCurrentSeed / q ;
	low = mCurrentSeed % q ;
	temp = mA * low - r * high ;
	
	if (temp > 0) 
	{
		mCurrentSeed = temp ;
	} 
	else
	{
		mCurrentSeed = temp + mM ;
	}
	
//	if (GAME.GetGameTurn() < 3)
//		printf("RANDOM() called in turn %d: result %d \n", GAME.GetGameTurn(), mCurrentSeed);
	
	if (mCurrentSeed < 0 )
	{
		return -mCurrentSeed ;
	}
	
	return mCurrentSeed ;
}

