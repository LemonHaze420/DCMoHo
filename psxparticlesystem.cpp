#include	"Common.h"

#if TARGET == PSX

#include	"PSXParticleSystem.h"
#include	"Globals.h"
#include	"ParticleSystem.h"
#include    <mwinline.h>
#include <gtemac.h>
#include <gtereg_s.h>
#include <inline_s.h>
#include	"PSXDecal.h"


extern void ParticleRingRender(CParticle *p,CThing *mOwner);
extern void ParticleRayRender(CParticle *p);
extern void RenderAlphaShpere(CParticle *p);
extern void RenderAlphaSpark(CParticle *p, BOOL alpha, SINT Queuelength,CThing *mOwner);
extern void RenderStarTrail(CParticle *p, SINT Queuelength);
extern void RenderLightning(CParticle *p,CThing *mOwner);

//******************************************************************************************

#define MAKE_COLOUR(x) ((((x) >> 1) & 0x7f7f7f7f) | (0x02 << 24))

#define MAKE_ICOLOUR(r,g,b,i) ( ((b*i)<<8)|(g*i)|((r*i)>>8)|(2<<24))

#define gte_ldcvshift( r0 )  \
   __evaluate (__arg0, (long)(r0));\
   __asm_start();\
   __I_lbu (12, 0, __arg0);\
   __I_lbu (13, 1, __arg0);\
   __I_lbu (14, 2, __arg0);\
   __I_sll (12,12,4);\
   __I_sll (13,13,4);\
   __I_sll (14,14,4);\
   __I_mtc2 (12, 9);\
   __I_mtc2 (13, 10);\
   __I_mtc2 (14, 11);\
   __asm_end();

#if 0
asm long Interpolcol (unsigned long *col,short mult)
{
	lw		t0,0(a0)
	mtc2	a1,C2_IR0
	li		t1,4096
	ctc2	zero,C2_RFC
	ctc2	zero,C2_GFC
	ctc2	zero,C2_BFC
	sub		t0,t1,t0
	mtc2	t0,C2_RGB
	nop
	DPCS
	nop
	mfc2	v0,C2_RGB2
	nop
	sll		v0,v0,8
	jr	ra
	srl		v0,v0,8
}
#else

long Interpolcol (unsigned long *col,short mult)
{
	long retval;
	gte_ldrgb(col);
	gte_lddp(4096-mult);
	gte_dpcs();
	gte_strgb(&retval);
	return retval;
}
#endif

#if 0

long Interpolcol (unsigned long *col,short mult)
{
	unsigned char result[3];
	LoadAverageCol((unsigned char*)col,(unsigned char*)col,(long)mult>>1,(long)mult>>1,result);
	long rcol=((result[0])+(result[1]<<8)+(result[2]<<16));
	return rcol;
}



#endif

void    CPSXParticleSystem::RenderFromData(CThing *t, const SRTInfo &ri)
{
	CParticleSystem	*ps = t->GetRenderParticleSystem();
	if((!ps)||(!(ps->IsActive())))
		return;
	Render(ps);
}

void CPSXParticleSystem::Render(CParticleSystem*p)
{
	//if ((!ps) || (!(ps->IsActive())))
	//	return;

	SINT	c0;
	CParticle	*part = p->mParticles;

	CommonOffsetCoord(0,0,0);           // Load the camera matrix....
	CameraAlreadyLoaded=1;
	for (c0 = 0; c0 < MAX_PARTICLES; c0 ++)
	{
		if (part->mType != PART_NONE)
		{

			if(part->mDelay==0)
			{
				switch(part->mType)
				{
	//				case	PART_FIRE1		:	CommonDecalRender(&part->mPos,part->mRadius,ENGINE.GetParticleTexture(TP_FIRE1),part->mColour|(0x02<<24)); break ;
	//				case	PART_FIRE2		:	CommonDecalRender(&part->mPos,part->mRadius,ENGINE.GetParticleTexture(TP_FIRE2),part->mColour|(0x02<<24)); break ;
	//				case	PART_FIRE3		:	CommonDecalRender(&part->mPos,part->mRadius,ENGINE.GetParticleTexture(TP_FIRE3),part->mColour|(0x02<<24)); break ;
	//				case	PART_Z			:	CommonDecalRender( &part->mPos,part->mRadius,ENGINE.GetParticleTexture(TP_Z    ),part->mColour|(0x02<<24)); break ;
	//				default 	 			:	CommonDecalRender(&part->mPos,part->mRadius,ENGINE.GetParticleTexture(TP_FIRE1),part->mColour|(0x02<<24)); break ;

					case	PART_FIRE1		:
					case	PART_BODY_FIRE  :	CPSXDecal::CommonDecalRender(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(TP_FIRE1), MAKE_COLOUR(part->mColour)); break ;
					case	PART_FIRE2		:	CPSXDecal::CommonDecalRender(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(TP_FIRE2), MAKE_COLOUR(part->mColour)); break ;
					case	PART_FIRE3		:	CPSXDecal::CommonDecalRender(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(TP_FIRE3), MAKE_COLOUR(part->mColour)); break ;
					case	PART_Z			:	CPSXDecal::CommonDecalRender(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(TP_Z    ), MAKE_COLOUR(part->mColour)); break ;
					case	PART_WRING2		:	CPSXDecal::CommonDecalRender(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(TP_BUBBLE), MAKE_COLOUR(part->mColour)); break ;

					//case	PART_FIRE1		:
					//case	PART_BODY_FIRE  :
					//case	PART_FIRE2		:
					//case	PART_FIRE3		:
					//case	PART_Z			:  break;


  					case PART_TELEPORTINRING:
					case PART_TELEPORTINCOLUMN:
					case PART_NECOLUMN 		:
					case	PART_WATERRING_SMALL	:
					case PART_WATERRING 	:
					case	PART_COLUMN		:
					case	PART_RING		:	ParticleRingRender(part,p->mOwner); break ;


					case	PART_RAY		:	ParticleRayRender(part); break ;
//					case	PART_WATERRING	:	RenderLightning(part,p->mOwner); break ;
					case	PART_STAR1		:
						{
							long col=Interpolcol(&part->mColour,part->uPStar.mBrightness)&0x00ffffff;//part->uPStar.mBrightness);
							CPSXDecal::CommonDecalRender(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(TP_STAR1),col|(2<<24));
						}
						break;
					case	PART_STAR2		:	CPSXDecal::CommonDecalRender(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(TP_STAR1), part->mColour|(0x02<<24)); break ;
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
								CPSXDecal::CommonDecalRenderHoriz(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(part->uPAnim.mFrame),col|(2<<24),fudge);
							}
							else
							if(part->uPAnim.mHoriz==2)
								CPSXDecal::CommonDecalRenderHoriz2(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(part->uPAnim.mFrame),col|(2<<24),part->uPAnim.mSn,part->uPAnim.mCs);
							else
								CPSXDecal::CommonDecalRender(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(part->uPAnim.mFrame),col|(2<<24));
						}
						break;
					case	PART_SMOKE		:
							long col=Interpolcol(&part->mColour,part->uPAnim.mBrightness)&0x00ffffff;//part->uPStar.mBrightness);
//							CPSXDecal::CommonDecalRender(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(part->uPAnim.mFrame),col|(2<<24));
							CPSXDecal::CommonDecalRender(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(TP_SMOKE),col|(2<<24));
						break;

					case	PART_HEMI		:	RenderAlphaShpere(part); break ;
					case	PART_SPARK		:	RenderAlphaSpark(part, FALSE,2,NULL); break ;
					case	PART_SPLASH		:	RenderAlphaSpark(part, TRUE,2,NULL); break ;
					case	PART_SPINSPARK	:	RenderAlphaSpark(part, TRUE,4,NULL); break ;
					//case	PART_TELEPORTSPARK: RenderStarTrail(part, 7); break ;
//					case	PART_TELEPORTSPARK: RenderAlphaSpark(part, TRUE, 7,p->mOwner); break ;
					case	PART_LIGHTNING	:	RenderLightning(part,p->mOwner); break ;
					case	PART_LIGHT		:	CPSXDecal::CommonDecalRender(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(TP_HIT1), part->mColour|(0x02<<24)); break ;
					default 	 			:	CPSXDecal::CommonDecalRender(&part->mPos,part->mRadius<<9,ENGINE.GetParticleTexture(TP_FIRE1), MAKE_COLOUR(part->mColour)); break ;
				}
			}
		}
		part ++;
	}
	CameraAlreadyLoaded=0;
}

//******************************************************************************************
#endif
