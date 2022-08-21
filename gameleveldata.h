// GameLevelData.h: interface for the CGameLevelData class.
//
//////////////////////////////////////////////////////////////////////
#ifndef GAME_LEVEL_DATA_H
#define GAME_LEVEL_DATA_H

// max number of checkpoints 
#define		NO_CHECKPOINTS_MAX 20
#define		MAX_CHECKPOINT_POSTS_IN_WORLD NO_CHECKPOINTS_MAX * 3 

#include "ListArray.h"


class CKingHillEntry ;

typedef  CListArrayInstances<CKingHillEntry,  MAX_CHARS_IN_WORLD> KingHillEntryList ;

class CKingHillEntry
{	
	public:
		CKingHillEntry() {} ;
		CKingHillEntry(CCharacter* character, SINT last_frame_on_hill) : mCharacter(character), 
																		 mLastFrameOnHill(last_frame_on_hill) {} ;
		CCharacter* GetCharacter() { return mCharacter ; }
		SINT		GetLastFrameOnHill() { return mLastFrameOnHill ;}
		void		SetLastFrameOnHill(SINT new_time) { mLastFrameOnHill = new_time ; }
	private:
		CCharacter* mCharacter;
		SINT mLastFrameOnHill;
};


// ****** natrually this would normaly be split up into different children but
// not allowed to use new and delete.. hmmmm this causes a problem...

class CGameLevelData

{


	// generic parent part
public:

	void CharacterFinishedLevel(CCharacter* character) ;
	void TimeHasRunOut() ;
	void Init() ;
	void ProcessLevel() ;
	void DeclareCharsThatWillAppearInThisGame() ;
	void CharacterHasDied(CCharacter* c);
	SINT GetStartCharsInGame() { return mStartCharsInGame ; }


protected:
	SINT mCharsInGame;
	SINT mStartCharsInGame ;
	SINT		mCurrentAllow10SecondsCount;


public:
	// racer stuff

	void	CharacterFinishedRace(CCharacter* character) ;
	void	CharacterHasDiedRacer(CCharacter* character) ;
	SINT	GetFinshedPositionForCharacter(CCharacter* character) ;
	void    ProcessRace() ;

protected:


	CCharacter* mRacePositionResults[MAX_CHARS_IN_WORLD] ;

	SINT		mCurrentRaceResultIndex;



public: 
	// king of the hill

	

	void AddCharacterToKingHill(CCharacter* new_character) ;
	void CharacterFinishedKingHill(CCharacter* character) ;
	void CharacterHasDiedKingHill(CCharacter* character) ;
	void RefreshKingHillData() ;
	KingHillEntryList& GetKingHillEntries() { return mCharsOnHill ; }
	SINT NumCharactersOnHill() ;


protected:


	KingHillEntryList mCharsOnHill ;
	SINT mLastKingHillRefreshTime ;


	// pursuit direct gaunlet whatever

	void	CharacterFinishedDirectOrPursuit(CCharacter* character) ;
	void	CharacterHasDiedDirectOrPursuit(CCharacter* character) ;
	void	ProcessDirectOrPursuit() ;



	// TAG
public:

	void	CharacterHasDiedTag(CCharacter * character) ;
	void	TimeHasRunOutTagMultiPlayer() ;

protected: 
	SINT	mNumberOfTagPrimarysDead ;



	// DROID BALL
public:
	void	TimeHasRunOutDroidBall() ;
	void	CharacterHasDiedDroidBall(CCharacter * character) ;

} ;

#endif 
