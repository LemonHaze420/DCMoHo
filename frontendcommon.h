#ifndef FRONTENDCOMMON_H
#define FRONTENDCOMMON_H


//*******************************************************
// Prison Select

#if TARGET != DC	// safety

enum	EFEPST
{
	FEPST_BACK = 0				,
	FEPST_TITLEA				,
	FEPST_TITLEB				,
	FEPST_CHARGRIDB				,
	FEPST_BOXL					,
	FEPST_BOXM					,
	FEPST_BOXR					,
	FEPST_PR_ECO				,
	FEPST_PR_ZEPPELIN			,
	FEPST_PR_HOSPITAL			,
	FEPST_PR_SHIP				,
	FEPST_PR_REHAB				,
	FEPST_PR_MINING				,
	FEPST_PR_DEATHROW			,
	FEPST_PR_SPORTS				,
	FEPST_PR_MILITARY			,
	FEPST_PR_ALPHASTAR			,
	FEPST_PR_BONUS				,
	FEPST_LT_KING_OF_THE_HILL	,
	FEPST_LT_LAST_MAN_ROLLING	,
	FEPST_LT_RUN_THE_GAUNTLET	,
	FEPST_LT_PURSUIT			,
	FEPST_LT_RACE				,
	FEPST_LT_TRICK				,
	FEPST_LT_POWERBALL			,
	FEPST_LT_GOLF				,
	FEPST_LT_PINBALL			,
	FEPST_LT_SBALL				,
	FEPST_LT_SHADOW				,
	FEPST_QUALIFY				,
	FEPST_GOLD					,
	FEPST_SPARK					,
	FEPST_BORD_PT				,
	FEPST_BORD_PB				,
	FEPST_BORD_PL				,
	FEPST_BORD_PR				,
	FEPST_BORD_CTL				,
	FEPST_BORD_CTR				,
	FEPST_BORD_CBL				,
	FEPST_BORD_CBR				,
	FEPST_LEVEL_CLIP_L			,
	FEPST_LEVEL_CLIP_R			,
	FEPST_ARROW_L				,
	FEPST_ARROW_R				,
	FEPST_ARROW_U				,
	FEPST_ARROW_D				,
	FEPST_QUALIFY2				,
	FEPST_DRAW					,
	FEPST_LOST_TIME				,
	FEPST_HALO					,
};

#endif

enum	EFEDCPST
{
	FEDCPST_BACK1 = 0			,
	FEDCPST_BACK2				,
	FEDCPST_BACK3				,
	FEDCPST_BACK4				,
	FEDCPST_BACK5				,
	FEDCPST_BACK6				,
	FEDCPST_TITLE1				,
	FEDCPST_TITLE2				,
	FEDCPST_TITLE3				,
	FEDCPST_TITLE4				,
	FEDCPST_TITLE5				,
	FEDCPST_TITLE6				,
	FEDCPST_LCD1				,
	FEDCPST_LCD2				,
	FEDCPST_LCD3				,
	FEDCPST_LCD4				,
	FEDCPST_LCD5				,
	FEDCPST_CHARGRIDB			,
	FEDCPST_BOXL				,
	FEDCPST_BOXM				,
	FEDCPST_BOXR				,
	FEDCPST_PR_ECO				,
	FEDCPST_PR_ZEPPELIN			,
	FEDCPST_PR_HOSPITAL			,
	FEDCPST_PR_SHIP				,
	FEDCPST_PR_REHAB			,
	FEDCPST_PR_MINING			,
	FEDCPST_PR_DEATHROW			,
	FEDCPST_PR_SPORTS			,
	FEDCPST_PR_MILITARY			,
	FEDCPST_PR_ALPHASTAR		,
	FEDCPST_PR_BONUS			,
	FEDCPST_LT_KING_OF_THE_HILL	,
	FEDCPST_LT_LAST_MAN_ROLLING	,
	FEDCPST_LT_RUN_THE_GAUNTLET	,
	FEDCPST_LT_PURSUIT			,
	FEDCPST_LT_RACE				,
	FEDCPST_LT_TRICK			,
	FEDCPST_LT_POWERBALL		,
	FEDCPST_LT_GOLF				,
	FEDCPST_LT_PINBALL			,
	FEDCPST_LT_SBALL			,
	FEDCPST_LT_SHADOW			,
	FEDCPST_QUALIFY				,
	FEDCPST_GOLD				,
	FEDCPST_SPARK				,
	FEDCPST_BORD_PT				,
	FEDCPST_BORD_PB				,
	FEDCPST_BORD_PL				,
	FEDCPST_BORD_PR				,
	FEDCPST_BORD_CTL			,
	FEDCPST_BORD_CTR			,
	FEDCPST_BORD_CBL			,
	FEDCPST_BORD_CBR			,
	FEDCPST_LEVEL_CLIP_L		,
	FEDCPST_LEVEL_CLIP_R		,
	FEDCPST_ARROW_L				,
	FEDCPST_ARROW_R				,
	FEDCPST_ARROW_U				,
	FEDCPST_ARROW_D				,
	FEDCPST_DRAW				,
	FEDCPST_LOST_TIME			,
	FEDCPST_HALO				,
	FEDCPST_ALPHAWHITE			,
};

enum	EFEPSM
{
	FEPSM_ANGEL = 0			,
	FEPSM_APOSTLE			,
	FEPSM_BENNY				,	
	FEPSM_LOCKDOWN			,
	FEPSM_SOPHIE  			,
	FEPSM_DOCTOR			,
	FEPSM_FLAG_UK			,
	FEPSM_FLAG_FR			,
	FEPSM_FLAG_GR			,
	FEPSM_FLAG_SP			,
	FEPSM_FLAG_IT			,
};

//#define FEPSM_PLANET			0

//*******************************************************
enum	ESCREENT
{
	SCREENT_MAIN1 = 0		,
	SCREENT_MAIN2 			,
	SCREENT_MAIN3 			,
	SCREENT_MAIN4 			,
	SCREENT_MAIN5 			,
	SCREENT_MAIN6 			,
};

//*******************************************************
//	old level select

#define FET_STARSKY			0
#define	FET_TV				1
#define	FET_TVMOHO			2
#define FET_TVNOISE			3
#define FET_TVLINES			4
#define FET_TVLINES2		5
#define	FET_TVDOT			6
#define	FET_LEVELCIRCLE		7

#endif