#include	"Common.h"

#include	"OIDs.h"
#include	"Thing.h"
#include	"Actor.h"
#include	"Effector.h"
#include	"Spawner.h"
//#include	"Drone.h"
#include	"Primary.h"
#include	"Finish.h"
#include	"Spikes.h"
#include	"Effect.h"
#include	"Morph.h"
#include	"Pickup.h"
#include	"Trigger.h"
//#include	"Drawbridge.h"
//#include	"Wall.h"
#include	"StaticMesh.h"
#include	"Cannon.h"
#include	"CannonBl.h"
//#include	"Knight.h"
//#include	"Solid.h"
//#include	"Lift.h"
//#include	"Catapault.h"
//#include	"Flipper.h"
#include	"Teleport.h"
#include	"Character.h"
//#include	"Nurbling.h"
#include	"FlameThrower.h"
//#include	"Clown.h"
#include	"BadGuys.h"
//#include	"Blimp.h"
#include	"WayPoint.h"
#include	"KingHillGoalPoint.h"
#include	"KingHillBadGuy.h"
#include	"NamedMesh.h"
//#include	"Boat.h"
#include	"HoldingZonePoint.h"
#include	"Racer.h"
#include	"Projectile.h"
#include	"Golf.h"
#include    "CheckPoint.h"
#include	"GunTurret.h"
#include	"Switch.h"
#include	"Crate.h"
#include	"Weapon.h"
#include	"Torso.h"
#include	"Fence.h"
#include	"Light.h"
#include	"Start.h"
//#include	"Grenade.h"
#include	"DroidBallDispenser.h"
#include	"DroidBall.h"
#include	"DroidBallPot.h"
#include	"HealthTile.h"
#include	"Crown.h"
#include	"Laser.h"
#include    "BadGuyPrimary.h"
#include	"Hovbot.h"
#include    "Gillam.h"

#if TARGET == PSX || TARGET == DC
#pragma dont_inline on
#include 	"malloc.h"
#endif
//#if TARGET == PSX
//#pragma dont_inline on
//#endif
CThing	*SpawnThing(SINT id, BOOL report_errors)
{
	switch (id)
	{
	case OID_CThing:				return new CThing;
	case OID_CActor:				return new CActor;
//	case OID_CEffector:				return new CEffector;
	case OID_CEffector:				return new CEFRainDrop; //!!! ARRGhh
	case OID_CEFRainDrop:			return new CEFRainDrop;
	case OID_CSpawner:				return new CSpawner;
//	case OID_CSpawnerBall:			return new CSpawnerBall;
//	case OID_CSpawnerDrone:			return new CSpawnerDrone;
//	case OID_CDrone:				return new CDrone;
//!	case OID_CPrimary:				return new CPrimary;
	case OID_CFinish:				return new CFinish;
	case OID_CSpikes:				return new CSpikes;
	case OID_CEffect:				return new CEffect;
	case OID_CFXLightning:			return new CFXLightning;
	case OID_CMorph:				return new CMorph;
	case OID_CMPBomb:				return new CMPBomb;
	case OID_CMPAverage:			return new CMPAverage;
	case OID_CPickup:				return new CPickup;
	case OID_CPickupStatic:			return new CPickupStatic;
//	case OID_CTrigger:				return new CTrigger;
//	case OID_CSpawnerFollower:		return new CSpawnerFollower;
//	case OID_CDrawbridge:			return new CDrawbridge;
//	case OID_CWall:					return new CWall;
	case OID_CStaticMesh:			return new CStaticMesh;
//	case OID_CSMCastle:				return new CSMCastle;
	case OID_CCannon:				return new CCannon;
	case OID_CCannonBall:			return new CCannonBall;
	case OID_CMPLower:				return new CMPLower;
//	case OID_CSMTower1:				return new CSMTower1;
//	case OID_CSMTower2:				return new CSMTower2;
//	case OID_CSMTower3:				return new CSMTower3;
//	case OID_CSMWindmill:			return new CSMWindmill;
//	case OID_CSMWindmillBlades:		return new CSMWindmillBlades;
//	case OID_CKnight:				return new CKnight;
	//case OID_CSolid:				return new CSolid;
	//case OID_CLift:					return new CLift;
	//case OID_CCatapault:			return new CCatapault;
	//case OID_CLiftRight1:			return new CLiftRight1;
//	case OID_CSMTree1:				return new CSMTree1;
//	case OID_CSMTree2:				return new CSMTree2;
	//case OID_CFlipperRight:			return new CFlipperRight;
	//case OID_CFlipperLeft:			return new CFlipperLeft;
	case OID_CTeleport:				return new CTeleport;
	case OID_CFXFlamer:				return new CFXFlamer;
	case OID_CEFLineWavePair:		return new CEFLineWavePair;
//	case OID_CNurbling1:			return new CNurbling1;
//	case OID_CNurbling2:			return new CNurbling2;
//	case OID_CNurbling3:			return new CNurbling3;
//	case OID_CNurbling4:			return new CNurbling4;
	case OID_CFlameThrower:			return new CFlameThrower;
	case OID_CPUPBrake:				return new CPUPBrake;
	case OID_CPUPToken:				return new CPUPToken;
//	case OID_CSpikyDeath:			return new CSpikyDeath;
//	case OID_CClown:				return new CClown;
//	case OID_CFXZ:					return new CFXZ;
//	case OID_CVampire:				return new CVampire;
//	case OID_CPumpkinHead:			return new CPumpkinHead;
//	case OID_CViking:				return new CViking;
//	case OID_CCowboy:				return new CCowboy;
//	case OID_CGhost:				return new CGhost;
//	case OID_CCharNoBall:			return new CCharNoBall;
//	case OID_CJack:					return new CJack;
	case OID_CSMArrowL:				return new CSMArrowL;
	case OID_CSMArrowR:				return new CSMArrowR;
	case OID_CFinishPin:			return new CFinishPin;
//	case OID_CZombie:				return new CZombie;
//	case OID_CBlimp:				return new CBlimp;
//	case OID_CNurbling5:			return new CNurbling5;
	case OID_CGuardB:				return new CGuardB;
	case OID_CGuardA:				return new CGuardA;
	case OID_CSGuardC:				return new CSGuardC;
//	case OID_CNurblingA1:			return new CNurblingA1;
//	case OID_CNurblingA2:			return new CNurblingA2;
//	case OID_CNurblingA3:			return new CNurblingA3;
//	case OID_CNurblingA4:			return new CNurblingA4;
//	case OID_CDictator:				return new CDictator;
//	case OID_CRobot:				return new CRobot;
//	case OID_COfficer:				return new COfficer;
//	case OID_CSoldier1:				return new CSoldier1;
//	case OID_CTank:					return new CTank;
//	case OID_CDcTank:				return new CDcTank;
//	case OID_CSMMTree1:				return new CSMMTree1;
//	case OID_CSMMTree2:				return new CSMMTree2;
//	case OID_CSMMTree3:				return new CSMMTree3;
//	case OID_CSMMTree4:				return new CSMMTree4;
//	case OID_CSMMTree5:				return new CSMMTree5;
//	case OID_CSMMTree6:				return new CSMMTree6;
//	case OID_CSMHwTower1:			return new CSMHwTower1;
//	case OID_CSMHwHouse:			return new CSMHwHouse;
//	case OID_CSMHwTomb:				return new CSMHwTomb;
//	case OID_CSMHwHotel:			return new CSMHwHotel;
//	case OID_CSMHwGraves:			return new CSMHwGraves;
//	case OID_CSMHwCastle:			return new CSMHwCastle;
//	case OID_CSMMechBossTower:		return new CSMMechBossTower;
//	case OID_CSMVacuumCannon:		return new CSMVacuumCannon;
//	case OID_CSMHwChurch:			return new CSMHwChurch;
//	case OID_CSMHwOrgan:			return new CSMHwOrgan;
//	case OID_CHwDoc:				return new CHwDoc;
	case OID_CWayPoint:				return new CWayPoint;
	case OID_CNamedMesh:			return new CNamedMesh;
//	case OID_CBoatViking:			return new CBoatViking;
//	case OID_CBoatPirate:			return new CBoatPirate;
	case OID_CRacerTank:			return new CRacerTank;
	case OID_CRacerGhost:			return new CRacerGhost;
	//case OID_CMechCatapault:		return new CMechCatapault;
//	case OID_CFlyDroid:				return new CFlyDroid;
	//case OID_CSteamHammer:			return new CSteamHammer;
	case OID_CEFLineWavePair2:		return new CEFLineWavePair2;
	case OID_CProjectile:			return new CProjectile;
	case OID_CPJBullet:				return new CPJBullet;
	case OID_CGolfArrow:			return new CGolfArrow;
	case OID_CGunTurret:			return new CGunTurret;
	case OID_CBadGuy:				return new CBadGuy;
	case OID_CPJBulletRainDrop:		return new CPJBulletRainDrop;
	case OID_CPrimaryBall:			return new CPrimaryBall;
	case OID_CTriggerTile:			return new CTriggerTile;
	case OID_CTriggerDead:			return new CTriggerDead;
	case OID_CTriggerSwitch:		return new CTriggerSwitch;
	case OID_CTriggerLap:			return new CTriggerLap;
	case OID_CSwitchDead:			return new CSwitchDead;
	case OID_CSwitchAND:			return new CSwitchAND;
	case OID_CSwitchOR:				return new CSwitchOR;
	case OID_CPrimaryAngel:			return new CPrimaryAngel;
	case OID_CPrimaryApostle:		return new CPrimaryApostle;
	case OID_CPrimaryLockdown:		return new CPrimaryLockdown;
	case OID_CCrate:				return new CCrate;
	case OID_CGenerator:			return new CGenerator;
	case OID_CPUPHealth:			return new CPUPHealth;
	case OID_CWeapon:				return new CWeapon;
	case OID_CWeaponAngelSword:		return new CWeaponAngelSword;
	case OID_CLaserTurret:			return new CLaserTurret;
	case OID_CFXFlamer2:			return new CFXFlamer2;
	case OID_CFXFlamer3:			return new CFXFlamer3;
	case OID_CFlameThrowerGround:	return new CFlameThrowerGround;
	case OID_CPrimaryAngelTorso:	return new CPrimaryAngelTorso;
	case OID_CPrimaryApostleTorso:	return new CPrimaryApostleTorso;
	case OID_CPrimaryLockdownTorso:	return new CPrimaryLockdownTorso;
	case OID_CGuardBTorso:			return new CGuardBTorso;
	case OID_CGuardATorso:			return new CGuardATorso;
	case OID_CSGuardCTorso:			return new CSGuardCTorso;
	case OID_CFencePost:			return new CFencePost;
	case OID_CFenceSection:			return new CFenceSection;
	case OID_CCannon2:				return new CCannon2;
	case OID_CPrimaryBenny:			return new CPrimaryBenny;
	case OID_CPrimaryBennyTorso:	return new CPrimaryBennyTorso;
	case OID_CClownTorso:			return new CClownTorso;
	case OID_CPrimarySophie:		return new CPrimarySophie;
	case OID_CPrimarySophieTorso:	return new CPrimarySophieTorso;
	case OID_CKingHillGoalPoint:	return new CKingHillGoalPoint;
	case OID_CCheckPoint:			return new CCheckPoint;
	case OID_CCheckPointSection:	return new CCheckPointSection;
	case OID_CWeaponApostleSword:	return new CWeaponApostleSword;
	case OID_CWeaponClub:			return new CWeaponClub;
	case OID_CWeaponCowProd:		return new CWeaponCowProd;
	case OID_CWeaponDagger:			return new CWeaponDagger;
	case OID_CWeaponHammer:			return new CWeaponHammer;
	case OID_CPrimaryDoctor:		return new CPrimaryDoctor;
	case OID_CPrimaryDoctorTorso:	return new CPrimaryDoctorTorso;
	case OID_CThingSpawner:			return new CThingSpawner;
	case OID_CPrisoner1AKingHill:	return new CPrisoner1AKingHill;
	case OID_CKingHillBadGuy:		return new CKingHillBadGuy;
	case OID_CPrisoner2AKingHill:	return new CPrisoner2AKingHill;
	case OID_CPrisoner3AKingHill:	return new CPrisoner3AKingHill;
	case OID_CEditorLight:			return new CEditorLight;
	case OID_CStartP1:				return new CStartP1;
	case OID_CStartP2:				return new CStartP2;
	case OID_CPathFinderPoint:		return new CPathFinderPoint;
	case OID_CHoldingZonePoint:		return new CHoldingZonePoint;
//	case OID_CGrenade:				return new CGrenade;
	case OID_CMarshall:				return new CMarshall;
	case OID_CDeputy:				return new CDeputy;
	case OID_CPrisoner1A:			return new CPrisoner1A;
	case OID_CPrisoner2A:			return new CPrisoner2A;
	case OID_CPrisoner3A:			return new CPrisoner3A;
	case OID_CMarshallTorso:		return new CMarshallTorso;
	case OID_CDeputyTorso:			return new CDeputyTorso;
	case OID_CPrisoner1ATorso:		return new CPrisoner1ATorso;
	case OID_CPrisoner2ATorso:		return new CPrisoner2ATorso;
	case OID_CPrisoner3ATorso:		return new CPrisoner3ATorso;
	case OID_CFXExplosion1:			return new CFXExplosion1;
	case OID_CPrisoner1B:			return new CPrisoner1B;
	case OID_CPrisoner1BTorso:		return new CPrisoner1BTorso;
	case OID_CPrisoner1C:			return new CPrisoner1C;
	case OID_CPrisoner1CTorso:		return new CPrisoner1CTorso;
	case OID_CTriggerScore:			return new CTriggerScore;
	case OID_CFXExplosion2:			return new CFXExplosion2;
	case OID_CDroidBallDispenser:	return new CDroidBallDispenser;
	case OID_CDroidBall:			return new CDroidBall;
	case OID_CDroidBallPot:			return new CDroidBallPot;

	case OID_CPrisoner2B:			return new CPrisoner2B;
	case OID_CPrisoner2C:			return new CPrisoner2C;
	case OID_CPrisoner3B:			return new CPrisoner3B;
	case OID_CPrisoner3C:			return new CPrisoner3C;
	case OID_CPrisoner2BTorso:		return new CPrisoner2BTorso;
	case OID_CPrisoner2CTorso:		return new CPrisoner2CTorso;
	case OID_CPrisoner3BTorso:		return new CPrisoner3BTorso;
	case OID_CPrisoner3CTorso:		return new CPrisoner3CTorso;
	case OID_CGuardC:				return new CGuardC;
	case OID_CGuardCTorso:			return new CGuardCTorso;
	case OID_CSGuardA:				return new CSGuardA;
	case OID_CSGuardB:				return new CSGuardB;
	case OID_CSGuardATorso:			return new CSGuardATorso;
	case OID_CSGuardBTorso:			return new CSGuardBTorso;
	case OID_CHealthTile:			return new CHealthTile;
	case OID_CFXBodyFlamer:			return new CFXBodyFlamer;
	case OID_CCrown:				return new CCrown;
	case OID_CLaserBeam:			return new CLaserBeam;
//	case OID_CGrenadeDispenser:		return new CGrenadeDispenser;
	case OID_CBadGuyLockDown:		return new CBadGuyLockDown;
	case OID_CBadGuyApostle:		return new CBadGuyApostle;
	case OID_CBadGuyBenny:			return new CBadGuyBenny;
	case OID_CBadGuySophie:			return new CBadGuySophie;
	case OID_CBadGuyDoctor:			return new CBadGuyDoctor;
	case OID_CBadGuyAngel:			return new CBadGuyAngel;
	case OID_CHovBot:				return new CHovBot;
	case OID_CGillam:				return new CGillam;
	case OID_CGillamTorso:			return new CGillamTorso;
	case OID_CFXTeleportIn:			return new CFXTeleportIn;
	case OID_CFXRageAttack:		return new CFXRageAttack;
	case OID_CStinger:				return new CStinger ;
	case OID_CFinishTrigger:		return new CFinishTrigger;
	case OID_CSMBillBoard:			return new CSMBillBoard;
	case OID_CPit:					return new CPit;
	case OID_CSMWaterTower:			return new CSMWaterTower ;
	case OID_CSMLitTower:			return new CSMLitTower ;
	case OID_CSMGolfBoard:			return new CSMGolfBoard;
	case OID_CSMIcon:				return new CSMIcon;
	case OID_CSMStatue:				return new CSMStatue;
	case OID_CSMVase:				return new CSMVase;
	case OID_CTVCamera:				return new CTVCamera;
	case OID_CSMChevron:			return new CSMChevron;
	case OID_CSMChevrons:			return new CSMChevrons;
//	case OID_CKingHillBadGuyLockDown:		return new CKingHillBadGuyLockDown;
//	case OID_CKingHillBadGuyApostle:		return new CKingHillBadGuyApostle;
//	case OID_CKingHillBadGuyBenny:			return new CKingHillBadGuyBenny;
//	case OID_CKingHillBadGuySophie:			return new CKingHillBadGuySophie;
//	case OID_CKingHillBadGuyDoctor:			return new CKingHillBadGuyDoctor;
//	case OID_CKingHillBadGuyAngel:			return new CKingHillBadGuyAngel;
	case OID_CRacerReSpawner:				return new CRacerReSpawner;
	case OID_CTriggerOnePrimaryOnHill:		return new CTriggerOnePrimaryOnHill;

	//! errr
	case OID_CPrimary:				return new CPrimaryAngel;
//	case OID_CSpawnerBall:			return new CPrimaryAngel;
	case OID_CPrimaryChar:			return new CPrimaryAngel;

	// dead things
	case OID_CEFLineWaveYR:			return new CEFLineWavePair;
//	case OID_CSMShipViking:			return new CBoatViking;
//	case OID_CSMShipPirate:			return new CBoatPirate;

//	case OID_CFollower1:			return new CNurbling1;
//	case OID_CFollower2:			return new CNurbling2;
//	case OID_CFollower3:			return new CNurbling3;
//	case OID_CFollower4:			return new CNurbling4;

	case OID_CTrigger:				return new CTriggerTile;

	default:
		if (report_errors)
		{
//			ASSERT(0);
		}
		return NULL;
	};
}

#ifdef LINK_EDITOR

// this is a bit of a bodge.

BOOL	CanThisOIDBeInstantiated(SINT id)
{
	switch (id)
	{
		// high level things
	case OID_CThing:
	case OID_CActor:
	case OID_CSpawner:
	case OID_CEffector:
	//case OID_CSolid:
	case OID_CEffect:
	case OID_CStaticMesh:
	case OID_CMorph:
	case OID_CPickup:
	//case OID_CFlipper:
	case OID_CCharacter:
	case OID_CFollower:
//	case OID_CNurbling:
//	case OID_CSMBoat:
	case OID_CCharBall:
	case OID_CEffectParticle:
//	case OID_CCharNoBall:
//	case OID_CNurblingA:
//	case OID_CBoat:
	case OID_CRacer:
	case OID_CProjectile:
	case OID_CBadGuy:
	case OID_CTrigger:
	case OID_CSwitch:
	case OID_CPrimary:
	case OID_CWeapon:
	case OID_CFXStreamer:
	case OID_CTorso:
		return FALSE;

		// dead things
	case OID_CEFLineWaveYR:
	case OID_CSMShipViking:
	case OID_CSMShipPirate:
	//case OID_CFollower1:
	//case OID_CFollower2:
	//case OID_CFollower3:
	//case OID_CFollower4:
//	case OID_CFlameThrowerBase:
		return FALSE;

	default:
		return TRUE;
	};
}

#endif

//#if TARGET == PSX
//#pragma dont_inline off
//#endif



