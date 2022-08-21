#ifndef WORLD_H
#define WORLD_H

//******************************************************************************************
#include	"Thing.h"
#include	"MemBuffer.h"
#include	"ParticleSystem.h"
#include	"Game.h"
#include	"ListArray.h"
#include	"Checkpoint.h"


#define	NUM_PARTICLE_SYSTEMS	19

#define MAX_CHARACTERS_IN_WORLD 30
#define MAX_WEAPONS_IN_WORLD	21
#define	MAX_PATHFINDERPOINTS_IN_WORLD	50
#define	MAX_HOLDINGZONEPOINTS_IN_WORLD	20
#define MAX_DROID_BALLS_IN_WORLD 20
#define MAX_HEALTH_TILES_IN_WORLD 5
#define MAX_DROID_BALL_POTS_IN_WORLD 3

class CCheckPoint ;
class CWayPoint;
class CPathFinderPoint;
class CKingHillGoalPoint;
class CHoldingZonePoint;
class CDroidBall;
class CHealthTile;
class CDroidBallPot;


typedef CListArray<CCheckPoint, MAX_CHECKPOINT_POSTS_IN_WORLD> CheckPointList ;
typedef CListArray<CWeapon, MAX_WEAPONS_IN_WORLD> WeaponList ;
typedef	CListArray<CCharacter, MAX_CHARACTERS_IN_WORLD> CharactersInWorldList ;
typedef	CListArray<CPathFinderPoint, MAX_PATHFINDERPOINTS_IN_WORLD> PathFinderPointsInWorldList ;
typedef	CListArray<CHoldingZonePoint, MAX_HOLDINGZONEPOINTS_IN_WORLD> HoldingZonePointsInWorldList ;
typedef	CListArray<CDroidBall, MAX_DROID_BALLS_IN_WORLD> DroidBallInWorldList ;
typedef	CListArray<CHealthTile, MAX_HEALTH_TILES_IN_WORLD> HealthTileInWorldList ;
typedef	CListArray<CDroidBallPot, MAX_DROID_BALL_POTS_IN_WORLD> DroidBallPotsInWorldList ;


//******************************************************************************************
class	CWorld
{
public:
	CWorld() {};
	~CWorld() {};

	BOOL	Init();
	void	Shutdown();
	void	Reset();

	void	AddDefaultObjects();

	void	Process();

	GINT	Gravity();

	// Thing stuff
	void	AddThing(CThing *t);
	void	RemoveThing(CThing *t);

	CThing	*GetFirstThing() {return mFirstThing;};

	GVector	GetPrimaryPosition(SINT n = 0);
	GVector GetPrimaryFollowPoint();

	void	 SetPrimaryThing(CPrimary *t, SINT n) {mPrimaryThings[n] = t;};

	CPrimary *GetPrimaryThing(SINT n = 0) {return mPrimaryThings[n];};

	void	DestroyThing(CThing *t);

	void	ValidateAllData();

	CharactersInWorldList& GetCharacterNB() { return mCharacterNB ; }
	CheckPointList& GetCheckPointNB() { return mCheckPointNB ; }
	WeaponList& GetWeaponsNB() { return mWeaponsNB ; }
	PathFinderPointsInWorldList& GetPathFinderPointsNB() { return mPathFinderPointsNB ; }
	HoldingZonePointsInWorldList& GetHoldingZonePointsNB() { return mHoldingZonePointsNB; }
	DroidBallInWorldList& GetDroidBallNB() { return mDroidBallNB; }
	HealthTileInWorldList& GetHealthTileNB() { return mHealthTileNB ; }
	DroidBallPotsInWorldList& GetDroidBallPotNB() { return mDroidBallPotNB ; }


	CHoldingZonePoint*	LocatePrimaryHoldingZonePoint(SINT for_holding_zone_point_num);
	// particle system stuff
	CParticleSystem	*GetParticleSystem();

	void	NextPrimary();

	// finish point
	void		RegisterFinishPos(GVector &p) {mFinishPos = p;};
	GVector		GetFinishPos() {return mFinishPos;};

	// golf
	void		GolfArrowEnable();
	void		GolfArrowDisable();

	CPlayer		*GetPlayerForPrimary(CPrimary *p);

protected:
	CThing		*mFirstThing;
	CPrimary	*mPrimaryThings[MAX_PLAYERS];

	SINT	mThingNumber;

	GVector	mPrimaryFollowPoint;

	CParticleSystem	mParticleSystems[NUM_PARTICLE_SYSTEMS];

	GVector		mFinishPos;

	// golf stuff
	CThing		*mGolfArrow;

	CharactersInWorldList mCharacterNB;
	CheckPointList mCheckPointNB ;
	WeaponList mWeaponsNB ;
	PathFinderPointsInWorldList mPathFinderPointsNB ;
	HoldingZonePointsInWorldList mHoldingZonePointsNB ;
	DroidBallInWorldList mDroidBallNB;
	HealthTileInWorldList mHealthTileNB;
	DroidBallPotsInWorldList mDroidBallPotNB;

};

//******************************************************************************************

#endif
