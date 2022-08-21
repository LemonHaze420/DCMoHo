#include	"Common.h"

#if TARGET == DC
#include	"DCRenderData.h"

CRenderData		GlobalRenderData[OID_EOL];
SINT			GRDSize = 0;

#define DECLARE_CHARACTER(name, mesh, skin, nskins)    \
	GlobalRenderData[n].mOID	= name;								\
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;					\
	i = &GlobalRenderData[n].mInit[0];								\
	strcpy(i->mName3, "circle.tga");								\
	strcpy(i->mName , mesh);										\
	i->mSkin = skin;												\
	i->mNumSkins = nskins;											\
	i->mShadow = ST_SPRITE;											\
	GlobalRenderData[n].mRTID[1]	= RTID_CRTTrail;				\
	i = &GlobalRenderData[n].mInit[1];								\
	strcpy(i->mName, "trail1.tga");									\
	GlobalRenderData[n].mRTID[2]	= RTID_CRTParticleSystem;		\
	GlobalRenderData[n].mRTID[3]	= RTID_CRTLink;					\
	i = &GlobalRenderData[n].mInit[3];								\
	strcpy(i->mName, "particles\\smallring.tga");/*link.tga");*/		\
	GlobalRenderData[n].mNumRenderThings = 4;						\
	n ++;

#define DECLARE_CHARACTER_AND_TORSO(name, mesh, skin, nskins)    \
	DECLARE_CHARACTER(name, mesh, skin, nskins)						\
	GlobalRenderData[n].mOID	= name##Torso;						\
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;					\
	i = &GlobalRenderData[n].mInit[0];								\
	strcpy(i->mName3, "circle.tga");								\
	strcpy(i->mName , mesh);										\
	i->mSkin = skin;												\
	i->mNumSkins = nskins;											\
	i->mShadow = ST_SPRITE;											\
	GlobalRenderData[n].mRTID[1]	= RTID_CRTTrail;				\
	i = &GlobalRenderData[n].mInit[1];								\
	strcpy(i->mName, "trail1.tga");									\
	GlobalRenderData[n].mRTID[2]	= RTID_CRTParticleSystem;		\
	GlobalRenderData[n].mNumRenderThings = 3;						\
	n ++;



//***********************************************************************************
void	InitRenderData()
{
	SINT				n = 0;
	CInitRenderThing	*i;

/*	// clear out defaults
	SINT	c0, c1;
	for (c0 = 0; c0 < OID_EOL; c0 ++)
	{
		for (c1 = 0; c1 < MAX_RENDER_THINGS_PER_THING; c1 ++)
		{
			i = &GlobalRenderData[c0].mInit[c1];
			i->mShadow = ST_NONE;
			i->mWaveResponse = WR_NONE;
			i->mName[0]  = 0;
			i->mName2[0] = 0;
			i->mName3[0] = 0;
			i->mName4[0] = 0;
		}
	}*/

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CCannon;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "xx_fgun.msh");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CCannon2;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "xx_fgun.msh");
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CCannonBall;
/*	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "slug.tga");
	i->mShadow = ST_SPRITE;
	n ++;*/
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "shell.msh");
	strcpy(i->mName3, "circle.tga");
	i->mShadow = ST_SPRITE;
	n ++;

	//***********************************************
//	GlobalRenderData[n].mOID	= OID_CDrawbridge;
//	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
//	i = &GlobalRenderData[n].mInit[0];
//	strcpy(i->mName, "cuboid1");
//	strcpy(i->mName, "lift2.msh");
//	n ++;

/*	//***********************************************
	GlobalRenderData[n].mOID	= OID_CDrone;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "chars\\evil.tga");
	i->mShadow = ST_SPRITE;
	n ++;
*/
	//***********************************************
	GlobalRenderData[n].mOID	= OID_CCheckPoint;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "editor checkpoint.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CWayPoint;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "editor waypoint.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSwitchDead;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "editor switch.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSwitchAND;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "editor switch.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSwitchOR;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "editor switch.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CTriggerDead;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "editor trigger.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CHealthTile;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "editor trigger.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CTriggerSwitch;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "editor trigger.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CTriggerLap;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "editor trigger.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CEditorLight;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "editor light.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CStartP1;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "StartP1.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CStartP2;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "StartP2.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFXLightning;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTLightning;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "lightning.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFenceSection;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTFence;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "fence1.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CEFRainDrop;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTBox;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "copper.raw");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CEFLineWaveYR;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "hcrystal.msh");
	i->mWaveResponse = WR_NONE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFinish;
//	GlobalRenderData[n].mRTID[0]	= RTID_CRTFlag;
//	i = &GlobalRenderData[n].mInit[0];
//	strcpy(i->mName, "Checkered.raw");
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "exitflag.msh");
	strcpy(i->mName3, "circle.tga");
	i->mShadow = ST_SPRITE;
	i->mWaveResponse = WR_NONE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFinishPin;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "golfpin.msh");
//	strcpy(i->mName, "flag_uk.msh");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CPJBullet;
/*	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
//	strcpy(i->mName, "bullet.tga");
	strcpy(i->mName, "bulletA0.tga");
	strcpy(i->mName2, "bulletA1.tga");
	i->mShadow = ST_SPRITE;*/
/*	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName , "bullet.msh");
	strcpy(i->mName3, "bul_shad.tga");
	i->mShadow = ST_SPRITE;*/
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "shuriken.tga");
	strcpy(i->mName2, "shuriken2.tga");
//	strcpy(i->mName, "bullet.tga");
	i->mAlpha = TRUE;
	i->mShadow = ST_SPRITE;
	i->mHoriz = TRUE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CPJBulletRainDrop;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
//	strcpy(i->mName, "bullet.tga");
//	strcpy(i->mName, "bulletB0.tga");
//	strcpy(i->mName2, "bulletB1.tga");
	strcpy(i->mName, "bullet2.tga");
	i->mShadow = ST_SPRITE;
	i->mAlpha = TRUE;
	i->mHoriz = TRUE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CPickupStatic;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTBox;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "copper.raw");
	n ++;

	//***********************************************
	//***********************************************
	// Characters

/*	//***********************************************
	GlobalRenderData[n].mOID	= OID_CRacerGhost;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName3, "circle.tga");
	strcpy(i->mName , "guard_a.msh");
	i->mShadow = ST_SPRITE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CRacerTank;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName3, "circle.tga");
	strcpy(i->mName , "sguard_a.msh");
	i->mShadow = ST_SPRITE;
	n ++;
*/
	//***********************************************
	GlobalRenderData[n].mOID	= OID_CPrimaryBall;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "angel.msh");
	strcpy(i->mName3, "circle.tga");
	i->mShadow = ST_SPRITE;
	GlobalRenderData[n].mRTID[1]	= RTID_CRTTrail;
	i = &GlobalRenderData[n].mInit[1];
	strcpy(i->mName, "trail1.tga");
	GlobalRenderData[n].mRTID[2]	= RTID_CRTParticleSystem;
	GlobalRenderData[n].mNumRenderThings = 3;
	n ++;

//	DECLARE_CHARACTER_AND_TORSO(OID_C, ".msh", 0, 1)

	DECLARE_CHARACTER_AND_TORSO(OID_CGuardA				, "guard_a.msh"		, 0, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CGuardB				, "guard_a.msh"		, 1, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CGuardC				, "guard_a.msh"		, 2, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CSGuardA			, "sguard_a.msh"	, 0, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CSGuardB			, "sguard_a.msh"	, 1, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CSGuardC			, "sguard_a.msh"	, 2, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CDeputy				, "deputy.msh"		, 0, 1)
	DECLARE_CHARACTER_AND_TORSO(OID_CMarshall			, "Marshal.msh"		, 0, 1)

	DECLARE_CHARACTER_AND_TORSO(OID_CPrisoner1A			, "pris1_a.msh"		, 0, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CPrisoner1B			, "pris1_a.msh"		, 1, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CPrisoner1C			, "pris1_a.msh"		, 2, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CPrisoner2A			, "pris2_a.msh"		, 0, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CPrisoner2B			, "pris2_a.msh"		, 1, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CPrisoner2C			, "pris2_a.msh"		, 2, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CPrisoner3A			, "pris3_a.msh"		, 0, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CPrisoner3B			, "pris3_a.msh"		, 1, 3)
	DECLARE_CHARACTER_AND_TORSO(OID_CPrisoner3C			, "pris3_a.msh"		, 2, 3)

	DECLARE_CHARACTER_AND_TORSO(OID_CPrimaryAngel		, "angel.msh"		, 0, 1)
	DECLARE_CHARACTER_AND_TORSO(OID_CPrimaryApostle		, "apostle.msh"		, 0, 1)
	DECLARE_CHARACTER_AND_TORSO(OID_CPrimaryBenny		, "benny.msh"		, 0, 1)
	DECLARE_CHARACTER_AND_TORSO(OID_CPrimaryLockdown	, "lockdown.msh"	, 0, 1)
	DECLARE_CHARACTER_AND_TORSO(OID_CPrimarySophie		, "sophie.msh"		, 0, 1)
	DECLARE_CHARACTER_AND_TORSO(OID_CPrimaryDoctor		, "doctor.msh"		, 0, 1)
	DECLARE_CHARACTER(OID_CBadGuyLockDown, "lockdown.msh", 0,1)
	DECLARE_CHARACTER(OID_CBadGuyAngel, "angel.msh", 0,1)
	DECLARE_CHARACTER(OID_CBadGuyBenny, "benny.msh", 0,1)
	DECLARE_CHARACTER(OID_CBadGuySophie, "sophie.msh", 0,1)
	DECLARE_CHARACTER(OID_CBadGuyDoctor, "doctor.msh", 0,1)
	DECLARE_CHARACTER(OID_CBadGuyApostle, "apostle.msh", 0,1)
//	DECLARE_CHARACTER(OID_CKingHillBadGuyLockDown, "lockdown.msh", 0,1)
//	DECLARE_CHARACTER(OID_CKingHillBadGuyAngel, "angel.msh", 0,1)
//	DECLARE_CHARACTER(OID_CKingHillBadGuyBenny, "benny.msh", 0,1)
//	DECLARE_CHARACTER(OID_CKingHillBadGuySophie, "sophie.msh", 0,1)
//	DECLARE_CHARACTER(OID_CKingHillBadGuyDoctor, "doctor.msh", 0,1)
//	DECLARE_CHARACTER(OID_CKingHillBadGuyApostle, "apostle.msh", 0,1)


	DECLARE_CHARACTER(OID_CPrisoner1AKingHill, "pris1_a.msh", 0, 3)
	DECLARE_CHARACTER(OID_CPrisoner2AKingHill, "pris2_a.msh", 2, 3)
	DECLARE_CHARACTER(OID_CPrisoner3AKingHill, "pris3_a.msh", 1, 3)

	DECLARE_CHARACTER(OID_CRacerGhost, "guard_a.msh", 2, 3);
	DECLARE_CHARACTER(OID_CRacerTank , "sguard_a.msh", 0, 3);


	GlobalRenderData[n].mOID	= OID_CGillam;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName3, "circle.tga");
	strcpy(i->mName , "gillam.msh");

	i->mShadow = ST_SPRITE;
	GlobalRenderData[n].mRTID[1]	= RTID_CRTTrail;
	i = &GlobalRenderData[n].mInit[1];
	strcpy(i->mName, "trail1.tga");
	GlobalRenderData[n].mRTID[2]	= RTID_CRTParticleSystem;
	GlobalRenderData[n].mRTID[3]	= RTID_CRTLink;
	i = &GlobalRenderData[n].mInit[3];
	strcpy(i->mName, "link.tga");
	GlobalRenderData[n].mNumRenderThings = 4;
	n ++;

	GlobalRenderData[n].mOID	= OID_CGillamTorso;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName3, "circle.tga");
	strcpy(i->mName , "gillam.msh");
	i->mShadow = ST_SPRITE;
	GlobalRenderData[n].mRTID[1]	= RTID_CRTTrail;
	i = &GlobalRenderData[n].mInit[1];
	strcpy(i->mName, "trail1.tga");
	GlobalRenderData[n].mRTID[2]	= RTID_CRTParticleSystem;
	GlobalRenderData[n].mNumRenderThings = 3;
	n ++;

	//***********************************************
/*	GlobalRenderData[n].mRTID[1]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[1];
	strcpy(i->mName, "Teleport.tga");
	GlobalRenderData[n].mNumRenderThings = 2;
	n ++;*/

	//***********************************************
/*	GlobalRenderData[n].mOID	= OID_CCatapault;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "lift2.msh");
	i->mShadow = ST_MAP;
	n ++;*/

	//***********************************************
/*	GlobalRenderData[n].mOID	= OID_CMechCatapault;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "catapult.msh");
	i->mShadow = ST_MAP;
	n ++;*/

	//***********************************************
/*	GlobalRenderData[n].mOID	= OID_CSteamHammer;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "stmhmr.msh");
	i->mShadow = ST_MAP;
	n ++;*/

	//***********************************************
/*	GlobalRenderData[n].mOID	= OID_CFlipperRight;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "Flipper.msh");
	n ++;*/

	//***********************************************
/*	GlobalRenderData[n].mOID	= OID_CFlipperLeft;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "Flipper.msh");
	n ++;*/

	//***********************************************
/*	GlobalRenderData[n].mOID	= OID_CLift;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "lift1.msh");
	n ++;*/

	//***********************************************
/*	GlobalRenderData[n].mOID	= OID_CLiftRight1;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "lift2.msh");
	i->mShadow = ST_MAP;*/
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSpikes;
//	GlobalRenderData[n].mRTID[0]	= RTID_CRTSpikes;
//	i = &GlobalRenderData[n].mInit[0];
//	strcpy(i->mName, "spikes.raw");
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "hspike.msh");
	i->mShadow = ST_NONE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CStinger ;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "stingr_c.msh");
	i->mShadow = ST_NONE;
	i->mWaveResponse = WR_DONT_ROTATE;
	GlobalRenderData[n].mRTID[1]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[1];
	strcpy(i->mName, "stingr_b.msh");
	i->mShadow = ST_NONE;
	i->mWaveResponse = WR_DONT_ROTATE;
	GlobalRenderData[n].mRTID[2]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[2];
	strcpy(i->mName, "stingr_a.msh");
	i->mShadow = ST_NONE;
	i->mWaveResponse = WR_DONT_ROTATE;
	GlobalRenderData[n].mNumRenderThings = 3;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CCrown;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "crown.msh");
	i->mShadow = ST_NONE;
	n ++;

	//***********************************************
/*	GlobalRenderData[n].mOID	= OID_CBlimp;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "blimp.msh");
	i->mShadow = ST_MAP;
	n ++;
*/
	//***********************************************
/*	GlobalRenderData[n].mOID	= OID_CFlyDroid;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "FlyDroid.msh");
	strcpy(i->mName3, "circle.tga");
	i->mShadow = ST_SPRITE;
	n ++;
*/
	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSMArrowL;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "dir_arow.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSMArrowR;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "dir_arow.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSMBillBoard;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "xx_blbrd.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSMWaterTower;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "wtr_twr.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSMLitTower;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "lit_twr.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSMGolfBoard;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "golf_brd.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;


	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSMChevron;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "chevron.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSMChevrons;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "chevrons.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;


	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSMIcon;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "icon.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSMStatue;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "Statue.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CSMVase;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "vase.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CTVCamera;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "camera.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CPit;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "pit.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CTeleport;
//	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
//	i = &GlobalRenderData[n].mInit[0];
//	strcpy(i->mName, "Teleport.tga");
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "Telep.msh");
	strcpy(i->mName3, "circle.tga");
	i->mShadow = ST_SPRITE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CTrigger;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTDecal;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "TriggerIcon.tga");
	n ++;

	//***********************************************
//	GlobalRenderData[n].mOID	= OID_CWall;
//	GlobalRenderData[n].mRTID[0]	= RTID_CRTSpringMesh;
//	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CMPAverage;
	GlobalRenderData[n].mRTID[0]	= RTID_NONE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CMPBomb;
	GlobalRenderData[n].mRTID[0]	= RTID_NONE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CMPLower;
	GlobalRenderData[n].mRTID[0]	= RTID_NONE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFXFlamer;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTParticleSystem;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFXBodyFlamer;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTParticleSystem;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFXFlamer2;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTParticleSystem;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFXFlamer3;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTParticleSystem;
	n ++;

	//***********************************************
/*	GlobalRenderData[n].mOID	= OID_CFXZ;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTParticleSystem;
	n ++;
*/
	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFXExplosion1;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTParticleSystem;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFXExplosion2;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTParticleSystem;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFXTeleportIn;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTParticleSystem;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFXRageAttack;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTParticleSystem;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CEFLineWavePair;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "crystal.msh");
	i->mWaveResponse = WR_NONE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CEFLineWavePair2;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "crystal.msh");
	i->mWaveResponse = WR_NONE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFencePost;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "efence.msh");
	i->mWaveResponse = WR_NONE;
	n ++;

	//***********************************************
/*	GlobalRenderData[n].mOID	= OID_CGrenade;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "hspike.msh");
	strcpy(i->mName3, "circle.tga");
	i->mShadow = ST_SPRITE;
	n ++;
*/
	//***********************************************
	GlobalRenderData[n].mOID	= OID_CHovBot;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "hovbot.msh");
	strcpy(i->mName3, "circle.tga");
	i->mShadow = ST_SPRITE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CDroidBall;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "ball.msh");
	strcpy(i->mName3, "circle.tga");
	i->mShadow = ST_SPRITE ;
	GlobalRenderData[n].mRTID[1]	= RTID_CRTTrail;
	i = &GlobalRenderData[n].mInit[1];
	strcpy(i->mName, "trail1.tga");
	GlobalRenderData[n].mNumRenderThings = 2;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CDroidBallPot;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "goal.msh");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CDroidBallDispenser;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "ball_dis.msh");
	n ++;

	//***********************************************
/*	GlobalRenderData[n].mOID	= OID_CGrenadeDispenser;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "ball_dis.msh");
	n ++;
*/

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CFlameThrower;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "xx_flamr.msh");
	i->mWaveResponse = WR_NONE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CGunTurret;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "xx_mgun.msh");
	i->mWaveResponse = WR_NONE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CLaserTurret;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "xx_laser.msh");
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CLaserBeam;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTLaser;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName,  "laser1.tga");
	strcpy(i->mName2, "laser1s.tga");
	strcpy(i->mName3, "laser1e.tga");
//	strcpy(i->mName4, "laser1f.tga");
	strcpy(i->mName4, "flash.tga");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CPUPBrake;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "brake.msh");
	strcpy(i->mName3, "circle.tga");
	i->mShadow = ST_SPRITE;
	i->mWaveResponse = WR_NONE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CPUPHealth;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "hhealth.msh");
	strcpy(i->mName3, "circle.tga");
	i->mShadow = ST_SPRITE;
	i->mWaveResponse = WR_NONE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CPUPToken;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
//	strcpy(i->mName, "hcrystal.msh");
	strcpy(i->mName, "medal.msh");
	strcpy(i->mName3, "circle.tga");
	i->mShadow = ST_SPRITE;
//	i->mWaveResponse = WR_NONE;
//	i->mWaveResponse = WR_DONT_ROTATE;
	n ++;

	//***********************************************
/*	GlobalRenderData[n].mOID	= OID_CSpikyDeath;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTSpikes;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "stone2.tga");
	n ++;
*/
	//***********************************************
	GlobalRenderData[n].mOID	= OID_CNamedMesh;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "");
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CGolfArrow;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "arrow.msh");
	i->mWaveResponse = WR_NONE;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CCrate;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "Crate.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CGenerator;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "gener.msh");
	i->mShadow = ST_MAP;
	i->mWaveResponse = WR_WARP;
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CWeaponAngelSword;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "ASword.msh");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CWeaponApostleSword;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "apsword.msh");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CWeaponClub;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "club.msh");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CWeaponCowProd;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "cowprod.msh");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CWeaponDagger;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "Dagger.msh");
	n ++;

	//***********************************************
	GlobalRenderData[n].mOID	= OID_CWeaponHammer;
	GlobalRenderData[n].mRTID[0]	= RTID_CRTMesh;
	i = &GlobalRenderData[n].mInit[0];
	strcpy(i->mName, "hammer.msh");
	n ++;


	GRDSize = n;
}


//***********************************************************************************
CRenderData		*GetRenderData(SINT oid)
{
	SINT	c0;

	for (c0 = 0; c0 < GRDSize; c0++)
	{
		if (GlobalRenderData[c0].mOID == oid)
		{
			return &GlobalRenderData[c0];
		}
	}

	return NULL;
}

//***********************************************************************************
CRenderThing	*CreateAndGetRenderThing(SINT oid, CThing *t)
{
	// simple linear search!
	SINT			c0, c1;
	CRenderThing	*top = NULL, *rt;

	for (c0 = 0; c0 < GRDSize; c0++)
	{
		if (GlobalRenderData[c0].mOID == oid)
		{
			if (GlobalRenderData[c0].mRTID[0] == RTID_NONE)
				return NULL; // request no renderthing

			for (c1 = GlobalRenderData[c0].mNumRenderThings - 1; c1 >= 0; c1 --)
			{
				rt = SpawnRenderThing(GlobalRenderData[c0].mRTID[c1]);

				if (rt)
				{
					GlobalRenderData[c0].mInit[c1].mThing = t; //! hmmmmmm.. probably ok!
					rt->Init(&GlobalRenderData[c0].mInit[c1]);
					rt->ChainRenderThing(top);
					top = rt;
				}
			}

			return rt;
		}
	}

	// didn't find one - so use a default RenderThing
	rt = SpawnRenderThing(RTID_CRTDecal);

	if (rt)
	{
		CInitRenderThing i;
		strcpy(i.mName, "Unknown.tga");
		i.mThing = t;
		rt->Init(&i);
	}
	return rt;
}

#endif
