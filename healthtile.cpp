// HealthTile.cpp: implementation of the CHealthTile class.
//
//////////////////////////////////////////////////////////////////////

#include "common.h"
#include "globals.h"
#include "HealthTile.h"
#include "Character.h"

//******************************************************************************************
void	CHealthTile::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	// snap myself to grid.
	GVector p = GVector(G(((mPos.X / 4).Whole() * 4) + 2),
						G(((mPos.Y / 4).Whole() * 4) + 2),
						G0);

	Move(p);
	StickToGround();
	WORLD.GetHealthTileNB().Add(this) ;

	mOnTimer = 0;

	SINT	c0, c1;
	for (c0 = mPos.Y.Whole() - 2; c0 < mPos.Y.Whole() + 2; c0++)
		for (c1 = mPos.X.Whole() - 2; c1 < mPos.X.Whole() + 2; c1++)
		{
			MAP.GetCell(CCellRef(c1, c0))->SetType(CT_HEALTH);
		}

	mFlags |= TF_RENDER_IN_EDITOR_ONLY;
}


//******************************************************************************************
void	CHealthTile::Shutdown()
{
	SUPERTYPE::Shutdown() ;
	WORLD.GetHealthTileNB().Remove(this) ;

	SINT	c0, c1;
	for (c0 = mPos.Y.Whole() - 2; c0 < mPos.Y.Whole() + 2; c0++)
		for (c1 = mPos.X.Whole() - 2; c1 < mPos.X.Whole() + 2; c1++)
		{
			MAP.GetCell(CCellRef(c1, c0))->SetType(CT_NORMAL);
		}
}

//******************************************************************************************
void	CHealthTile::Process()
{

#define LIGHT_SPACING 0x7200
	CParticleSystem*ps=MAP.GetEffectParticleSystem();
  	SINT turn=GAME.GetGameTurn();
	SINT x,y;
	SINT col;

	if(GAME.CanIBeSeen(mPos))
	{
		if(mOnTimer!=0)col=0x008000;
		else
		{
			col=0x000080;
			turn>>=1;
		}


		for(int i=0;i<4;i++)
		{
			CParticle *p=ps->CreateParticle(PART_LIGHT);
			if(p)
			{
				switch ( ((turn>>3)&3))
				{
					case 0:
						x=(mPos.X.mVal-0x20000)+((turn&7)*LIGHT_SPACING);
						y=mPos.Y.mVal+0x20000;
						break;
					case 1:
						x=mPos.X.mVal+0x20000;
						y=(mPos.Y.mVal+0x20000)-((turn&7)*LIGHT_SPACING);
						break;
					case 2:
						x=(mPos.X.mVal+0x20000)-((turn&7)*LIGHT_SPACING);
						y=mPos.Y.mVal-0x20000;
						break;
					case 3:
						x=mPos.X.mVal-0x20000;
						y=(mPos.Y.mVal-0x20000)+((turn&7)*LIGHT_SPACING);
						break;
					default:
						x = 0;
						y = 0;
						break;
				}

				p->mPos.X.mVal=x;
				p->mPos.Y.mVal=y;
				p->mPos.Z.mVal=mPos.Z.mVal;
				p->mColour=col;
			}
	 	turn+=8;
		}
	}




	if (mOnTimer != 0)
	{
		mOnTimer --;

		if (mOnTimer == 0)
		{
			SINT	c0, c1;
			for (c0 = mPos.Y.Whole() - 2; c0 < mPos.Y.Whole() + 2; c0++)
				for (c1 = mPos.X.Whole() - 2; c1 < mPos.X.Whole() + 2; c1++)
				{
					MAP.GetCell(CCellRef(c1, c0))->SetType(CT_HEALTH);
				}
		}
	}

	SUPERTYPE::Process();
}

//******************************************************************************************
void	CHealthTile::Hit(CThing *that)
{
	CCharacter* c = that->IsCharacter() ;
	if (c)
	{
		c->Heal(1) ;

		if (mOnTimer == 0)
		{
			SINT	c0, c1;
			for (c0 = mPos.Y.Whole() - 2; c0 < mPos.Y.Whole() + 2; c0++)
				for (c1 = mPos.X.Whole() - 2; c1 < mPos.X.Whole() + 2; c1++)
				{
					MAP.GetCell(CCellRef(c1, c0))->SetType(CT_HEALTH_ON);
				}
		}

		mOnTimer = 5;
	}
}

