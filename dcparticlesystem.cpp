#include	"Common.h"

#if TARGET == DC

#include	"DCParticleSystem.h"
#include	"Globals.h"
#include	"ParticleSystem.h"
#include	"DCDecal.h"
#include 	"MainCode.h"
#include	"DCJunk.h"


extern void ParticleRingRender(CParticle *p,CThing *mOwner);
extern void RenderAlphaShpere(CParticle *p);
extern void RenderAlphaSpark(CParticle *p, BOOL alpha, SINT Queuelength,CThing *mOwner);
extern void RenderLightning(CParticle *p,CThing *mOwner);

//******************************************************************************************
//#define MAKE_COLOUR(x) ((((x) >> 1) & 0x7f7f7f7f))
#define MAKE_COLOUR(x) (x)   // not needed!!

//******************************************************************************************
long Interpolcol(unsigned long *col,short mult)
{
	// interpolate colour with 4096 multiplier
	
	SINT	r = ((*col) >> 16) & 0xff;
	SINT	g = ((*col) >>  8) & 0xff;
	SINT	b = ((*col)      ) & 0xff;
	
	if (mult > 4096) mult = 4096;
	
	r = (r * mult) >> 12;
	g = (g * mult) >> 12;
	b = (b * mult) >> 12;
	
	return (r << 16) + (g << 8) + b;
}	

//******************************************************************************************
void    CDCParticleSystem::RenderFromData(CThing *t, const SRTInfo &ri)
{
	CParticleSystem	*ps = t->GetRenderParticleSystem();
	if((!ps)||(!(ps->IsActive())))
		return;
	Render(ps);
}

//******************************************************************************************
void CDCParticleSystem::Render(CParticleSystem*p)
{
	SINT	c0;
	CParticle	*part = p->mParticles;

	for (c0 = 0; c0 < MAX_PARTICLES; c0 ++)
	{
		if (part->mType != PART_NONE)
		{

			if(part->mDelay==0)
			{
				switch(part->mType)
				{
					case	PART_FIRE1		:
					case	PART_BODY_FIRE  :	CDCDecal::CommonDecalRender(&part->mPos,SINT(part->mRadius) << 9,ENGINE.GetParticleTexture(TP_FIRE1), MAKE_COLOUR(part->mColour)); break ;
					case	PART_FIRE2		:	CDCDecal::CommonDecalRender(&part->mPos,SINT(part->mRadius) << 9,ENGINE.GetParticleTexture(TP_FIRE2), MAKE_COLOUR(part->mColour)); break ;
					case	PART_FIRE3		:	CDCDecal::CommonDecalRender(&part->mPos,SINT(part->mRadius) << 9,ENGINE.GetParticleTexture(TP_FIRE3), MAKE_COLOUR(part->mColour)); break ;
					case	PART_Z			:	CDCDecal::CommonDecalRender(&part->mPos,SINT(part->mRadius) << 9,ENGINE.GetParticleTexture(TP_Z    ), MAKE_COLOUR(part->mColour)); break ;
					case	PART_WRING2		:	CDCDecal::CommonDecalRender(&part->mPos,SINT(part->mRadius) << 9,ENGINE.GetParticleTexture(TP_BUBBLE), MAKE_COLOUR(part->mColour)); break ;

  					case 	PART_TELEPORTINRING:
					case 	PART_TELEPORTINCOLUMN:
					case 	PART_NECOLUMN 		:
					case	PART_WATERRING_SMALL	:
					case 	PART_WATERRING 	:
					case	PART_COLUMN		:
					case	PART_RING		:	ParticleRingRender(part,p->mOwner); break ;


//	JCL not used	case	PART_RAY		:	ParticleRayRender(part); break ;
					case	PART_STAR1		:
						{
							long col=Interpolcol(&part->mColour,part->uPStar.mBrightness)&0x00ffffff;//part->uPStar.mBrightness);
							CDCDecal::CommonDecalRender(&part->mPos,SINT(part->mRadius) << 9,ENGINE.GetParticleTexture(TP_STAR1),col);
						}
						break;
					case	PART_STAR2		:	CDCDecal::CommonDecalRender(&part->mPos,SINT(part->mRadius) << 9,ENGINE.GetParticleTexture(TP_STAR1), part->mColour); break ;
					case	PART_SHOCKWAVE	:
					case	PART_FLASH		:
					case	PART_ANIM		:
						{
							long col=Interpolcol(&part->mColour,part->uPAnim.mBrightness)&0x00ffffff;//part->uPStar.mBrightness);
							if(part->uPAnim.mHoriz==1)
							{
								SINT fudge;
								if(part->mType==PART_SHOCKWAVE)SINT fudge=-20;
								else fudge=0;
								CDCDecal::CommonDecalRenderHoriz(&part->mPos,SINT(part->mRadius) << 9,ENGINE.GetParticleTexture(part->uPAnim.mFrame),col,fudge);
							}
							else
							if(part->uPAnim.mHoriz==2)
								CDCDecal::CommonDecalRenderHoriz2(&part->mPos,SINT(part->mRadius) << 9,ENGINE.GetParticleTexture(part->uPAnim.mFrame),col,part->uPAnim.mSn,part->uPAnim.mCs);
							else
								CDCDecal::CommonDecalRender(&part->mPos,SINT(part->mRadius) << 9,ENGINE.GetParticleTexture(part->uPAnim.mFrame),col);
						}
						break;
					case	PART_SMOKE		:
							long col=Interpolcol(&part->mColour,part->uPAnim.mBrightness)&0x00ffffff;//part->uPStar.mBrightness);
							CDCDecal::CommonDecalRender(&part->mPos,SINT(part->mRadius) << 9,ENGINE.GetParticleTexture(TP_SMOKE),col);
						break;

					case	PART_HEMI		:	RenderAlphaShpere(part); break ;
					case	PART_SPARK		:	RenderAlphaSpark(part, FALSE,2,NULL); break ;
					case	PART_SPLASH		:	RenderAlphaSpark(part, TRUE,2,NULL); break ;
					case	PART_SPINSPARK	:	RenderAlphaSpark(part, TRUE,4,NULL); break ;
					case	PART_LIGHTNING	:	RenderLightning(part,p->mOwner); break ;
					case	PART_LIGHT		:	CDCDecal::CommonDecalRender(&part->mPos,SINT(part->mRadius) << 9,ENGINE.GetParticleTexture(TP_HIT1), part->mColour); break ;
					default 	 			:	CDCDecal::CommonDecalRender(&part->mPos,SINT(part->mRadius) << 9,ENGINE.GetParticleTexture(TP_FIRE1), MAKE_COLOUR(part->mColour)); break ;
				}
			}
		}
		part ++;
	}
}

//******************************************************************************************
#endif
