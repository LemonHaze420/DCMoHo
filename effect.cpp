#include	"Common.h"

#include	"Effect.h"
#include	"Map.h"
#include	"Globals.h"
#include	"Character.h"


//******************************************************************************************
void	CFXLightning::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	ASSERT(!(WORLD.IsReading()));  // we're buggered if it is....

	GVector	start, end;

	if (WORLD.IsReading())
		end = ZERO_GVECTOR;
	else
	{
		CThing	*t;
		WORLD.GetThing(i->mTarget, &t);
		if (t)
			end = t->GetPos() + t->GetRenderThingEmitter(0);
		else
			end = GVector(G(32), G(32), G0);
	}

	SetStartAndEnd(i->mPos, end);

	mLife = 20;
}

//******************************************************************************************
void	CFXLightning::SetStartAndEnd(GVector &start, GVector &end)
{
	GVector	avg = (start + end) / 2;

	Move(avg);
	mStart = start;
	mEnd = end;

/*	// clip start and end points to the map...
	if (!(COORDS_INVALID(CCellRef(mStart))))
		mStart.Z = MAP.GetMapHeight(mStart);
	if (!(COORDS_INVALID(CCellRef(mEnd))))
		mEnd.Z = MAP.GetMapHeight(mEnd);*/
}


//******************************************************************************************
void	CFXLightning::Process()
{
	mLife--;

	if (mLife == 0)
		Suicide();

	SUPERTYPE::Process();
}

//******************************************************************************************
//******************************************************************************************

void	CEffectParticle::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mParticleSystem = NULL;

	// fade in sample
	SINT	s = GetSample();
	if (s != -1)
		SOUND.PlaySample(s, this, G(0, 20000), TRUE, TRUE, G(0, 3000));
}

//******************************************************************************************
void	CEffectParticle::Process()
{
	SINT	s = GetSample();
	if ((s != -1) && (mLife == InitialLife()))
		if (((GAME.GetGameTurn() + mNumber) & 15) == 0)
			SOUND.PlaySample(s, this, G(0, 20000), TRUE, TRUE, G(0, 3000));


	if (GAME.CanIBeSeen(mPos) || (_GetClassID() == OID_CFXTeleportIn))
	{
		if (!mParticleSystem)
		{
			mParticleSystem = WORLD.GetParticleSystem();
			if (mParticleSystem)
				mParticleSystem->Activate();
		}

		if (mLife == InitialLife())
		{
			if (mParticleSystem)
				CreateParticles();
		}
		else
		{
			mLife --;
			{
				if (mLife == 0)
				{
					if (mParticleSystem)
						mParticleSystem->DeActivate();   // we don't need it any more
					mParticleSystem = NULL;

					Suicide();

				}
			}
		}

		if (mParticleSystem)
		{
			mParticleSystem->Process();
		}
	}
	else
	{
		if (mParticleSystem)
		{
			mParticleSystem->DeActivate();   // we don't need it any more
			mParticleSystem = NULL;
		}
	}

	SUPERTYPE::Process();
}

//******************************************************************************************
void	CEffectParticle::Die()
{
	mLife --; // start death sequence...

	SINT 	s = GetSample();
	if (s != -1)
		SOUND.FadeTo(s, G0, G(0, 3000), this);
}

//******************************************************************************************
//******************************************************************************************


//******************************************************************************************
void	CFXStreamer::Init(CInitThing *i)
{
	SUPERTYPE::Init(i) ;
	mXRandomStartOffset = G0;
	mYRandomStartOffset = G0;
	mZRandomStartOffset = G0 ;
}

//******************************************************************************************
void	CFXStreamer::CreateParticles()
{
	SINT freq = NewParticleFrequency() ;
	if (((GAME.GetGameTurn() % freq) == 0) && (mParticleSystem))
	{
		// create a particle!
		CParticle *p = mParticleSystem->CreateParticle(ParticleType());

		if (p)
		{
			p->mPos = mPos;

			p->mPos += GVector( (G(0, RANDOM() % 32768) - G(0, 16384)) * G(4) * mXRandomStartOffset ,
							    (G(0, RANDOM() % 32768) - G(0, 16384)) * G(4) * mYRandomStartOffset,
								(G(0, RANDOM() % 32768) - G(0, 16384)) * G(4) * mZRandomStartOffset);
		//	p->mPos.X+=(G(RANDOM()%4)) - G(3) ;
		//	p->mPos.Y+=(G(RANDOM()%4)) - G(3);
			p->mVelocity = (mOrientation * GVector( G(0, (RANDOM() % 2000)) - G(0, 1000),
												   YParticleSpeed(),
												   G(0, (RANDOM() % 2000)) - G(0, 1000)));
		}
	}
}


//******************************************************************************************
void	CFXStreamer::MoveStreamerAndParticles(GVector* new_pos)
{
	if (mParticleSystem)
	{
		GVector movment = *new_pos - mPos ;
		mParticleSystem->MoveAllParticles(&movment);
		Move(*new_pos) ;
	}
}


//******************************************************************************************
//******************************************************************************************

#if 0

void	CFXZ::CreateParticles()
{
	if ((((GAME.GetGameTurn() + mNumber * 43) & 15) == 0) && (mParticleSystem))
	{
		// create a particle!

		CParticle*p = mParticleSystem->CreateParticle(PART_WATERRING);

		if (p)
		{
			p->mPos = mPos +  GVector(G(0, (RANDOM() % 10000)) - G(0, 5000), G(0, (RANDOM() % 10000)) - G(0, 5000), G0);
			//p->uPFire.mVelocity = GVector(G0, G0, -G(0, 4000));
		}
	}
}

#endif

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
void	CFXExplosion1::CreateParticles()
{
	mParticleSystem->MakeExplosion1(&mPos);

	// do damage staright away
	CThing* a_thing_near_by_in_map_who  = FindFirstCollision(mPos, G(6)) ;
	while (a_thing_near_by_in_map_who)
	{
		CCharacter* a_character_near_by_in_map_who = a_thing_near_by_in_map_who->IsCharacter() ;

		if ( a_character_near_by_in_map_who)
		{
			// get dist
			GVector me_to_that = a_character_near_by_in_map_who->GetPos() - mPos ;
			GINT dist = me_to_that.Magnitude() ;
			if (dist < G(6))
			{
				GINT damage = (G(0,8123) + ((G(6) - dist) / G(24)))  ;
				a_thing_near_by_in_map_who->Damage(damage) ;
				me_to_that.Normalise() ;
				me_to_that*=damage ;
				me_to_that/=G(1,32123) ;
				a_character_near_by_in_map_who->AddVelocity(me_to_that) ;
			}
		}
		a_thing_near_by_in_map_who = FindNextCollision() ;
	}

	mLife --;	// trigger death sequence
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
void	CFXExplosion2::CreateParticles()
{
	mParticleSystem->MakeExplosion2(&mPos);

	// wibble the land?
	if (mRippleLandscape)
	{
		MAP.Raindrop(CCellRef(mPos), G(0, 40000));
	}

	mLife --;	// trigger death sequence

	// sound?
	SOUND.PlaySample(SOUND_EXPLODE3, this, 50000); //was default
}



//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
void	CFXBodyFlamer::Process()
{
	if (RANDOM()%3 == 0) mNewParticleFrequency+=1 ;
	SUPERTYPE::Process() ;
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
void	CFXTeleportIn::CreateParticles()
{
	// mParticleSystem->Add  start stuff

	SOUND.PlaySample(SOUND_TELEPORT, this, G(0, 25000));

	SINT	inc = 1;

	if (GAME.IsMultiPlayer())
		inc = 2; 		// less stuff in Multiplayer

	CParticle *p;
	int i;

	p=mParticleSystem->CreateParticle(PART_TELEPORTINCOLUMN);
	if(p)
	{
		//p->mColour=0x00408020;
		p->mColour=0xff2020;
		p->mPos.X.mVal=mPos.X.mVal;
		p->mPos.Y.mVal=mPos.Y.mVal;
		p->mPos.Z.mVal=mPos.Z.mVal;
		p->uPColumn.mTextureNum=TP_FIRE3|PART_TILEH|PART_TILEV;
	}

	p=mParticleSystem->CreateParticle(PART_TELEPORTINCOLUMN);
	if(p)
	{
//		p->mColour=0x00408020;
		p->mColour=0x20ff20;
		p->mPos.X.mVal=mPos.X.mVal;
		p->mPos.Y.mVal=mPos.Y.mVal;
		p->mPos.Z.mVal=mPos.Z.mVal;
		p->uPColumn.mTextureNum=TP_FIRE3|PART_TILEH|PART_TILEV;
		p->uPColumn.mSpin=-p->uPColumn.mSpin;
	}



	for(i=0;i<4;i += inc)
	{
		p=mParticleSystem->CreateParticle(PART_TELEPORTINRING);
		if(p)
		{
				p->mDelay=i*3;
//				p->mColour=0xff00ff;
//				p->mColour=0x1fff1f;
				p->mColour = (((7 - i) << 5) << 16) + (((i) << 5) << 8);
				p->mColour=((p->mColour>>1)%0x7f7f7f);


				//p->uPRing.mWidth=0x80;
				p->mPos.X.mVal=mPos.X.mVal;
				p->mPos.Y.mVal=mPos.Y.mVal;
				p->mPos.Z.mVal=mPos.Z.mVal-0x40000;
				//p->mColour|=0x01808080;
				//p->uPColumn.mTextureNum=TP_EXP08;
		}
	}
	mLife --;	// trigger death sequence
}

//******************************************************************************************
void	CFXTeleportIn::Process()
{
	if (((InitialLife() - mLife)  % 8) == 0)
	{
		if (mParticleSystem)
		{
			// mParticleSystem->Add whatever
		}
	}

	SUPERTYPE::Process() ;
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
void	CFXRageAttack::CreateParticles()
{

	SINT	inc = 1;

	if (GAME.IsMultiPlayer())
		inc = 2; 		// less stuff in Multiplayer


	if(mParticleSystem)mParticleSystem->mOwner=mOwner;
	CParticle*p=mParticleSystem->CreateParticle(PART_NECOLUMN);
	if(inc==1)
	{
		if(p)
		{
				p->mRadius=0x100;
				p->mLife=100;
				p->mIsRelative=TRUE;
				p->mColour=0xffffff;
				p->uPColumn.mWidth=0x100;
				p->uPColumn.mHeight=0x400;
				p->mPos.X.mVal=
				p->mPos.Y.mVal=
				p->mPos.Z.mVal=0;
				p->uPColumn.mSpin=0x80;
				p->uPColumn.mTextureNum=TP_INNER;
		}
	}
	p=mParticleSystem->CreateParticle(PART_NECOLUMN);
	if(p)
	{
			p->mRadius=0x100;
			p->mLife=100;
			p->mIsRelative=TRUE;
			p->mColour=0xa0a0a0;
			p->uPColumn.mWidth=0x200;
			p->uPColumn.mHeight=0x400;
			p->mPos.X.mVal=
			p->mPos.Y.mVal=
			p->mPos.Z.mVal=0;
			p->uPColumn.mSpin=-0x80;
			p->uPColumn.mTextureNum=TP_MIDDLE;
	}
	p=mParticleSystem->CreateParticle(PART_NECOLUMN);
	if(p)
	{
			p->mRadius=0x100;
			p->mLife=100;
			p->mIsRelative=TRUE;
			p->mColour=0x808080;
			p->uPColumn.mWidth=0x400;
			p->uPColumn.mHeight=0x400;
			p->mPos.X.mVal=
			p->mPos.Y.mVal=
			p->mPos.Z.mVal=0;
			p->uPColumn.mSpin=0x80;
			p->uPColumn.mTextureNum=TP_OUTER;
	}


/*	for(int j=0;j<4;j++)
	{
		for(int i=0;i<4;i += inc)
		{
			p=mParticleSystem->CreateParticle(PART_TELEPORTSPARK);
			if(p)
			{
				if(i&inc)p->uPSpark.mVel.vz=-0x010;
				else p->uPSpark.mVel.vz=0x010;

				p->mIsRelative=1;
				p->mLife=60;
				p->mDelay=20+(i<<1)-(i>>1)+(j*10);
				p->mColour=0x00ffffff;
				p->mPos.X.mVal=
				p->mPos.Y.mVal=
				p->mPos.Z.mVal=0;
				p->uPSpark.mVel.vx=(4096/4)*i;

			}
		}
	}
*/



	mLife --;	// trigger death sequence
}

//******************************************************************************************
void	CFXRageAttack::Process()
{
	VALIDATE(mOwner);
	if(mParticleSystem)
	{
		mParticleSystem->mOwner=mOwner;
		if(mOwner)
		{
			CCharacter *c=mOwner->IsCharacter();
			if(c)
			{
				if(c->GetRenderAnimation()!=AM_RAGE_ATTACK)
				{
					for(int i=0;i<MAX_PARTICLES;i++)
					{
						CParticle*p=&mParticleSystem->mParticles[i];
						if(p->mLife<98)
						{
							if(p->mDelay!=0)p->mType=PART_NONE;  // Kill if it's waiting to happen
							if(p->mType==PART_NECOLUMN)
								if(p->mLife<68)
								{
									if(p->mLife>32)
									{
										p->mLife=32;  // Start fading out...
									}
								}
								else(p->mLife=100-p->mLife);
						}
					}
				}
			}
		}
	}

	//if (((InitialLife() - mLife)  % 8) == 0)
	//{
		//if (mParticleSystem)
		//{
		//	// mParticleSystem->Add whatever
		//}
   	//}

	SUPERTYPE::Process() ;
}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

void	CFXExplosion2::Process()
{
	mAge++ ;
	SUPERTYPE::Process() ;
}


//******************************************************************************************
void	CFXExplosion2::Hit(CThing* that)
{
//	CONSOLE.AddString("here!!") ;
	// ok lets do some damage...
	CThing* a_thing_near_by_in_map_who  = FindFirstCollision(mPos, G(6)) ;
	while (a_thing_near_by_in_map_who)
	{
		CCharacter* a_character_near_by_in_map_who = a_thing_near_by_in_map_who->IsCharacter() ;

		if ( a_character_near_by_in_map_who)
		{
			// get dist
			GVector me_to_that = a_character_near_by_in_map_who->GetPos() - mPos ;
			GINT dist = me_to_that.Magnitude() ;
			if (dist < G(6))
			{
				GINT damage = G(0,16123) + ((G(6) - dist) / G(24)) ;
				a_thing_near_by_in_map_who->Damage(damage) ;
				me_to_that.Normalise() ;
				me_to_that*=damage ;
				me_to_that/=G(1,32123) ;
				a_character_near_by_in_map_who->AddVelocity(me_to_that) ;
			}
		}
		a_thing_near_by_in_map_who = FindNextCollision() ;
	}

}

