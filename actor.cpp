#include	"Common.h"

#include	"Actor.h"
#include	"Globals.h"
#include	"Charball.h"

#if TARGET == DC
#include	"DCBuildType.h"
#include	"DCCheats.h"
#endif

#if TARGET == PC
#include	"PCCheats.h"
#endif

//******************************************************************************************
GINT	CActor::GroundFriction(EFrictionType friction_type)
{
	GINT	gf;
	gf = GINT(0);	// are: It was possible for this method to return the value of 'gf' without having initialised it.

	// cheat!
	if (GAME.IsCheatEnabled(CHEAT_SLIPPERY))
		return G(0, 65300);

//	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_TAG)
//		gf = GINT(0, 65069);
//	else
//		gf = GINT(0, 63569);

	switch (friction_type)
	{
		case FT_NORMAL:
			gf = GINT(0, 63569);
			break;
		case FT_FAST:
			gf = GINT(0, 65069);
			break;
		case FT_SLOW:
			gf = GINT(0, 61500); //! check this!
			break;
		case FT_ICE:
			gf = GINT(0, 65300);
			break;
	};

#define	MAGIC	G(0, 3000)

	GINT	magic;

	switch (GAME.GetIVar(IV_LEVEL_TYPE))
	{
		case LT_GOLF:
			magic = G(0, 12000);
			break;

		case LT_NORMAL:
			magic = G(0, 6000);
			break;

		default:
			magic = G(0, 3000);
			break;

	};

	if (friction_type != FT_ICE)
	{
		GINT	n = mVelocity.MagnitudeXY();
		if (n > magic) n = magic;
		n += (G1 - magic);
		return gf * n;
	}
	else
		return gf;
};

//******************************************************************************************
void	CActor::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mVelocity = i->mVelocity;
	mAngularVelocity = ID_GMATRIX;
	mOldPos = mPos;
	mStunned = 0;
	mCurrentMapNormalCalcTime = - 50;
	mCurrentMapNormal = GVector(G0,G0,-G1) ;
	mLastAThing = NULL ;
	mLastAThingTime = -50 ;
	mDFlags=DFC_DrawTrail;                          // Damo's misc flags for effects etc.... (turn 'em all off!)

}

//******************************************************************************************
#if TARGET == PC
void	CActor::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	i.mVelocity = mVelocity;
}
#endif

//******************************************************************************************

#define BOOST_SPEED G1

void	CActor::Process()
{
	mOldPos = mPos;

	// have we fallen off map?
	if ((MAP.FallenOffMap(mPos)) && (mPos.Z > (PIT_HEIGHT * 7) / 8))
		Suicide();

	// Deal with cell types and Damping...
	//! Should this be done in the ground collision function?
	if (!(COORDS_INVALID(CCellRef(mPos))))
	{
		CMapCell *cell = MAP.GetCell(CCellRef(mPos));

		if (mFlags & TF_ON_GROUND)
		{
			if (cell->GetType() != CT_ICE)
			{
				EFrictionType ft = cell->GetFriction();

				mVelocity *= GroundFriction(ft);


			}
		}

		//! hmmmmm
		if (DoICollideWithMap())
		{

			if (mFlags & TF_ON_GROUND)
			{
				BOOL	boost = FALSE;
				GVector boost_vel;

				switch(cell->GetType())
				{
					// any other stuff

				case CT_BOOSTR:
					boost = TRUE;
					boost_vel = GVector( BOOST_SPEED,  G0, G0);
					break;

				case CT_BOOSTL:
					boost = TRUE;
					boost_vel = GVector(-BOOST_SPEED,  G0, G0);
					break;

				case CT_BOOSTU:
					boost = TRUE;
					boost_vel = GVector( G0,  BOOST_SPEED, G0);
					break;

				case CT_BOOSTD:
					boost = TRUE;
					boost_vel = GVector( G0, -BOOST_SPEED, G0);
					break;

				default:
//					boost = false;
					boost_vel = GVector(G0, G0, G0);
				};

				// boost!!
				if (boost)
				{
	//				mVelocity = ((boost_vel - mVelocity ) * 0.6f) + mVelocity;
					mVelocity = boost_vel;

					DeclareBoost(); // trail?
				}

			}
		}
	}

	if (!(mFlags & TF_NOT_ALLOWED_TO_MOVE))
	{
		// Add Gravity
		mVelocity.Z += Gravity();
		// Do velocity.
		Move(mPos + mVelocity);
	}

	// flash?
	if (mStunned > 0)
	{
		mStunned--;
		mFlags |= TF_FLASHING;
	}
	else
		mFlags &= ~TF_FLASHING;



	GVector vel_before_hit = mVelocity;
	BOOL in_air_b4 = !(mFlags & TF_ON_GROUND );

	// Do Collision
	if (!(mFlags & TF_NOT_ALLOWED_TO_MOVE))
		ProcessCollision();

	// wobble ground if we just hit it
	if (DoICollideWithMap() &&
		mFlags & TF_ON_GROUND &&
		in_air_b4 ) //&&
		//GAME.GetIVar(IV_LEVEL_TYPE) != LT_TAG)
	{
		SINT game_turn = GAME.GetGameTurn() ;
		if (mCurrentMapNormalCalcTime != game_turn )
		{
			mCurrentMapNormalCalcTime = game_turn ;
			mCurrentMapNormal = MAP.GetMapNormal(mPos) ;
		}

		GVector vel = vel_before_hit ;
		GINT	dot = mCurrentMapNormal * vel_before_hit  ;
		dot += G(0, 16384);
		if (dot < G0)
		{
			GINT	 damage = dot * vel_before_hit.Magnitude() ;
		//	char temp[50];
		//	sprintf(temp,"damage = %2.2f", GToF(damage)) ;
		//	CONSOLE.AddString(temp) ;

			SOUND.PlaySample(SOUND_LAND, this, DEFAULT_SOUND_VOLUME, FALSE, TRUE);

			// cheat!
			if (!(GAME.IsCheatEnabled(CHEAT_BOUNCY)))
				Damage(-(damage/2));

			MAP.Raindrop(CCellRef(mPos), GetMass() * (GABS(-damage) >> 2));

			if (dot < -G(0, 16384))
			{
				if (IsCharacter())
				{
					((CCharacter*)this)->TriggerVibration(1);
				}
				// $$$ HIT GROUND HARD EFFECT HERE
	#ifndef DEMO_GAME
				BOOL under_water = FALSE ;
				GINT wh;
				wh=GAME.GetLevelData()->mGVars[WATER_HEIGHT];
				if (mPos.Z > wh + Radius() )
				{
					under_water = TRUE ;
				}
	
				CCharacter* c=IsCharacter();
				if(c && (!(mDFlags&DFC_Inwater)))
				{
					CParticleSystem *ps=c->GetParticleSystem();
					if(ps)
					{
	
						for(int i=0;i<4;i++)
						{
							CParticle *p=ps->CreateParticle(PART_SMOKE);
							if(p)
							{
								p->mPos=mPos;
								p->mRadius=128;
	
								int 	r = rand()&4095;
								SINT	rad = (rand() % 7) + 8;
								p->mVelocity.X.mVal=((rcos(r) * rad) >> (6 + 4))<<8;
								p->mVelocity.Y.mVal=((rsin(r) * rad) >> (6 + 4))<<8;
								p->mVelocity.Z.mVal=-0x20<<8;
	
							}
						}
					}
				}
	#endif
			}

		}
	}

	SUPERTYPE::Process();
}

//******************************************************************************************
void	CActor::SetSpin(GVector ax)
{
	GINT	radius_per_dist	= ((G(2) * G_PI) / (G_PI * Radius()));
	GINT	theta			= radius_per_dist * (ax.Magnitude());

	if (theta > GINT(0, 655))
	{
		ax.Normalise();
		mAngularVelocity.MakeGeneralAxisRotation(ax, theta);
	}
	else
		mAngularVelocity.MakeID();
}

//******************************************************************************************
void	CActor::Roll()
{
	if (mFlags & TF_NOT_ALLOWED_TO_MOVE) return ;
	SINT game_turn = GAME.GetGameTurn() ;
	if (mCurrentMapNormalCalcTime != game_turn )
	{
		mCurrentMapNormalCalcTime = game_turn ;
		mCurrentMapNormal = MAP.GetMapNormal(mPos) ;
	}

	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER && GAME.IsGameStarting() && IsCharacter() )
	{
		CCharBall* cb = (CCharBall*) this ;
		GVector current_racer_spin_vector = cb->GetRacerBoostVector() ;
		GVector ax;
		GVector::Cross3(mCurrentMapNormal,current_racer_spin_vector, ax);
		SetSpin(ax);
		cb->SetRacerBoostVector(current_racer_spin_vector * G(0,60000) ) ;

		CCharacter *c=IsCharacter();
		if(c)
		{
			GVector	spos = mPos;
			spos.Z += Radius();

			GINT	spd=current_racer_spin_vector.MagnitudeXY();

			GINT	baserad = GATAN2(current_racer_spin_vector.Y, current_racer_spin_vector.X) + G_PI;  // guaranteeed not 0/0...

			CParticle *ptcl;
			if(spd.mVal>0x1000)
			{
				if((spd.mVal<0x4000) || ((GAME.GetGameTurn() & 3) == 0)) // otherwise there's no particles left for smoke!!
				{
					for(int pc=0;pc<2;pc++)
					{
						SINT	speed = spd.mVal;
						if (speed > 0x4000)
							speed = 0x4000;
						ptcl = GetRenderParticleSystem()->CreateParticle(PART_SPINSPARK);
						if(ptcl)
						{
							ptcl->mPos.X=spos.X;
							ptcl->mPos.Y=spos.Y;
							ptcl->mPos.Z.mVal=spos.Z.mVal;  //-(0x00010000);

							int 	r = (((G(rand() % 4095) * G(0,16384))) + (baserad / (G_PI * 2) * 4096) - (G(0,16384) * G(4096) / 2)).Whole();
							SINT	rad = (rand() % 7) + 8;
							ptcl->uPSpark.mVel.vx=(rcos(r) * rad * speed) >> (6 + 4 + 14);
							ptcl->uPSpark.mVel.vy=(rsin(r) * rad * speed) >> (6 + 4 + 14);
							ptcl->uPSpark.mVel.vz=-0x30;

							//ptcl->uPSpark.mVel.vz=-0x080;
							SINT	col = rand() & 0xff;
//							ptcl->mColour=0x00ffffff;
							ptcl->mColour=(col << 16) + (col << 8) + col;
						}
					}
				}
				if (spd.mVal > 0x4000)
				{
					ptcl=GetRenderParticleSystem()->CreateParticle(PART_SMOKE);
					if(ptcl)
					{
						ptcl->mPos=spos;
						ptcl->mRadius=128;

						int 	r = (((G(rand() % 4095) * G(0,16384))) + (baserad / (G_PI * 2) * 4096) - (G(0,16384) * G(4096) / 2)).Whole();
						SINT	rad = (rand() % 7) + 8;
						ptcl->mVelocity.X.mVal=((rcos(r) * rad) >> (6 + 4))<<8;
						ptcl->mVelocity.Y.mVal=((rsin(r) * rad) >> (6 + 4))<<8;
						ptcl->mVelocity.Z.mVal=-0x20<<8;


					}
				}
			}

			//GetRenderParticleSystem()->MakeSplash2(&spos, 5, baserad, G(0, 16384));
		}




	//	char temp[50] ;
	//	sprintf(temp,"Speed = %2.2f", GToF(current_racer_spin_vector.Magnitude()) ) ;
	//	CONSOLE.AddString(temp) ;
	}

	// are we on the ground?
	else if (mFlags & TF_ON_GROUND)
	{
		// yes - get angular velocity...

		// find rotation axis


		GVector ax;
		CMapCell *cell = MAP.GetCell(CCellRef(mPos));

		// are we on ice
		if ((cell->GetFriction() == FT_ICE) && IsCharacter())
		{

			CCharBall* cb = (CCharBall*) this ;
			GVector current_ice_spin_vector = cb->GetIceBallSpinVector() ;

			// if we have just started touching the ice the set the spin vector to the current
			// velocity vector
			if (current_ice_spin_vector == GVector(G0,G0,G0) )
			{
				current_ice_spin_vector = mVelocity ;
				cb->SetIceBallSpinVector(current_ice_spin_vector = mVelocity) ;
			}

			GVector::Cross3(mCurrentMapNormal, current_ice_spin_vector, ax);
			// $$$ SPIN ON ICE EFFECT HERE
			if (current_ice_spin_vector.Magnitude() > G(0,200) )
			{
				cb->SetIceBallSpinVector(current_ice_spin_vector * G(0,63000) ) ;

				/////////////////////
				GINT	spd=current_ice_spin_vector.MagnitudeXY();

				GINT	baserad = GATAN2(current_ice_spin_vector.Y, current_ice_spin_vector.X) + G_PI;  // guaranteeed not 0/0...

				CParticle *ptcl;
				if(spd.mVal>0x1000)
				{
					if((spd.mVal<0x4000) || ((GAME.GetGameTurn() & 3) == 0))
					{
						//for(int pc=0;pc<2;pc++)
						//{
							SINT	speed = spd.mVal;
							if (speed > 0x4000)
								speed = 0x4000;
							ptcl = GetRenderParticleSystem()->CreateParticle(PART_SPINSPARK);
							if(ptcl)
							{
								ptcl->mPos.X=mPos.X;
								ptcl->mPos.Y=mPos.Y;
								ptcl->mPos.Z.mVal=(mPos.Z+Radius()).mVal;  //-(0x00010000);

								int 	r = (((G(rand() % 4095) * G(0,16384))) + (baserad / (G_PI * 2) * 4096) - (G(0,16384) * G(4096) / 2)).Whole();
								SINT	rad = (rand() % 7) + 8;
								ptcl->uPSpark.mVel.vx=(rcos(r) * rad * speed) >> (6 + 4 + 14);
								ptcl->uPSpark.mVel.vy=(rsin(r) * rad * speed) >> (6 + 4 + 14);
								ptcl->uPSpark.mVel.vz=-0x30;
								ptcl->mColour=0xff8000;
							}
						//}
					}
				}
			}
		}
		else
		{
			// we are not on ice so if we are a char ball reset ice spin factor
			if (IsCharacter())
			{
				((CCharBall*)this)->SetIceBallSpinVector(GVector(G0,G0,G0)) ;
			}
			GVector::Cross3(mCurrentMapNormal, mVelocity, ax);
		}
		if ((cell->GetFriction() == FT_SLOW) && IsCharacter())
		{
/////////////////////
			GINT	spd=mVelocity.MagnitudeXY();

			GINT	baserad = GATAN2(mVelocity.Y, mVelocity.X) + G_PI;  // guaranteeed not 0/0...

			CParticle *ptcl;
			if(spd.mVal>0x1000)
			{
				if((spd.mVal<0x4000) || ((GAME.GetGameTurn() & 3) == 0))
				{
					//for(int pc=0;pc<2;pc++)
					//{
						SINT	speed = spd.mVal;
						if (speed > 0x4000)
							speed = 0x4000;
						ptcl = GetRenderParticleSystem()->CreateParticle(PART_SPINSPARK);
						if(ptcl)
						{
							ptcl->mPos.X=mPos.X;
							ptcl->mPos.Y=mPos.Y;
							ptcl->mPos.Z.mVal=(mPos.Z+Radius()).mVal;  //-(0x00010000);

							int 	r = (((G(rand() % 4095) * G(0,16384))) + (baserad / (G_PI * 2) * 4096) - (G(0,16384) * G(4096) / 2)).Whole();
							SINT	rad = (rand() % 7) + 8;
							ptcl->uPSpark.mVel.vx=(rcos(r) * rad * speed) >> (6 + 4 + 14);
							ptcl->uPSpark.mVel.vy=(rsin(r) * rad * speed) >> (6 + 4 + 14);
							ptcl->uPSpark.mVel.vz=-0x30;
							ptcl->mColour=0x00ff20;
						}
					//}
				}
			}
		}
///////////////////////////




















		SetSpin(ax);
	}

	// spin
	mOrientation = mAngularVelocity * mOrientation;

	// normalise every now and then
//	if (((GAME.GetGameTurn() + mNumber) & 31) == 0)  // shit - doesn't work....
		mOrientation.Normalise();
}

//******************************************************************************************
GINT	CActor::Gravity()
{
	return WORLD.Gravity();
};

//******************************************************************************************
GVector		CActor::GetRenderPos(SINT no)
{
	if (no == 0)
	{
		// JCL - fix for internal compiler error...
		GVector	delta = mPos - mOldPos;
		return mOldPos + (delta * GAME.GetFrameRenderFraction());
	}
	else
		return ZERO_GVECTOR;
}
