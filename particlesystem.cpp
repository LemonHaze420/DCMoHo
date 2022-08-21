#include	"Common.h"

#include	"ParticleSystem.h"

#include	"Texturecommon.h"
#include	"Globals.h"



//************************************************************************
void	CParticleSystem::Activate()
{
	mActive = TRUE;

	// clear effects

	SINT	c0;

	for (c0 = 0; c0 < MAX_PARTICLES; c0 ++)
	{
		mParticles[c0].mType = PART_NONE;
	}
}


#define TELEPORT_RING_LIFE	34
#define TELEPORT_COLUMN_LIFE	35
#define TELEPORT_WIDTH 0x200
#define TELEPORT_RING_WIDTH_ADDER  8
#define TELEPORT_COLUMN_WIDTH_ADDER  (TELEPORT_WIDTH/TELEPORT_COLUMN_LIFE)
#define TELEPORT_VELOCITY_ADDER 0x0480
#define TELEPORT_MAX_BRIGHTNESS (0x1000)
#define TELEPORT_RING_BRIGHTNESS_ADDER (TELEPORT_MAX_BRIGHTNESS/TELEPORT_RING_LIFE)
#define TELEPROT_COLUMN_BRIGHTNESS_ADDER (TELEPORT_MAX_BRIGHTNESS/TELEPORT_COLUMN_LIFE)

#define SMOKE_LIFE 25
#define FLASH_LIFE 10

#define SHOCKWAVE_LIFE 15
#define SHOCKWAVE_INITIAL_GROWTH 0x30
#define SHOCKWAVE_GROWTH_DECEL ((SHOCKWAVE_INITIAL_GROWTH)/SHOCKWAVE_LIFE)

// for the rotating sparks
#define  TSPoffs    (p->uPSpark.mVel.vx)
#define  TSPradius    (p->uPSpark.mVel.vy)

//************************************************************************
short waterbright[7]={0x200,0x500,0x900,0xb00,0xa00,0x600,0x300};
CParticle	*CParticleSystem::CreateParticle(EParticleType type)
{
	// find a free particle

	SINT		c0;
	CParticle	*p = mParticles;

	for (c0 = 0; c0 < MAX_PARTICLES; c0 ++)
	{
		if (p->mType == PART_NONE)
		{
			p->mType = type;
			p->mDelay=0;                // Gametick delay to activation
			p->mIsRelative=0;
			mActive = TRUE;
			switch (type)
			{
				// Type specific initialisation

			case PART_FIRE1:
			case PART_FIRE2:
			case PART_FIRE3:
				p->mLife   = 30;
				p->mRadius = 0x100;
				break;

			case PART_BODY_FIRE:
				p->mLife   = 20;
				p->mRadius = 100;
				break;

			case PART_Z:
				p->mRadius	= 78;       // G(0,20000) >> 8
				p->mLife	= 50;
				break;
			case PART_RING:
				p->mRadius	= 0;
				p->mLife	= 100;
				p->uPRing.mSpin = rand();
				p->uPRing.mWidth = 512+(rand()&255);
				p->uPRing.mHeight = 0;
				p->uPRing.mSegs = 10;
				p->uPRing.mStep=(4096/10);
				p->uPColumn.mTextureNum=0;
				break;
			case PART_COLUMN:
				p->mRadius	= 1;
//				p->mLife	= 25;
				p->mLife	= 16;
//				p->uPColumn.mSpin = rand();
//				p->uPColumn.mWidth = 512+(rand()&255);
				p->uPColumn.mOffs = 0;
				p->uPColumn.mSpin = 0;
				p->uPColumn.mWidth = 512;
				p->uPColumn.mHeight = 2048;
				p->uPColumn.mSegs = 12;
				p->uPColumn.mStep=(4096/12);
				p->uPColumn.mTextureNum=0;
				break;
			case PART_NECOLUMN :
				p->mRadius	= 0x100;
				p->uPColumn.mSegs = 12;
				p->uPColumn.mStep=(4096/12);
				break;
			case PART_RAY:
				p->mRadius	= 0;
				p->mLife	= 100;
				p->uPRay.mSpin = rand();
				if(rand()&4)p->uPRay.mSpin=-p->uPRay.mSpin;
				p->uPRay.mLength = 2048+(rand()&255);
				p->uPColumn.mHeight = (rand()&512);
				p->uPRay.mSegs = 4 + (rand()&7);
				p->uPRay.mStep=(4096/p->uPRay.mSegs);
				break;
			case PART_WATERRING :
				p->mRadius	= 0;
				p->mLife	= 25;
				p->mColour	= 0x00a0a000;  // Cyan
				p->uPRing.mBrightness = 250;
				p->uPRing.mSpin = (rand()&8)?64:-64;
				p->uPRing.mWidth = 110;
				p->uPRing.mHeight = 0;
				p->uPRing.mSegs = 14;
				p->uPRing.mStep=(4096/14);
				p->uPColumn.mTextureNum=0;
				break;
			case PART_STAR1 :
				p->mRadius	= 40;
				p->mLife	= 25;
				break;
			case PART_WRING2:
				p->mRadius	= 78>>2;       // G(0,20000) >> 8
				p->mLife	= 1000;
				p->uPColumn.mTextureNum=0;
				break;

			case PART_ANIM :
				p->mLife=255;
				p->uPAnim.mTick=1;
				p->uPAnim.mBrightness = waterbright[6];//(p->uPAnim.mFrames) << 9;
				break;
			case PART_SMOKE :
				{
					p->mLife=SMOKE_LIFE;
					SINT	col = rand() & 0x7f;
					p->mColour = (col << 16) + (col << 8) + col;
					p->uPAnim.mBrightness = 0x800;//(p->uPAnim.mFrames) << 9;
				}
				break;
			case PART_FLASH :
				{
					p->uPAnim.mHoriz = 0; // safety
					p->mLife=FLASH_LIFE;
					SINT	col = rand() & 0x7f;
					p->mColour = (col << 16) + (col << 8) + col;
					p->uPAnim.mBrightness = 0x1000;//(p->uPAnim.mFrames) << 9;
				}
				break;
			case PART_HEMI:
				p->mRadius	= 1;
//				p->mLife	= 25;
				p->mLife	= 18;
				break;
			case PART_SPARK :
				{
					p->mRadius	= 40;
					p->mLife	= 8;

					for(int s=0;s<2;s++)
					{
						p->uPSpark.mQueue[s].vx=0;
						p->uPSpark.mQueue[s].vy=0;
						p->uPSpark.mQueue[s].vz=0;
					}
				}
				break;
			case PART_SPLASH:
				{
					p->mRadius	= 40;
					p->mLife	= 16;
					for(int s=0;s<2;s++)
					{
						p->uPSpark.mQueue[s].vx=0;
						p->uPSpark.mQueue[s].vy=0;
						p->uPSpark.mQueue[s].vz=0;
					}
				}
				break;
			case PART_SPINSPARK:
				{
					p->mRadius	= 40;
					p->mLife	= 16;
					for(int s=0;s<4;s++)
					{
						p->uPSpark.mQueue[s].vx=0;
						p->uPSpark.mQueue[s].vy=0;
						p->uPSpark.mQueue[s].vz=0;
					}
				}
				break;

			case PART_TELEPORTINCOLUMN:
				p->mRadius	= 1;
				p->mLife	= TELEPORT_COLUMN_LIFE;
				p->uPColumn.mSpin = 64;
				p->uPColumn.mHeight = 2048;
				p->uPColumn.mWidth=0;
//				p->uPColumn.mSegs = 5;
				p->uPColumn.mSegs = 10;
				p->uPColumn.mStep=(4096/10);
				p->uPColumn.mTextureNum=0;
				break;
/////////////////////////////////////
			case PART_TELEPORTINRING:
				p->mRadius	= 0x1;//0x80+(TELEPORT_RING_WIDTH_ADDER*TELEPORT_RING_LIFE);
				p->mLife	= TELEPORT_RING_LIFE;
				p->uPRing.mBrightness = 0x1000;
				p->uPRing.mSpin = 0;
				p->uPRing.mHeight = 0x100;
				p->uPRing.mSegs = 14;
//				p->uPRing.mSegs = 7;
				p->uPRing.mStep=(4096/14);
				p->uPRing.mWidth=0;
				p->mVelocity.X.mVal=0;
				p->mVelocity.Y.mVal=0;
				p->mVelocity.Z.mVal=0x00000400;
				p->uPColumn.mTextureNum=0;
				break;
/*			case PART_TELEPORTSPARK :
				{
					p->mColour=0xffffff;
					p->mRadius	= 40;
					p->mLife	= 8;

					for(int s=0;s<8;s++)
					{
						p->uPSpark.mQueue[s].vx=0;
						p->uPSpark.mQueue[s].vy=0;
						p->uPSpark.mQueue[s].vz=0;
					}


					TSPoffs = 0;
					TSPradius = 0x1;
					p->uPSpark.mVel.vz=-0x010;
				}
				break;*/

			case PART_SHOCKWAVE :
			{
					p->mLife=SHOCKWAVE_LIFE;
					p->mRadius=0x0;
					p->mVelocity.X.mVal=SHOCKWAVE_INITIAL_GROWTH;
					p->mColour = 0xffff20;
					p->uPAnim.mBrightness = 0x1000;
			}
			break;





			case PART_LIGHTNING:
				break;

			case PART_LIGHT :
				p->mRadius	= 32;
				p->mLife	= 2;
				break;


			default:
				ASSERT(0);
			}

			return p;
		}
		p++;
	}

	return NULL;
}


//************************************************************************
void	CParticleSystem::MoveAllParticles(GVector* by)
{

	CParticle	*p = mParticles;
	for (SINT c0 = 0 ; c0 < MAX_PARTICLES; c0 ++)
	{
		if (p->mType != PART_NONE)
		{
			p->mPos += *by;
		}
		p ++;
	}
}




//************************************************************************
BOOL	CParticleSystem::Process()
{
	VALIDATE(mOwner);                   // Make sure the owner is still valid....

	if (!mActive)
		return TRUE;

	GINT wh;
	wh=GAME.GetLevelData()->mGVars[WATER_HEIGHT];
	SINT		c0;
	CParticle	*p = mParticles;

	int ValidOwner=0;
	if(mOwner)ValidOwner=1;             // We've got a valid owner (for particles to move relative to)...

	int		activecount=0;
	for (c0 = 0 ; c0 < MAX_PARTICLES; c0 ++)
	{
		if (p->mType != PART_NONE)
		{
			activecount++;

			if(p->mDelay)p->mDelay--;
			else
			{
				// default stuff

				p->mLife --;

				if (p->mLife == 0)	// kill it?
					p->mType = PART_NONE;
				else
				{
					// type specific processing
					switch (p->mType)
					{
					case PART_FIRE1:
					case PART_FIRE2:
					case PART_FIRE3:
						{
							p->mPos += p->mVelocity;
							SINT	col;
							if (p->mLife < 7)
								col = p->mLife * 30;
							else
								col = 256 - (p->mLife * 8);
							p->mColour = (col << 16) + (col << 8) + col;

							p->mRadius = (((30-p->mLife)*1500)+3000)>>8;
							//p->mRadius = G(0, 1500 * (30 - p->mLife)) + G(0, 30000);
						}
						break;

					case PART_BODY_FIRE:
						{
							p->mPos += p->mVelocity;
							SINT	col;
							if (p->mLife < 7)
								col = p->mLife * 30;
							else
								col = 256 - (p->mLife * 8);
							p->mColour = (col << 16) + (col << 8) + col;
							p->mRadius = (((20-p->mLife)*1800 )+3000)>>8;
						}
						break;

					case PART_Z:
						{
							p->mPos += p->mVelocity;
							SINT	col;

							col = p->mLife * 5;
							p->mColour = (col << 16) + (col << 8) + col;

	//						p->mRadius = G(0, 1500 * (30 - p->mLife)) + G(0, 30000);

							p->mVelocity.X =   GSIN(G(p->mLife) / 6) / 10;
	//						p->mVelocity.Y =  -GCOS(G(p->mLife) / 6) / 10;
						}
						break;
					case PART_RING :
					{
							if(ValidOwner)
							p->uPRing.mOffs+=p->uPRing.mSpin;
							p->uPRing.mBrightness = p->mLife *32;
							p->mRadius=p->mLife *10;

					}
						break;
					case PART_RAY :
					{
							p->uPRay.mOffs+=p->uPRay.mSpin;
							p->uPRay.mBrightness=p->mLife<<7;

					}
						break;
					case PART_COLUMN :
					{
							p->uPColumn.mOffs+=p->uPColumn.mSpin;
							p->mRadius+=64;
							p->uPRing.mBrightness=p->mLife<<7;

					}
						break;
					case PART_NECOLUMN :
					{
							p->uPColumn.mOffs+=p->uPColumn.mSpin;
							SINT life=p->mLife;
							if(life>68)life=(100-life);
							life<<=8;
							p->uPRing.mBrightness=life;

					}
						break;
					case PART_WATERRING :
					{
							p->uPRing.mBrightness=p->mLife*163;
							p->uPRing.mOffs+=p->uPRing.mSpin;
							p->mRadius=(25-p->mLife)<<4;

					}
						break;
					case PART_WATERRING_SMALL :
					{
							p->uPRing.mBrightness=p->mLife*220;
							p->uPRing.mOffs+=p->uPRing.mSpin;
							p->mRadius=(12-p->mLife)<<3;

					}
						break;
					case PART_STAR1 :
					{
							p->mPos += p->mVelocity;
							p->uPStar.mBrightness=p->mLife<<7;
							p->mRadius --;
					}
						break;
					case PART_ANIM :
					{
							p->mPos+=p->mVelocity;
							p->mLife++;     // reset the mLife so it doesn't die until the end of the anim...
							p->uPAnim.mTick--;
							if(!p->uPAnim.mTick)
							{
								if(--p->uPAnim.mFrames)
								{
									p->uPAnim.mTick=p->uPAnim.mRate;
									p->uPAnim.mFrame++;
									// Only the wated does this...
									if(p->uPAnim.mHoriz==2)
										p->uPAnim.mBrightness = waterbright[p->uPAnim.mFrames];//(p->uPAnim.mFrames) << 9;
									else
										p->uPAnim.mBrightness = (p->uPAnim.mFrames) << 8;
								}
								else
								p->mType=PART_NONE;  // Kill me! anim finished!

							}
					}
						break;
					case PART_SMOKE :
					{
							p->mPos+=p->mVelocity;
							p->uPAnim.mBrightness = (2048/SMOKE_LIFE)*p->mLife;
					}
						break;
					case PART_FLASH :
					{
							//p->mPos+=p->mVelocity;
							p->uPAnim.mBrightness = (4096/FLASH_LIFE)*p->mLife;
					}
						break;

					case PART_HEMI :
					{
//							p->mRadius+=4;
//							p->uPHemi.mBrightness=p->mLife<<7;

//							p->mRadius+=8;
							p->mRadius+=3;
							p->uPHemi.mBrightness=p->mLife<<8;
					}
						break;

					case PART_SPARK :
					{
							// Shift the queue down....
							for(int i=2-1;i>0;i--)
									p->uPSpark.mQueue[i]=p->uPSpark.mQueue[i-1];

//							p->uPSpark.mVel.vz+=0x30;
//							p->uPSpark.mVel.vz+=0x18;
							p->uPSpark.mVel.vz+=0x03;

							p->uPSpark.mQueue[0].vx+=p->uPSpark.mVel.vx;
							p->uPSpark.mQueue[0].vy+=p->uPSpark.mVel.vy;
							p->uPSpark.mQueue[0].vz+=p->uPSpark.mVel.vz;
//							p->uPSpark.mBrightness=p->mLife<<9;
							p->uPSpark.mBrightness=(p->mLife << 7) + 3071;
					}
						break;
					case PART_SPLASH :
					{
							// Shift the queue down....
							for(int i=2-1;i>0;i--)
									p->uPSpark.mQueue[i]=p->uPSpark.mQueue[i-1];

//							p->uPSpark.mVel.vz+=0x30;
							p->uPSpark.mVel.vz+=0x18;

							p->uPSpark.mQueue[0].vx+=p->uPSpark.mVel.vx;
							p->uPSpark.mQueue[0].vy+=p->uPSpark.mVel.vy;
							p->uPSpark.mQueue[0].vz+=p->uPSpark.mVel.vz;
							p->uPSpark.mBrightness=p->mLife<<8;


						#if 0
						if(p->mLife==1)
						{
							GINT h;
							GVector pp=p->mPos;
							pp.X.mVal+=p->uPSpark.mQueue[0].vx<<2;//(GINT_SHIFT-PSXENGINE_SCALE));
							pp.Y.mVal+=p->uPSpark.mQueue[0].vy<<2;//(GINT_SHIFT-PSXENGINE_SCALE));
							pp.Z=wh;
							h=MAP.GetMapHeight(pp);
							if(h>wh)
							{
								p->mType	= PART_WATERRING;
								p->mRadius	= 0;
								p->mLife	= 25;
								p->mColour	= 0x00ffffff;  // Cyan
								p->uPRing.mBrightness = 250;
								p->uPRing.mSpin = (rand()&8)?64:-64;
								p->uPRing.mWidth = 110;
								p->uPRing.mHeight = 0;
								p->uPRing.mSegs = 14;
								p->uPRing.mStep=(4096/14);
								p->mPos=pp;
							}
						}
						#endif
					}
						break;

					case PART_SPINSPARK :
					{
							// Shift the queue down....
							for(int i=4-1;i>0;i--)
									p->uPSpark.mQueue[i]=p->uPSpark.mQueue[i-1];

//							p->uPSpark.mVel.vz+=0x30;
//							p->uPSpark.mVel.vz+=0x08;
							p->uPSpark.mVel.vz+=0x04;

							p->uPSpark.mQueue[0].vx+=p->uPSpark.mVel.vx;
							p->uPSpark.mQueue[0].vy+=p->uPSpark.mVel.vy;
							p->uPSpark.mQueue[0].vz+=p->uPSpark.mVel.vz;
							p->uPSpark.mBrightness=p->mLife<<8;
					}
						break;



					case PART_WRING2:
						{
							p->mPos += p->mVelocity;
							SINT	col;

							col = 1000-p->mLife;
							if(col>50)col=50;
							col = col * 5;
							p->mColour = (col << 16) + (col << 8) + col;


							SINT	wh=GAME.GetLevelData()->mGVars[WATER_HEIGHT].GetVal();
							if((p->mLife<998)&&(p->mPos.Z.mVal<=wh))
							{
								p->mLife=12;
								p->mPos.Z.mVal=wh;
								p->mVelocity.Z.mVal=0;
								p->mVelocity.X.mVal=0;
								p->mType=PART_WATERRING_SMALL;
								p->mRadius	= 0;
								p->mLife	= 12;
								p->mColour	= 0x00ff7f00;  // Cyan
								p->uPRing.mBrightness = 120;
								p->uPRing.mSpin = (rand()&8)?280:-280;
								p->uPRing.mWidth = 60;
								p->uPRing.mHeight = 0;
								p->uPRing.mSegs = 7;
								p->uPRing.mStep=(4096/7);

							}
							else
							{
								if(p->mVelocity.Z.mVal>-0x1600)p->mVelocity.Z.mVal-=0x100;
								//p->mRadius = G(0, 1500 * (60 - p->mLife)) + G(0, 30000);
								p->mVelocity.X =   GSIN(G(p->mLife) / 2) / 10;
							}
						}
						break;


					case PART_TELEPORTINCOLUMN :
					{
							p->mRadius+=TELEPORT_COLUMN_WIDTH_ADDER;
							p->uPRing.mOffs+=p->uPRing.mSpin;
//							p->uPRing.mBrightness=(0x1400/TELEPORT_COLUMN_LIFE)*(TELEPORT_COLUMN_LIFE-p->mLife);
							p->uPRing.mBrightness=(0x1400/TELEPORT_COLUMN_LIFE)*(p->mLife);
							if(p->uPRing.mBrightness>0x1000)p->uPRing.mBrightness=0x1000;

					}
					break;
					case PART_TELEPORTINRING :
					{
						// Not hit floor yet....
						SINT bright=(0x1000/TELEPORT_RING_LIFE)*p->mLife;
						if(bright>0x7000)bright=0x7000;
						p->uPRing.mBrightness=bright;

						if(p->mVelocity.Z.mVal!=0)
						{
							int z=MAP.GetMapHeight(p->mPos).mVal - 65536;
							p->uPRing.mOffs+=64;
							//p->mRadius-=TELEPORT_RING_WIDTH_ADDER;
							//if(p->uPRing.mBrightness>0)p->uPRing.mBrightness-=0x80;
							if(p->mRadius<0x80+(TELEPORT_RING_WIDTH_ADDER*TELEPORT_RING_LIFE))p->mRadius+=0x60;
							p->mPos += p->mVelocity;
							p->mVelocity.Z.mVal+=TELEPORT_VELOCITY_ADDER;
							if(p->mPos.Z.mVal>z)
							{
								p->uPRing.mSegs = 16;
								p->uPRing.mStep=(4096/16);
								p->mPos.Z.mVal=z-0x120;
								p->uPRing.mHeight=0;
								p->uPRing.mWidth=0x60;
								p->mVelocity.Z.mVal=0;  // We're expanding now...
								p->mColour=0xffff00;
							}
						}
						else
						{
							p->uPRing.mWidth+=(p->uPRing.mWidth>>2);
						}

					}
					break;

/*					case PART_TELEPORTSPARK :
					{
							{
								// Shift the queue down....
								for(int i=7;i>0;i--)
										p->uPSpark.mQueue[i]=p->uPSpark.mQueue[i-1];
								TSPoffs+=280;
								if(TSPradius<0x100)
								TSPradius+=0x10;

								short sn=rsin(TSPoffs);
								short cs=rcos(TSPoffs);

								p->uPSpark.mQueue[0].vx=((sn)*(TSPradius))>>11;
								p->uPSpark.mQueue[0].vy=((cs)*(TSPradius))>>11;
								SINT vel=p->uPSpark.mVel.vz;
								p->uPSpark.mQueue[0].vz+=vel;
								if(vel<0x200)
								p->uPSpark.mVel.vz+=(vel>>4);
								p->uPSpark.mBrightness=0x0fff;
							}
					}*/
						break;
					case PART_SHOCKWAVE :
					{
						p->mRadius+=p->mVelocity.X.mVal;
						p->mVelocity.X.mVal-=SHOCKWAVE_GROWTH_DECEL;
						p->uPAnim.mBrightness=(4096/SHOCKWAVE_LIFE)*p->mLife;
						p->uPAnim.mHoriz=1;
						p->uPAnim.mFrame=TP_SHOCKWAVE;
					}
					break;
					case PART_LIGHTNING :
					{
						p->mRadius=0x100;
					}
						break;

					case PART_LIGHT :
						p->mColour-=(p->mColour>>1);
						break;


					default:
						break;
					}
				}
			}
		}
		p ++;
	}
	//if(!activecount)mActive=FALSE;      // Kill off if no active particles left...

	// reinserted JCL6/3
	if(!activecount)
		return TRUE;      // stop
	else
		return FALSE;
}

//************************************************************************
//************************************************************************
int randcol(void)
{
	int v=rand();
	int col=0;
	if(v&4)col|=0xff;
	if(v&8)col|=0xff00;
	if(v&16)col|=0xff0000;
	if(v&32)col^=(rand()&0x00ffffff);
	return col;
}

// JCL - Damian - what was this supposed to do????????????????/

//#define SRAND ((16384-rand())<<1)
#define SIGNEDRAND (((SINT(rand()) & 0xfff) - 0x7ff) << 4)

//************************************************************************
void CParticleSystem::MakeSplash(GVector *pos)
{
	for(int i=0;i<14;i++)
	{
		CParticle *ptcl;
		ptcl = CreateParticle(PART_SPLASH);
		if(ptcl)
		{
			ptcl->mPos.X=pos->X;
			ptcl->mPos.Y=pos->Y;
			ptcl->mPos.Z.mVal=pos->Z.mVal;  //-(0x00010000);

			int 	r = rand();
			SINT	rad = (rand() % 7) + 8;
			ptcl->uPSpark.mVel.vx=(ccos(r) * rad) >> (6 + 4);
			ptcl->uPSpark.mVel.vy=(csin(r) * rad) >> (6 + 4);
			ptcl->uPSpark.mVel.vz=-0x80;

			//ptcl->uPSpark.mVel.vz=-0x080;
			ptcl->mColour=0x00f0e000;
		}
	}
}

//************************************************************************
void	CParticleSystem::MakeSplash2(GVector *pos, SINT num, GINT baserad, GINT spread)
{
	for(int i=0;i<num;i++)
	{
		CParticle *ptcl;
		ptcl = CreateParticle(PART_SPLASH);
		if(ptcl)
		{
			ptcl->mPos.X=pos->X;
			ptcl->mPos.Y=pos->Y;
			ptcl->mPos.Z.mVal=pos->Z.mVal;  //-(0x00010000);

			int 	r = (((G(rand() % 4095) * spread)) + (baserad / (G_PI * 2) * 4096) - (spread * G(4096) / 2)).Whole();
			SINT	rad = (rand() % 7) + 8;
			ptcl->uPSpark.mVel.vx=(ccos(r) * rad) >> (6 + 4);
			ptcl->uPSpark.mVel.vy=(csin(r) * rad) >> (6 + 4);
			ptcl->uPSpark.mVel.vz=-0x80;

			//ptcl->uPSpark.mVel.vz=-0x080;
			ptcl->mColour=0x00f0e000;
		}
	}
}
//************************************************************************
void CParticleSystem::MakeSparks(GVector *pos)
{
	for(int i=0;i<5;i++)
	{
		CParticle *ptcl;
		ptcl = CreateParticle(PART_SPLASH);
		if(ptcl)
		{
			ptcl->mPos.X=pos->X;
			ptcl->mPos.Y=pos->Y;
			ptcl->mPos.Z.mVal=pos->Z.mVal;  //-(0x00010000);

			int 	r = rand();
			SINT	rad = (rand() % 7) + 8;
			ptcl->uPSpark.mVel.vx=(ccos(r) * rad) >> (6 + 4);
			ptcl->uPSpark.mVel.vy=(csin(r) * rad) >> (6 + 4);
			ptcl->uPSpark.mVel.vz=-0x80;

			//ptcl->uPSpark.mVel.vz=-0x080;
			ptcl->mColour=0x00ffffff;
		}
	}
}

				 //************************************************************************

#define setpxyz(a,b) {\
	SINT tx=b->X.mVal;\
	SINT ty=b->Y.mVal;\
	SINT tz=b->Z.mVal;\
	a->mPos.X.mVal=tx;\
	a->mPos.Y.mVal=ty;\
	a->mPos.Z.mVal=tz;\
}



void	CParticleSystem::MakeHitEffect(GVector *pos, GINT baserad,EeHitType type)
{
	SINT	i;

	baserad += G_PI_2;
	CParticle *p;


	switch(type)
	{

		case HT_NORMAL :
		case HT_BLOCK :
		case HT_JUMP :
		case HT_BULLETCHARACTER :
		case HT_BALLGROUND :
		case HT_WEAPON :
		case HT_LASERED:
		{


			p=CreateParticle(PART_FLASH);
			if(p)
			{
				SINT frame;
				switch (type)
				{
				case HT_BLOCK:
					frame = TP_HIT2;
					break;
				
				case HT_LASERED:
					frame = (rand()&1)?TP_HIT1:TP_HIT2;
					break;
					
				default:
					frame = TP_HIT1;
					break;
				};
				
				p->uPAnim.mFrame=frame;
				p->mRadius=0x90;
				setpxyz(p,pos);
			}



			for(i=0;i<8;i++)
			{

				short xvel=ccos((baserad / (G_PI * 2) * G(4096)).Whole()) >> 8;
				short yvel=csin((baserad / (G_PI * 2) * G(4096)).Whole()) >> 8;

				p = CreateParticle(PART_SPARK);
				if(p)
				{
					setpxyz(p,pos);

					p->uPSpark.mVel.vx=xvel + (rand() & 0x1f) - 0x10;
					p->uPSpark.mVel.vy=yvel + (rand() & 0x1f) - 0x10;
					p->uPSpark.mVel.vz=(rand() & 0x3f) - 0x30;

					SINT	c = rand() & 0xff;
					p->mColour = 0x0000ffff + (c << 16);
				}
			}

			for(i = 0; i < 3; i ++)
			{

				p=CreateParticle(PART_STAR1);
				if(p)
				{
					SINT angle=(rand()&4095);
					p->mColour=randcol()|0x80;
					setpxyz(p,pos);
					p->mRadius=0x30;
					p->mVelocity.Z.mVal = rand() % 0x1fff - 0x1000;
					p->mVelocity.X.mVal = rand() % 0x1fff - 0x1000;
					p->mVelocity.Y.mVal = rand() % 0x1fff - 0x1000;

					p->mLife	= 13;
				}
			}


			break;
		};

//		case HT_WEAPON :
//			printf("Ht by WEAPON!\n");
		case HT_BULLETGROUND :
		{

			p=CreateParticle(PART_SHOCKWAVE);
			if(p)
			{
//				printf("New Shockwave\n");
//				printf("X: 0x%08x Y:0x%08x Z: 0x%08x\n",pos->X.mVal,pos->Y.mVal,pos->Z.mVal);

				setpxyz(p,pos);
				p->mPos.Z.mVal-=0x10000;
				p->mColour=0xffffff;
			}
			break;
		}

//		default: printf("Incorrect explosion type!\n");while(1);
	}



}

//************************************************************************
void CParticleSystem::MakeExplosion1(GVector *pos)
{
	// Add an explosion to the particle system.

	CParticle *p;
	int c0;
	for(c0=0;c0<8;c0++)
	{
		p=CreateParticle(PART_ANIM);
		if(p)
		{
			p->mColour=0x00808080;
			p->mPos.X.mVal=pos->X.mVal+(SIGNEDRAND << 1);
			p->mPos.Y.mVal=pos->Y.mVal+(SIGNEDRAND << 1);
			p->mPos.Z.mVal=pos->Z.mVal-(0x10000+(SIGNEDRAND << 1));
			p->uPAnim.mFrame=TP_EXP00;
			p->uPAnim.mRate=1;
			p->mRadius=256-(c0<<3);
			p->uPAnim.mFrames=16;
			p->mDelay=c0<<2;
			p->uPAnim.mLoops=0;
			p->uPAnim.mHoriz=0;
		}
	}

	for(c0=0;c0<5;c0++)
	{
		p=CreateParticle(PART_STAR1);
		if(p)
		{
			SINT angle=(rand()&4095);
			p->mColour=randcol()|0x80;
			p->mPos.X=pos->X;
			p->mPos.Y=pos->Y;
			p->mRadius=0x20;
			p->mPos.Z.mVal=pos->Z.mVal-0x00010000;
			p->mVelocity.Z.mVal=-0x4000;
			p->mVelocity.X.mVal=csin(angle)<<1;
			p->mVelocity.Y.mVal=ccos(angle)<<1;
			p->mDelay = rand() % 12;
		}
	}


}
//************************************************************************
void CParticleSystem::MakeExplosion1Small(GVector *pos)
{
	// Add an explosion to the particle system.

	CParticle *p;
	int c0;
	for(c0=0;c0<3;c0++)
	{
		p=CreateParticle(PART_ANIM);
		if(p)
		{
			p->mColour=0x00808080;
			p->mPos.X.mVal=pos->X.mVal+(SIGNEDRAND << 1);
			p->mPos.Y.mVal=pos->Y.mVal+(SIGNEDRAND << 1);
			p->mPos.Z.mVal=pos->Z.mVal-(0x10000+(SIGNEDRAND << 1));
			p->uPAnim.mFrame=TP_EXP00;
			p->uPAnim.mRate=1;
			p->mRadius=192-(c0<<3);
			p->uPAnim.mFrames=16;
			p->mDelay=c0<<2;
			p->uPAnim.mLoops=0;
			p->uPAnim.mHoriz=0;
		}
	}

	for(c0=0;c0<4;c0++)
	{
		p=CreateParticle(PART_STAR1);
		if(p)
		{
			SINT angle=(rand()&4095);
			p->mColour=randcol()|0x80;
			p->mPos.X=pos->X;
			p->mPos.Y=pos->Y;
			p->mRadius=0x18;
			p->mPos.Z.mVal=pos->Z.mVal-0x00010000;
			p->mVelocity.Z.mVal=-0x4000;
			p->mVelocity.X.mVal=csin(angle)<<1;
			p->mVelocity.Y.mVal=ccos(angle)<<1;
			p->mDelay = rand() % 12;
		}
	}


}


//************************************************************************
void CParticleSystem::MakeExplosion2(GVector *pos)
{
	CParticle *p;

	p=CreateParticle(PART_HEMI);
	if(p)
	{
//		p->mDelay=8;
		p->mPos=*pos;
		p->mColour=0x002000ff;
		p->mLife-=(p->mLife>>2);
	}
	p=CreateParticle(PART_COLUMN);
	if(p)
	{
		p->mDelay=4;
		p->mPos=*pos;
		p->mPos.Z -= G(2);
		p->mColour=0x0000ffff;
		p->uPColumn.mHeight = 2048;
		p->uPColumn.mWidth=0;
	}
	p=CreateParticle(PART_COLUMN);
	if(p)
	{
		p->mDelay=4;
		p->mPos=*pos;
		p->mPos.Z -= G(2);
		p->mColour=0x0000ffff;
		p->uPColumn.mHeight = -512;
		p->uPColumn.mWidth=0;
	}

	p=CreateParticle(PART_SHOCKWAVE);
	setpxyz(p,pos);
	p->mPos.Z.mVal-=0x10000;


}







