// GameLevelData.cpp: implementation of the CGameLevelData class.
//
//////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Globals.h"
#include "GameLevelData.h"
#include "Primary.h"
#include "DroidBall.h"

// Generic parent part 

//******************************************************************************************
void	CGameLevelData::Init() 
{
	mCharsOnHill.Clear() ;
	mLastKingHillRefreshTime = 0 ;
	for (SINT i=0;i<MAX_CHARS_IN_WORLD;i++) mRacePositionResults[i] = NULL;
	mCurrentRaceResultIndex = 0 ;
	mCurrentAllow10SecondsCount = 0 ;

	mCharsInGame = 0 ;
	mStartCharsInGame = 0 ;
	mNumberOfTagPrimarysDead = 0;

}


//******************************************************************************************
void	CGameLevelData::TimeHasRunOut() 
{
	// child part
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_TAG && GAME.IsMultiPlayer() )
	{
		TimeHasRunOutTagMultiPlayer() ;
		return ;
	}


	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_DROID_BALL )
	{
		TimeHasRunOutDroidBall() ;
		return ;
	}

	
	// parent part

	// game lose for eveyone !!! 
	for (SINT i =0;i<MAX_PLAYERS;i++)
	{
		if (PLAYER(i)->GetPrimary()) 
		{
			if (GAME.IsMultiPlayer())
			{
				PLAYER(i)->SetLevelResult(LR_DRAW) ;
			}
			else
			{
				PLAYER(i)->SetLevelResult(LR_LOSE) ;
			}
			// used so finsihed ground friction kicks in 
			PLAYER(i)->GetPrimary()->ChangeState(CS_FINISHED, CStateInit(0)) ;
		}
	}
	GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
}


//******************************************************************************************
void	CGameLevelData::ProcessLevel() 
{
	// check if timer has run out

	// parent part
	SINT	tl = GAME.GetLevelData()->mIVars[IV_TIME_LIMIT];
	if (tl > 0 )
	{
		SINT 	turns_taken = GAME.GetGameTimeSinceStart();
		SINT	time_left = tl - (turns_taken / 25);
		if(time_left <= 0)
		{
			TimeHasRunOut();
			SOUND.PlaySample(SOUND_FOGHORN);
		}
		else if (time_left * 25 == (tl * 25) - turns_taken)
		{
			SINT	sample = -1;
			switch (time_left)
			{
			case 10:
			case 8:
			case 6:
			case 4:
			case 2:
//				sample = SOUND_TIMER;
				break;
			};
			if (sample != -1)
				SOUND.PlaySample(sample);
		}
	}

	// child part

	switch (GAME.GetIVar(IV_LEVEL_TYPE))
	{
	case LT_NORMAL:
	case LT_DIRECT:
	case LT_GAUNTLET:
	case LT_PURSUIT:
		{
			ProcessDirectOrPursuit() ;
			break ;
		}
	case LT_KING:
	case LT_RACER: 
		{
			ProcessRace() ;
			break ;
		}
	}
}

//******************************************************************************************
void	CGameLevelData::CharacterFinishedLevel(CCharacter* character) 
{
	// this would normally be a virtual fuction but no 'new' 'delete' means 
	// it's a all-in-one type class. 

	// parent part
	if (GAME.GetGameState() != GAME_STATE_RUNNING) return ;

	// child part


	switch (GAME.GetIVar(IV_LEVEL_TYPE))
	{
	case LT_TAG:
	case LT_DROID_BALL:
		{
			// multiplayer ends by timeout
			if (GAME.IsMultiPlayer())
			{
				ASSERT(0) ;
			}
			PLAYER(0)->SetLevelResult(LR_WIN) ;
			GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
			break ;
		}

	case LT_KING:
		{
			CharacterFinishedKingHill(character) ;
			break ;
		}

	case LT_RACER: 
		{
			CharacterFinishedRace(character) ;
			break ;
		}
	case LT_NORMAL:
	case LT_DIRECT:
	case LT_GAUNTLET:
	case LT_PURSUIT:
		{
			CharacterFinishedDirectOrPursuit(character) ;
			break ;
		}

	case LT_GOLF:
		{
			if (GAME.IsMultiPlayer())
			{
				ASSERT(0) ;
			}
			PLAYER(0)->SetLevelResult(LR_WIN) ;
			GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
			break ;
		}
	default:
		{
			CONSOLE.AddString("Ay up don't kno what to do when char is dead") ;
			GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
			break ;
		}

	}
}




//******************************************************************************************
void	CGameLevelData::CharacterHasDied(CCharacter* character) 
{


	// parent part
	if (GAME.GetGameState() != GAME_STATE_RUNNING) return ;

	mCharsInGame-- ;

	// child part

	switch (GAME.GetIVar(IV_LEVEL_TYPE))
	{
	case LT_LASTMAN:
	case LT_KING:
		{
			CharacterHasDiedKingHill(character) ;
			break ;
		}

	case LT_TAG:
		{
			CharacterHasDiedTag(character) ;
			break ;
		}
	case LT_DROID_BALL:
		{
			CharacterHasDiedDroidBall(character) ;
			break ;
		}

	case LT_NORMAL:
	case LT_DIRECT:
	case LT_GAUNTLET:
	case LT_PURSUIT:
		{
			CharacterHasDiedDirectOrPursuit(character) ;
			break ;
		}

	case LT_RACER: 
		{
			CharacterHasDiedRacer(character) ;
			break ;
		}
	}
}

//******************************************************************************************
void CGameLevelData::DeclareCharsThatWillAppearInThisGame()
{
	mStartCharsInGame = mCharsInGame = WORLD.GetCharacterNB().Size() ;
}


// child functionality 

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//  RACER STUFF

//******************************************************************************************
void	CGameLevelData::ProcessRace()
{
	// ok multiplayer shit !!!
	// if character has finished
	if (mCurrentAllow10SecondsCount !=0 )
	{
		mCurrentAllow10SecondsCount+=1 ;
		if (mCurrentAllow10SecondsCount == 250)
		{
			GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
		}
	}
}


//******************************************************************************************
void	CGameLevelData::CharacterFinishedRace(CCharacter* character)
{	
	mRacePositionResults[mCurrentRaceResultIndex++] = character ;

	// multi player

	if (GAME.IsMultiPlayer() && mCurrentAllow10SecondsCount==0)
	{
		// is a racer 
		if (!character->IsPrimary() && mCurrentRaceResultIndex >0  )
		{
			// did the racer win ?
			if (mCurrentRaceResultIndex == 1)
			{
				// game drawn !!! 
				for (SINT i =0;i<MAX_PLAYERS;i++)
				{
					if (PLAYER(i)->GetPrimary()) PLAYER(i)->SetLevelResult(LR_DRAW) ;
				}
	
				GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
				return ;
			}
		}
		else
		{
			CPrimary* primary = character->IsPrimary() ;
			if (primary && mCurrentRaceResultIndex == 1 )
			{
				// game lose for eveyone else !!! 
				for (SINT i =0;i<MAX_PLAYERS;i++)
				{

					if (primary->GetPlayer() == PLAYER(i) )
					{
						PLAYER(i)->SetLevelResult(LR_WIN) ;
						CONTROLS.DeActivateContoller(i) ;
					}
					else
					{
						if (PLAYER(i)->GetPrimary()) PLAYER(i)->SetLevelResult(LR_LOSE) ;
					}
				}
			}

			// SRG change so game ends when first player finishes level
			GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
	//		mCurrentAllow10SecondsCount =1 ;
			return ;
		}
	}

	if (GAME.IsMultiPlayer() && mCurrentAllow10SecondsCount!=0 && character->IsPrimary())
	{
		GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
	}
	

	// single player

	if (!GAME.IsMultiPlayer() && character->IsPrimary())
	{
		if (mCurrentRaceResultIndex==1)
		{
			PLAYER(0)->SetLevelResult(LR_WIN) ;
		}
		else
		{
			PLAYER(0)->SetLevelResult(LR_LOSE) ;
		}
		GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
	}
}

//******************************************************************************************
void	CGameLevelData::CharacterHasDiedRacer(CCharacter * character) 
{
	if (GAME.IsMultiPlayer() && character->IsPrimary() )
	{
		// are there any other primary's still alive
		BOOL found_a_player_still_alive = FALSE ;
		for (SINT i =0;i<MAX_PLAYERS;i++)
		{
			if (PLAYER(i)->GetPrimary() &&
				(!(PLAYER(i)->GetPrimary()->GetFlags() & TF_DEAD)) )
			{
				found_a_player_still_alive = TRUE ;
			}
		}
		if ( found_a_player_still_alive == FALSE && mCurrentAllow10SecondsCount == 0)
		{
			// game drawn !!! 
			for (SINT i =0;i<MAX_PLAYERS;i++)
			{
				if (PLAYER(i)->GetPrimary()) PLAYER(i)->SetLevelResult(LR_DRAW) ;
			}
	
			GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
		}
		else
		{
			// although someone else in the game has the game already been won
			if (mCurrentAllow10SecondsCount!=0)
			{
				GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
			}
		}
	}
	

	// single player
	if (!GAME.IsMultiPlayer() && character->IsPrimary() )
	{
		PLAYER(0)->SetLevelResult(LR_LOSE) ;
		GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
	}
}



//******************************************************************************************
SINT	CGameLevelData::GetFinshedPositionForCharacter(CCharacter* character)
{	
	for (SINT i=0;i<MAX_CHARS_IN_WORLD;i++)
	{
		if (mRacePositionResults[i] == character ) return i ;
	}
	return -1 ;

}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
// KING OF THE HILL


//******************************************************************************************
void	CGameLevelData::AddCharacterToKingHill(CCharacter* new_character) 
{
	// check if character already on hill 


	CKingHillEntry* current;
	for (current = mCharsOnHill.First();
		 current != NULL ;
		 current = mCharsOnHill.Next() )
	{
		if (current->GetCharacter() == new_character)
		{
			current->SetLastFrameOnHill(GAME.GetGameTurn()) ;
			return ;
		}
	}

	mCharsOnHill.Add(CKingHillEntry(new_character, GAME.GetGameTurn()) ) ;

}


//******************************************************************************************
//  Basicaly checks that all characters stored in the list have been there is the last
//  frame or so.  else remove them from this list

void	CGameLevelData::RefreshKingHillData() 
{
	CKingHillEntry* current;
	for (current = mCharsOnHill.First();
		 current != NULL ;
		 current = mCharsOnHill.Next() )
	{
		if (current->GetLastFrameOnHill() <  GAME.GetGameTurn() -1  ||
			(current->GetCharacter() && current->GetCharacter()->GetState() == CS_DEAD) )
		{
			mCharsOnHill.Remove(current) ;
		}
		 } 

	mLastKingHillRefreshTime = GAME.GetGameTurn() ;
}


//******************************************************************************************

SINT	CGameLevelData::NumCharactersOnHill() 
{
	if (mLastKingHillRefreshTime < GAME.GetGameTurn()) RefreshKingHillData() ;
	return mCharsOnHill.Size() ;
}


//******************************************************************************************
void	CGameLevelData::CharacterHasDiedKingHill(CCharacter* character) 
{
	// multiplayer
	if (GAME.IsMultiPlayer())
	{
		// ok if primary is dead
		if (character->IsPrimary())
		{
			mNumberOfTagPrimarysDead++ ;

			// if all primarys dead then game is drawn
			if (mNumberOfTagPrimarysDead==2)
			{
				for (SINT i =0;i<MAX_PLAYERS;i++)
				{
					if (PLAYER(i)->GetPrimary()) PLAYER(i)->SetLevelResult(LR_DRAW) ;
				}	
				GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
				return ;
			}
		}

	
		// ok one person left in game 
	    if (mCharsInGame==1)
		{
			for (SINT i =0;i<MAX_PLAYERS;i++)
			{
				if (PLAYER(i)->GetPrimary())
				{
					// if a player ant dead then he must be the winner !!
					if (PLAYER(i)->GetPrimary()->GetState() != CS_DEAD)
					{
						PLAYER(i)->SetLevelResult(LR_WIN) ;
					
						//! JCL - hmmmm
						PLAYER(i)->GetPrimary()->GetLevelData().SetFinishedLevel() ;
						PLAYER(i)->GetPrimary()->ChangeState(CS_FINISHED, CStateInit()) ;
						PLAYER(i)->GetPrimary()->SetAnimMode(AM_VICTORY);
					}

					// player lost
					else
					{
						PLAYER(i)->SetLevelResult(LR_LOSE) ;
					}
				}
			}	
			GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
		}
	}

	// single player
	else
	{	
		if (mCharsInGame==1)
		{
			// ok is the player the last guy ??

			if (PLAYER(0)->GetPrimary() &&
				PLAYER(0)->GetPrimary()->GetState() != CS_DEAD)
			{
				PLAYER(0)->SetLevelResult(LR_WIN) ;
				//! JCL - hmmmm
				PLAYER(0)->GetPrimary()->GetLevelData().SetFinishedLevel() ;
				PLAYER(0)->GetPrimary()->ChangeState(CS_FINISHED, CStateInit()) ;
				PLAYER(0)->GetPrimary()->SetAnimMode(AM_VICTORY);

			}
			else
			{
				PLAYER(0)->SetLevelResult(LR_LOSE) ;
			}
			GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
		}
		else if (character->IsPrimary() )
		{
			PLAYER(0)->SetLevelResult(LR_LOSE) ;
			GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
		}
	}	
}


//******************************************************************************************
void	CGameLevelData::CharacterFinishedKingHill(CCharacter* character)
{
	// multiplayer
	if (GAME.IsMultiPlayer())
	{
		// if a non primary won everyone else drew
		if (!character->IsPrimary())
		{
			for (SINT i =0;i<MAX_PLAYERS;i++)
			{
				if (PLAYER(i)->GetPrimary()) PLAYER(i)->SetLevelResult(LR_DRAW) ;
			}	
		}
		// a player must have won
		else
		{
			for (SINT i =0;i<MAX_PLAYERS;i++)
			{
				if (PLAYER(i)->GetPrimary()) PLAYER(i)->SetLevelResult(LR_LOSE) ;
			}	
			((CPrimary*)character)->GetPlayer()->SetLevelResult(LR_WIN) ;
		}
		GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
	}
	else
	{
		// single player
		// ok if a non player has won it means the player has lost
		if (!character->IsPrimary())
		{
			PLAYER(0)->SetLevelResult(LR_LOSE) ;
		}
		else
		{
			PLAYER(0)->SetLevelResult(LR_WIN) ;
		}

		GAME.GetCamera(0)->SetWinner(character) ;
		GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
		
	}
}



//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
// GAUNLET PURSUIT DIRECT whatever

void	CGameLevelData::CharacterFinishedDirectOrPursuit(CCharacter* character)
{
	CPrimary* primary = character->IsPrimary() ;

	// multi player

	if (primary && GAME.IsMultiPlayer() && mCurrentAllow10SecondsCount==0) 
	{
		// game lose for eveyone else !!! 
		for (SINT i =0;i<MAX_PLAYERS;i++)
		{
			if (primary->GetPlayer() == PLAYER(i) )
			{
				PLAYER(i)->SetLevelResult(LR_WIN) ;
				CONTROLS.DeActivateContoller(i) ;
			}
			else
			{
				if (PLAYER(i)->GetPrimary()) PLAYER(i)->SetLevelResult(LR_LOSE) ;
			}
		}
	
		GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
		//mCurrentAllow10SecondsCount =1 ;
		return ;
	}

	if (GAME.IsMultiPlayer() && mCurrentAllow10SecondsCount!=0 && primary)
	{
		GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
	}
	

	// single player

	if (!GAME.IsMultiPlayer())
	{
		PLAYER(0)->SetLevelResult(LR_WIN) ;
		GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
	}
}


//******************************************************************************************
void	CGameLevelData::CharacterHasDiedDirectOrPursuit(CCharacter * character) 
{
	if (GAME.IsMultiPlayer() && character->IsPrimary() )
	{
		// are there any other primary's still alive
		BOOL found_a_player_still_alive = FALSE ;
		for (SINT i =0;i<MAX_PLAYERS;i++)
		{
			if (PLAYER(i)->GetPrimary() &&
				(!(PLAYER(i)->GetPrimary()->GetFlags() & TF_DEAD)) )
			{
				found_a_player_still_alive = TRUE ;
			}
		}

		// ok no one else is alive and no on else has finsihed the level then
		// must be a draw
		if ( found_a_player_still_alive == FALSE && mCurrentAllow10SecondsCount  == 0)
		{
			// game drawn !!! 
			for (SINT i =0;i<MAX_PLAYERS;i++)
			{
				if (PLAYER(i)->GetPrimary()) PLAYER(i)->SetLevelResult(LR_DRAW) ;
			}
	
			GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
		}
		else
		{
			// although someone else in the game has the game already been won
			if (mCurrentAllow10SecondsCount!=0)
			{
				GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
			}
		}
	}

	// single player
	if (!GAME.IsMultiPlayer() && character->IsPrimary() )
	{
		PLAYER(0)->SetLevelResult(LR_LOSE) ;
		GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
	}


}

//******************************************************************************************
void	CGameLevelData::ProcessDirectOrPursuit()
{
	// ok multiplayer shit !!!
	// if character has finished
	if (mCurrentAllow10SecondsCount !=0 )
	{
		mCurrentAllow10SecondsCount+=1 ;
		if (mCurrentAllow10SecondsCount == 250)
		{
			GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
		}
	}
}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
// TRICK n TAG


//******************************************************************************************
void   CGameLevelData::CharacterHasDiedTag(CCharacter* character)
{
	CPrimary* primary = character->IsPrimary() ;
	if (!primary) return ;

	// multiplayer
	if (GAME.IsMultiPlayer())
	{
		if (mNumberOfTagPrimarysDead==0)
		{
			for (SINT i =0;i<MAX_PLAYERS;i++)
			{
				if (PLAYER(i)->GetPrimary()) PLAYER(i)->SetLevelResult(LR_WIN) ;
			}
			primary->GetPlayer()->SetLevelResult(LR_LOSE) ;
			GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
		}
		mNumberOfTagPrimarysDead++ ;
	}

	// single player
	else
	{
		PLAYER(0)->SetLevelResult(LR_LOSE) ;
		GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
	}
}


//******************************************************************************************
void	CGameLevelData::TimeHasRunOutTagMultiPlayer()
{
	// ok who's got most tags ...

	CPlayer* winner = NULL ;
	SINT winning_no_tokens = -2 ;
	BOOL drawn = FALSE ;

	for (SINT i =0;i<MAX_PLAYERS;i++)
	{
		if (PLAYER(i)->GetPrimary())
		{
			SINT tokens_got = PLAYER(i)->GetPrimary()->GetLevelData().GetNumTokensPickedUp();
			if (tokens_got == winning_no_tokens)
			{
				drawn = TRUE ;
			}
			else if (tokens_got > winning_no_tokens)
			{
				winner = PLAYER(i) ;
				winning_no_tokens = tokens_got ;
			}
		}
	}

	if (drawn)
	{
		for (SINT i =0;i<MAX_PLAYERS;i++)
		{
			if (PLAYER(i)->GetPrimary())
			{
				PLAYER(i)->SetLevelResult(LR_DRAW) ;
				// so finished ground friction kicks in
				PLAYER(i)->GetPrimary()->ChangeState(CS_FINISHED,CStateInit(0)) ;
			}
		}
	}
	else
	{
		for (SINT i =0;i<MAX_PLAYERS;i++)
		{
			if (PLAYER(i)->GetPrimary())
			{
				PLAYER(i)->SetLevelResult(LR_LOSE) ;
				// used so finished ground friction kicks in 
				PLAYER(i)->GetPrimary()->ChangeState(CS_FINISHED,CStateInit(0)) ;
			}
		}
		winner->SetLevelResult(LR_WIN) ;
		winner->GetPrimary()->SetAnimMode(AM_VICTORY) ;
	}


	GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
// DROID BALL

void	CGameLevelData::TimeHasRunOutDroidBall()
{
	// ok a bit of a basketball thing here but although the timer has
	// run out the game is not over whilst any droid ball is in the air

	const DroidBallInWorldList& items = WORLD.GetDroidBallNB();

	// loop through all droidballs in world
	for (CDroidBall* nt = items.First() ;
					 nt != NULL ;
					 nt = items.Next() )
	{
		if (!(nt->IsBeingCarried() || (nt->GetFlags() & TF_ON_GROUND) | nt->IsStuckToPole()))
		{
			// incase ball gets stuck somewhere
			GINT velocity = nt->GetVelocity().MagnitudeSq() ;
			if (velocity != G0)
			{
				// ball still in air 
				return ;
			}
		}
	}


	// multiplayer or single player mode

	if (GAME.IsMultiPlayer())
	{
		// ok who's got most points ...
		CPlayer* winner = NULL ;
		SINT winning_points = -2 ;
		BOOL drawn = FALSE ;

		for (SINT i =0;i<MAX_PLAYERS;i++)
		{
			if (PLAYER(i)->GetPrimary())
			{
				SINT points_got = PLAYER(i)->GetPrimary()->GetScore();
				if (points_got == winning_points)
				{
					drawn = TRUE ;
				}
				else if (points_got > winning_points)
				{
					winner = PLAYER(i) ;
					winning_points = points_got ;
				}
			}
		}

		if (drawn)
		{
			for (SINT i =0;i<MAX_PLAYERS;i++)
			{
				if (PLAYER(i)->GetPrimary()) 
				{
					PLAYER(i)->SetLevelResult(LR_DRAW) ;
					// used so finished ground friction kicks in 
					PLAYER(i)->GetPrimary()->ChangeState(CS_FINISHED, CStateInit(0)) ;
				}

			}
		}
		else
		{
			for (SINT i =0;i<MAX_PLAYERS;i++)
			{
				if (PLAYER(i)->GetPrimary())
				{
					PLAYER(i)->SetLevelResult(LR_LOSE) ;
					// used so finished ground friction kicks in 
					PLAYER(i)->GetPrimary()->ChangeState(CS_FINISHED, CStateInit(0)) ;
				}
			}
			winner->SetLevelResult(LR_WIN) ;
			winner->GetPrimary()->SetAnimMode(AM_VICTORY) ;
		}
	}
	else
	// single player
	{
		// you have lost! cos getting the required points would have triggered
		// the win condition
		PLAYER(0)->SetLevelResult(LR_LOSE) ;
		// used so finished ground friction kicks in 
		PLAYER(0)->GetPrimary()->ChangeState(CS_FINISHED, CStateInit(0)) ;
	}
	
	GAME.SetGameState(GAME_STATE_GAME_FINISHED) ;
}


//******************************************************************************************
void	CGameLevelData::CharacterHasDiedDroidBall(CCharacter* character)
{
	//  the same as tag...
	CharacterHasDiedTag(character) ;
}
