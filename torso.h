#ifndef	Torso_H
#define	Torso_H

#include	"Actor.h"
#include	"Effect.h"

#define TORSO_OFFSET	(G(2))

DECLARE_THING_CLASS(CTorso, CActor)
public:
	virtual	void		Init(CInitThing *i);

	virtual	BOOL				DoISeekCollisions() {return FALSE;};
	virtual ECollideShape		GetCollideShape()		{return CSH_NONE;};

	virtual	void		Process();
	void		SetSpin(BOOL to) {mSpin = to ;}
	virtual	GINT		Radius() { return G1;};
	virtual	GINT		GetRenderRadius()	{return GINT_HALF;};
			void		SetBodyFlamer(CFXBodyFlamer* flamer) { mBodyFlamer = flamer ; } 
	virtual void		ValidateData() { VALIDATE(mBodyFlamer); SUPERTYPE::ValidateData();};

	virtual BOOL		RenderRenderPart(SINT pn)	{return (pn == 1);};

	virtual	GVector		GetRenderPos(SINT no = 0)
	{
		if (no != 1)
			 return SUPERTYPE::GetRenderPos(no);
		else return mOrientation * GVector(G0, G0, TORSO_OFFSET);
	};

	virtual	GINT		GroundFriction(EFrictionType friction_type) {return G(0, 55000);};

protected:
	GINT		mYaw, mDestYaw;
	GINT		mPitch, mDestPitch;
	GINT		mRoll, mDestRoll;
	CFXBodyFlamer* mBodyFlamer ;
	BOOL		mSpin;
	GINT		mSpinamount ;
};


DECLARE_THING_CLASS(CPrimaryAngelTorso, CTorso)  };
DECLARE_THING_CLASS(CPrimaryApostleTorso, CTorso)  };
DECLARE_THING_CLASS(CPrimaryLockdownTorso, CTorso)  };
DECLARE_THING_CLASS(CPrimaryBennyTorso, CTorso)  };
DECLARE_THING_CLASS(CPrimarySophieTorso, CTorso)  };
DECLARE_THING_CLASS(CPrimaryDoctorTorso, CTorso)  };

DECLARE_THING_CLASS(CGuardATorso, CTorso)  };
DECLARE_THING_CLASS(CGuardBTorso, CTorso)  };
DECLARE_THING_CLASS(CGuardCTorso, CTorso)  };
DECLARE_THING_CLASS(CSGuardATorso, CTorso)  };
DECLARE_THING_CLASS(CSGuardBTorso, CTorso)  };
DECLARE_THING_CLASS(CSGuardCTorso, CTorso)  };

DECLARE_THING_CLASS(CClownTorso, CTorso)  };
DECLARE_THING_CLASS(CMarshallTorso, CTorso)  };
DECLARE_THING_CLASS(CDeputyTorso, CTorso)  };
DECLARE_THING_CLASS(CPrisoner1ATorso, CTorso)  };
DECLARE_THING_CLASS(CPrisoner1BTorso, CTorso)  };
DECLARE_THING_CLASS(CPrisoner1CTorso, CTorso)  };
DECLARE_THING_CLASS(CPrisoner2ATorso, CTorso)  };
DECLARE_THING_CLASS(CPrisoner2BTorso, CTorso)  };
DECLARE_THING_CLASS(CPrisoner2CTorso, CTorso)  };
DECLARE_THING_CLASS(CPrisoner3ATorso, CTorso)  };
DECLARE_THING_CLASS(CPrisoner3BTorso, CTorso)  };
DECLARE_THING_CLASS(CPrisoner3CTorso, CTorso)  };
DECLARE_THING_CLASS(CGillamTorso, CTorso)  };


#endif