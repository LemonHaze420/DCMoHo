#define __ACTORCOLLISION__
#include	"Common.h"

#include	"Actor.h"
#include	"Profile.h"
#include	"Globals.h"
#include	"Character.h"

//#if TARGET == PSX
//#pragma optimization_level 4
//#endif

// this file is essentially a continuation of Actor.cpp; function headers are in Actor.h

//******************************************************************************************
#define ACTOR_DAMPING GINT_HALF

//******************************************************************************************
// static stuff

CThing	*CActor::mCachedCollisionThing;
GVector	CActor::mCachedCollisionPos;
GINT	CActor::mCachedCollisionRadiusSq;


CThing *CActor::mLastAThing = NULL ;
SINT	CActor::mLastAThingTime = -50 ;
ECollideShape CActor::mCurrentACollideShape =CSH_NONE;
ECollideShape CActor::mCurrentBCollideShape =CSH_NONE;

//******************************************************************************************
void	CActor::ProcessCollision()
{
	PROFILE_FN(GameCollision);

	mFlags &= ~(TF_ON_GROUND | TF_ON_OBJECT);

	//! Think about which should be first....
	if (DoICollideWithMap())
		ProcessMapCollision();

	ProcessObjectCollision();
}

//******************************************************************************************
BOOL	CActor::DoSpheresCollide(CThing *a, CThing *b, GVector &dir)
{
	// SRG speeded up a bit

	// cache away thing 'a' details

	static SINT	d1 ;
	static GVector a1_pos, a2_pos ;
	static GINT arad, arad2 ;
	SINT game_turn = GAME.GetGameTurn() ;

	if (mLastAThing != a || mLastAThingTime != game_turn)
	{
		d1 = (a->GetCollideShape() == CSH_DOUBLE_SPHERE);
		a1_pos = a->GetPos() ;
		arad = a->Radius() ;
		if (d1)
		{
			a2_pos = a->GetPos(1) ;
			arad2 = a->Radius2() ;
		}
		mLastAThing = a ;
		mLastAThingTime = game_turn ;
	}


	SINT	hit = FALSE;

	SINT	d2 = (b->GetCollideShape() == CSH_DOUBLE_SPHERE);
	GVector b1_pos = b->GetPos() ;
	GINT brad = b->Radius() ;
	GVector b2_pos ;
	GINT brad2 ;

	if (d2)
	{
		b2_pos = b->GetPos(1) ;
		brad2 = b->Radius2() ;
	}


	// do we collide?
	GVector d;

	dir = a1_pos - b1_pos;
	GINT rsq= arad + brad ;
	rsq*=rsq ;

	if (dir.MagnitudeSq() <= rsq)
		hit = TRUE;

	if (d1)
	{
		d = a2_pos - b1_pos;
		rsq= arad2 + brad ;
		rsq*=rsq ;
		if (d.MagnitudeSq() <= rsq)
		{
			hit = TRUE;
		}
	}

	if (d2)
	{
		d = a1_pos - b2_pos;
		rsq= arad + brad2 ;
		rsq*=rsq ;
		if (d.MagnitudeSq() <= rsq)
		{
			hit = TRUE;
		}
	}

	if (d1 && d2)
	{
		d = a2_pos - b2_pos;
		rsq= arad2 + brad2 ;
		rsq*=rsq ;
		if (d.MagnitudeSq() <= rsq)
		{
			hit = TRUE;
		}
	}

	return hit;


}

//******************************************************************************************
void	CActor::CollideSphereSphereBounce(CThing *a, CThing *b)
{
	GVector	d;
	if (!(DoSpheresCollide(a, b, d)))
		return;

	// Get approach speed.
	GVector v1 = b->GetVelocity();
	GVector av = a->GetVelocity() - v1;

	// is a in the right direction?
	if (d * av >= G0)
		return; // nope

	//!!!!  this should be the approach speed projected on to the line between the centres...

	// and add half to each thing

	GINT s = (av.Magnitude() / 2); // * a->CoE() * b->CoE();

	d.Normalise();
	GVector ov = d * s;

	a->SetVelocity(a->GetVelocity() + ov);
	b->SetVelocity(v1 - ov);

	//! Hmmm.
	a->Move(a->GetOldPos() + a->GetVelocity());
	b->Move(b->GetOldPos() + v1 - ov);

	a->Hit(b);
	b->Hit(a);

	a->SetHitFlag();
	b->SetHitFlag();
}

//******************************************************************************************
void	CActor::CollideSphereSphereStatic(CActor *a, CThing *b)
{
	// do we collide?
	GVector	d;
	if (!(DoSpheresCollide(a, b, d)))
		return;

	// Get approach speed.
	GVector v1 = a->GetVelocity();

	//! OK - serious fucking hack here.
	// just using the line of centres is silly, as the spheres could have passed nearly right through each other.
	// use the midpoint of the moving spheres position in this frame and the last instead!!!

	GVector movement = a->GetOldPos() - a->GetPos() ;
	d = d + (movement / G(2));

	// is a in the right direction?
	if ( d * v1 >= G0 && movement.MagnitudeSq() > G(0,100) )
	{
		return; // nope
	}

	// add back to thing in direction
	GINT	s = v1.Magnitude();

	GINT	dm = d.Magnitude();

	if (dm > G(0, 10))
	{
		d /= dm;
		GVector ov = d * s;

		a->mVelocity += ov;

		// Hmm....
		a->mVelocity.Normalise();
		a->mVelocity *= s / G(2);

		a->Move(mOldPos + mVelocity);

		a->Hit(b);
		b->Hit(a);

		a->SetHitFlag();
		b->SetHitFlag();
	}
}

//******************************************************************************************
void	CActor::CollideSphereSpherePassive(CThing *a, CThing *b)
{
	GVector	d;
	if (!(DoSpheresCollide(a, b, d)))
		return;

	a->Hit(b);
	b->Hit(a);

	a->SetHitFlag();
	b->SetHitFlag();
}


//#if TARGET == PSX
//#pragma optimization_level  0
//#endif

/*
//******************************************************************************************
void	CActor::CollideSphereComplexStatic(CActor *a, CThing *b)
{
#if 1

	CCollisionVolume	*cv;

	BOOL	any_hits = FALSE;

	GVector	posa, oldposa;
	GINT	radiusa;
	SINT	numspheres = (a->GetCollideShape() == CSH_DOUBLE_SPHERE) ? 2 : 1;
	GVector	posoff;

	SINT	c99;

	for (c99 = 0; c99 < numspheres; c99 ++)
	{
		cv = b->GetComplexCollisionVolume();
		if (cv == NULL)
			return;

		if (c99 == 0)
		{
			posa = a->mPos;
			oldposa = a->mOldPos;
			radiusa = a->Radius();
			posoff = ZERO_GVECTOR;
		}
		else
		{
			// 1
			posa = a->GetPos(1);
			oldposa = posa + (a->mOldPos - a->mPos);
			radiusa = a->Radius2();
			posoff = a->mPos - posa;
		}

		while (cv)
		{
			switch (cv->mType)
			{
			case VT_BOUNDING_BOX:
				{
					// rotate a into frame of reference of b.
					//GVector	 pa = posa;
					//GVector opa = oldposa;
					GVector	 pb = cv->mTransformedOrigin + b->GetPos();

					GVector  d =  posa - pb;
					GVector od = oldposa - pb;
					GVector  v;// = a->GetVelocity();
					GMatrix mb = cv->mTransformedOrientation;
					GMatrix i = mb.Transpose();

					 d = i *  d;
					od = i * od;
					 v = i *  a->GetVelocity();
					 //v = i *  v;

					GINT	r = radiusa;
					GVector delta = d - od;

					// ok, quick reject -
					// check with outer cuboid.

					if ((GABS(d.X) > cv->mAxes.X + r) ||
						(GABS(d.Y) > cv->mAxes.Y + r) ||
						(GABS(d.Z) > cv->mAxes.Z + r))
					{
						// not hit! good!
						break;
					}

					// and inner
		///			if ((GABS(d.X) > cv->mAxes.X ) ||
		//				(GABS(d.Y) > cv->mAxes.Y ) ||
		//				(GABS(d.Z) > cv->mAxes.Z ))
		//			{
		//				// ok - we've not hit the inner cuboid.  check if we're moving away, to prevent odd forces
		//				if (delta * d > G0)
		//					break;
		//			}

					// OK - now check for disaster case where start point is also inside the cuboid..
					if (((GABS(od.X) < cv->mAxes.X + r) &&
						 (GABS(od.Y) < cv->mAxes.Y + r) &&
						 (GABS(od.Z) < cv->mAxes.Z + r)) ||
						(delta.MagnitudeSq() < G(0, 100)))  // or when delta is very small (fucks up edge calculations...)
					{
						// shite.  panic!

						// er... erm....  ok.  try to extend the old position back lots.
		//				GVector dir = -delta;
						GVector dir = od;
						GINT	mag = dir.Magnitude();

						if (mag < G(0, 100))
						{
							// Fuck.  just use gravity instead...
							dir = i * GVector(G0, G0, -G1);
							mag = G1;
						}
						dir *= (cv->mAxes.X + cv->mAxes.Y + cv->mAxes.Z) / mag;
						od = d + dir;
						delta = d - od;

						if (delta.MagnitudeSq() < G(0, 100))
							break; // oh dear....

						// are we out now??
						ASSERT ((GABS(od.X) > cv->mAxes.X + r) ||
								(GABS(od.Y) > cv->mAxes.Y + r) ||
				     			(GABS(od.Z) > cv->mAxes.Z + r));
					}

					// right.  now either we hit a face straight on; or an edge; or a corner.
					enum
					{
						HIT_NONE,
						HIT_SIMPLE,
						HIT_EDGE,
						HIT_CORNER
					} hit_type = HIT_NONE;

					GVector hp, normal, line, point;
					GINT	f;

					// check against 6 faces
					while (1) // silly..
					{
#if 1
						if (od.X > cv->mAxes.X + r)
						{
							f = (od.X - (cv->mAxes.X + r)) / (od.X - d.X);
							hp = od + delta * f;
							if ((GABS(hp.Y) < cv->mAxes.Y + r) &&
								(GABS(hp.Z) < cv->mAxes.Z + r))
							{
								if ((GABS(hp.Y) < cv->mAxes.Y) &&
									(GABS(hp.Z) < cv->mAxes.Z))
								{
									normal = GVector(G1, G0, G0);
									hit_type = HIT_SIMPLE;
									break;
								}
//								point = GVector(cv->mAxes.X * GSIGN(hp.X), cv->mAxes.Y * GSIGN(hp.Y), cv->mAxes.Z * GSIGN(hp.Z));
								point.X = GABS2(cv->mAxes.X , hp.X);
								point.Y = GABS2(cv->mAxes.Y , hp.Y);
								point.Z = GABS2(cv->mAxes.Z , hp.Z);

								if ((GABS(hp.Y) >= cv->mAxes.Y) &&
									(GABS(hp.Z) >= cv->mAxes.Z))
								{ // corner
									hit_type = HIT_CORNER;
									break;
								}
								hit_type = HIT_EDGE;
								if (GABS(hp.Y) >= cv->mAxes.Y)
									 line = GVector(G1, G1, G0);
								else line = GVector(G1, G0, G1);
								break;
							}
						}

						if (od.X < -cv->mAxes.X - r)
						{
							f = (od.X - (-cv->mAxes.X - r)) / (od.X - d.X);
							hp = od + delta * f;
							if ((GABS(hp.Y) < cv->mAxes.Y + r) &&
								(GABS(hp.Z) < cv->mAxes.Z + r))
							{
								if ((GABS(hp.Y) < cv->mAxes.Y) &&
									(GABS(hp.Z) < cv->mAxes.Z))
								{
									normal = GVector(-G1, G0, G0);
									hit_type = HIT_SIMPLE;
									break;
								}
//								point = GVector(cv->mAxes.X * GSIGN(hp.X), cv->mAxes.Y * GSIGN(hp.Y), cv->mAxes.Z * GSIGN(hp.Z));
								point.X = GABS2(cv->mAxes.X , hp.X);
								point.Y = GABS2(cv->mAxes.Y , hp.Y);
								point.Z = GABS2(cv->mAxes.Z , hp.Z);

								if ((GABS(hp.Y) >= cv->mAxes.Y) &&
									(GABS(hp.Z) >= cv->mAxes.Z))
								{ // corner
									hit_type = HIT_CORNER;
									break;
								}
								hit_type = HIT_EDGE;
								if (GABS(hp.Y) >= cv->mAxes.Y)
									 line = GVector(G1, G1, G0);
								else line = GVector(G1, G0, G1);
								break;
							}
						}

						if (od.Y > cv->mAxes.Y + r)
						{
							f = (od.Y - (cv->mAxes.Y + r)) / (od.Y - d.Y);
							hp = od + delta * f;
							if ((GABS(hp.X) < cv->mAxes.X + r) &&
								(GABS(hp.Z) < cv->mAxes.Z + r))
							{
								if ((GABS(hp.X) < cv->mAxes.X) &&
									(GABS(hp.Z) < cv->mAxes.Z))
								{
									normal = GVector(G0, G1, G0);
									hit_type = HIT_SIMPLE;
									break;
								}
//								point = GVector(cv->mAxes.X * GSIGN(hp.X), cv->mAxes.Y * GSIGN(hp.Y), cv->mAxes.Z * GSIGN(hp.Z));
								point.X = GABS2(cv->mAxes.X , hp.X);
								point.Y = GABS2(cv->mAxes.Y , hp.Y);
								point.Z = GABS2(cv->mAxes.Z , hp.Z);

								if ((GABS(hp.X) >= cv->mAxes.X) &&
									(GABS(hp.Z) >= cv->mAxes.Z))
								{ // corner
									hit_type = HIT_CORNER;
									break;
								}
								hit_type = HIT_EDGE;
								if (GABS(hp.X) >= cv->mAxes.X)
									 line = GVector(G1, G1, G0);
								else line = GVector(G0, G1, G1);
								break;
							}
						}

						if (od.Y < -cv->mAxes.Y - r)
						{
							f = (od.Y - (-cv->mAxes.Y - r)) / (od.Y - d.Y);
							hp = od + delta * f;
							if ((GABS(hp.X) < cv->mAxes.X + r) &&
								(GABS(hp.Z) < cv->mAxes.Z + r))
							{
								if ((GABS(hp.X) < cv->mAxes.X) &&
									(GABS(hp.Z) < cv->mAxes.Z))
								{
									normal = GVector(G0, -G1, G0);
									hit_type = HIT_SIMPLE;
									break;
								}
//								point = GVector(cv->mAxes.X * GSIGN(hp.X), cv->mAxes.Y * GSIGN(hp.Y), cv->mAxes.Z * GSIGN(hp.Z));
								point.X = GABS2(cv->mAxes.X , hp.X);
								point.Y = GABS2(cv->mAxes.Y , hp.Y);
								point.Z = GABS2(cv->mAxes.Z , hp.Z);

								if ((GABS(hp.X) >= cv->mAxes.X) &&
									(GABS(hp.Z) >= cv->mAxes.Z))
								{ // corner
									hit_type = HIT_CORNER;
									break;
								}
								hit_type = HIT_EDGE;
								if (GABS(hp.X) >= cv->mAxes.X)
									 line = GVector(G1, G1, G0);
								else line = GVector(G0, G1, G1);
								break;
							}
						}

						if (od.Z > cv->mAxes.Z + r)
						{
							f = (od.Z - (cv->mAxes.Z + r)) / (od.Z - d.Z);
							hp = od + delta * f;
							if ((GABS(hp.X) < cv->mAxes.X + r) &&
								(GABS(hp.Y) < cv->mAxes.Y + r))
							{
								if ((GABS(hp.X) < cv->mAxes.X) &&
									(GABS(hp.Y) < cv->mAxes.Y))
								{
									normal = GVector(G0, G0, G1);
									hit_type = HIT_SIMPLE;
									break;
								}
//								point = GVector(cv->mAxes.X * GSIGN(hp.X), cv->mAxes.Y * GSIGN(hp.Y), cv->mAxes.Z * GSIGN(hp.Z));
								point.X = GABS2(cv->mAxes.X , hp.X);
								point.Y = GABS2(cv->mAxes.Y , hp.Y);
								point.Z = GABS2(cv->mAxes.Z , hp.Z);

								if ((GABS(hp.X) >= cv->mAxes.X) &&
									(GABS(hp.Y) >= cv->mAxes.Y))
								{ // corner
									hit_type = HIT_CORNER;
									break;
								}
								hit_type = HIT_EDGE;
								if (GABS(hp.X) >= cv->mAxes.X)
									 line = GVector(G1, G0, G1);
								else line = GVector(G0, G1, G1);
								break;
							}
						}

						if (od.Z < -cv->mAxes.Z - r)
						{
							f = (od.Z - (-cv->mAxes.Z - r)) / (od.Z - d.Z);
							hp = od + delta * f;
							if ((GABS(hp.X) < cv->mAxes.X + r) &&
								(GABS(hp.Y) < cv->mAxes.Y + r))
							{
								if ((GABS(hp.X) < cv->mAxes.X) &&
									(GABS(hp.Y) < cv->mAxes.Y))
								{
									normal = GVector(G0, G0, -G1);
									hit_type = HIT_SIMPLE;
									break;
								}
//								point = GVector(cv->mAxes.X * GSIGN(hp.X), cv->mAxes.Y * GSIGN(hp.Y), cv->mAxes.Z * GSIGN(hp.Z));
								point.X = GABS2(cv->mAxes.X , hp.X);
								point.Y = GABS2(cv->mAxes.Y , hp.Y);
								point.Z = GABS2(cv->mAxes.Z , hp.Z);

								if ((GABS(hp.X) >= cv->mAxes.X) &&
									(GABS(hp.Y) >= cv->mAxes.Y))
								{ // corner
									hit_type = HIT_CORNER;
									break;
								}
								hit_type = HIT_EDGE;
								if (GABS(hp.X) >= cv->mAxes.X)
									 line = GVector(G1, G0, G1);
								else line = GVector(G0, G1, G1);
								break;
							}
						}

						// shite
						ASSERT(0);
#endif
						break;
					};
					if (hit_type == HIT_NONE)
						break; // well, safety first, or something....

					// special cases
					if (hit_type == HIT_CORNER)
					{
						// find point of closest approach
						GVector		w = point - od;
						GINT		wmag = w.Magnitude();
						GINT		dmag = delta.Magnitude();

						GINT		d1  = (w * delta) / dmag;
						GINT		asq = (wmag * wmag) - (d1 * d1);
						if (asq > (r * r))	break;   // didn't hit!
						GINT		d2  = GROOT((r * r) - asq);
						GINT		d3  = d1 - d2;
						f = d3 / dmag;

						if (f.mVal > 0x10000)
							break;		// well, it didn't hit after all!
						hp = od + (delta * f);
						normal = hp - point;
						normal.Normalise();
					}
					else if (hit_type == HIT_EDGE)
					{
						// find point of closest approach
						// slightly wierd 2.5D version of the above...  .Mult(line) converts into 2D
						GVector		w2d     = (point - od).Mult(line);
						GVector		delta2d = delta.Mult(line);
						GINT		wmag = w2d.Magnitude();
						GINT		dmag = delta2d.Magnitude();

						GINT		d1  = (w2d * delta2d) / dmag;
						GINT		asq = (wmag * wmag) - (d1 * d1);
						if (asq > (r * r))	break;   // didn't hit!
						GINT		d2  = GROOT((r * r) - asq);
						GINT		d3  = d1 - d2;
						f = d3 / dmag;

						if (f.mVal > 0x10000)
							break;		// well, it didn't hit after all!
						hp = od + (delta * f);
						normal = (hp - point).Mult(line);
						ASSERT(GABS(normal.Magnitude() - r) < G(0,100));
						normal.Normalise();
					}

					// clip to surface
					GVector mod = normal * ((G1 - f) * (od - d).Magnitude());
					d += mod;

					// and reflect the velocity.

					// we need to get the velocity of object b as well, here
					// which means we need the collision point in the frame of reference of the object,,,
		//			GVector hpb = b->GetOrientation() * ((cv->mTransformedOrientation * hp) + cv->mTransformedOrigin);

					// get point velocity
		//			GVector bvel = b->GetPointVelocity(hpb);
		//			bvel = i * bvel; // transform it into the frame of reference
		
					// take two.

					GVector	bvel;

					if (b->DoIRotate() && cv->mMeshPart)
					{
						// get velocity due to rotation
						bvel = ZERO_GVECTOR;

						// transform the hit point back into the frame of reference of the mesh part
						GVector hpt = (cv->mOrientation * hp) + cv->mOrigin;

						// and use the mesh part to get the velocity
						cv->mMeshPart->EvaluatePointVelocity(hpt, bvel, b);
					}
					else
						bvel = i * b->GetVelocity();  // basic speed in frame of reference

					v -= bvel; // transform

					if (v * normal < G0) // if we're actually going towards the object..
		//				v.ReflectInNormal(normal, ACTOR_DAMPING);
						v.ReflectInNormal(normal, b->CoE());  //! should this be a * b?

					// friction...
					// already in the frame of reference of the box, so it's quite easy

					// get component of velocity perpendicular to normal
					GVector pvel = (normal ^ v) ^ normal;  // conveniently this has correct magnitude!
					v -= pvel * b->CoF(); // friction component

					// set it spinning
					a->SetSpin(normal ^ v);

					v += bvel;  // and transform back

					// reality check

					ASSERT(GABS(v.X) < G(4));
					ASSERT(GABS(v.Y) < G(4));
					ASSERT(GABS(v.Z) < G(4));

					// rotate back into original frame of reference.
					d = mb * d;
					a->Move(d + pb + posoff);

					v = mb * v;
					a->SetVelocity(v);

					any_hits = TRUE;

					// does this count as a ground hit?
					GVector	real_normal = mb * normal;
					if (real_normal.Z < -GINT_HALF)
					{
						mFlags |= TF_ON_GROUND;
						mFlags |= TF_ON_OBJECT;
						a->HitGround(pvel.Magnitude()); // hmmmm - not sure this is the right value!
					}
				}
				break;

			case VT_SPHERE:
			default:
				ASSERT(0);
				break;
			};

			if (!cv->mNext)
				cv = cv->mNextPart;
			else
				cv = cv->mNext;
		}
	}

	if (any_hits)
	{
		a->Hit(b);
		b->Hit(a);

		a->SetHitFlag();
		b->SetHitFlag();
	}
#endif
}
*/
//#if TARGET == PSX
//#pragma optimization_level   4
//#endif

//******************************************************************************************
void	CActor::CollideSphereParticleSystem(CActor *a, CThing *b)
{
	CParticleSystem *ps = b->GetParticleSystem();

	if (ps)
	{
		CParticle	*part = ps->mParticles;

		SINT	c0;
		GVector a_pos = a->GetPos() ;
		GVector a_pos1 = a->GetPos(1) ;
		BOOL a_double_sphere(a->GetCollideShape() == CSH_DOUBLE_SPHERE) ;
		SINT	r = a->Radius().mVal;
		SINT	r2 = a->Radius2().mVal;

		SINT check = GAME.GetGameTurn() & 1 ;

		for (c0 = 0; c0 < MAX_PARTICLES; c0 ++)
		{
			// only check each partile every other frame
			if ((c0 & 1) == check)
			{
				// Only collide with a 'particle' if it has a radius... (radius is 1.7.8)
				if ((part->mType != PART_NONE)&&(part->mType<PART_RING)&&(part->mRadius))
				{
					BOOL	hit = FALSE;

					GVector d = a_pos - part->mPos;

					if (d.Magnitude().mVal < (r + (part->mRadius<<8) ))
						hit = TRUE;

					if (a_double_sphere)
					{
						d = a_pos1 - part->mPos;
						if (d.Magnitude().mVal < (r2 + (part->mRadius<<8)))
							hit = TRUE;
					}

					if (hit)
						a->HitParticle(part->mType);
				}
			}
			part ++;
		}
	}
}



//******************************************************************************************
void	CActor::CollideSphereFenceYAllign(CActor *a, CThing *b, BOOL passive)
{
	GINT fx=b->GetRenderStartPos().X;
	GINT ax=a->GetPos().X ;
	GINT r = a->Radius() ;
	if ( ax < (fx+r) && 
		 ax > (fx-r) ||
		 (a->IsCharacter() && ((CCharacter*)a)->GetState() == CS_KNOCKED_OUT) )
	{
		CollideSphereFence(a,b,passive) ;
	}
}


//******************************************************************************************
void	CActor::CollideSphereFenceXAllign(CActor *a, CThing *b, BOOL passive)
{
	GINT fy=b->GetRenderStartPos().Y;
	GINT ay=a->GetPos().Y ;
	GINT r = a->Radius() ;
	if ( ay < (fy+r) && 
		 ay > (fy-r) ||
		 (a->IsCharacter() && ((CCharacter*)a)->GetState() == CS_KNOCKED_OUT) )
	{
		CollideSphereFence(a,b,passive) ;
	}
}


//******************************************************************************************
void	CActor::CollideSphereFence(CActor *a, CThing *b, BOOL passive)
{
	PROFILE_FN(FenceCollision);

	if (mVelocity.MagnitudeSq() < G(0, 2))
	{
		return ;
	}
	GVector	s = b->GetRenderStartPos();
	GVector	e = b->GetRenderEndPos();

	s.Z = G0;
	e.Z = G0;

	GVector d = e - s;
	GVector n = b->GetCollisionNormal();

	GINT	r, d1, d2;
	GVector p1, p2;
	BOOL	hit;

	hit = FALSE;

	r  = a->Radius();
	p1 = a->GetPos();
	p2 = a->GetOldPos();

	if (((p1 - s) * d > -r) &&
		((e - p1) * d > -r))
	{
		d1 = (p1 - s) * n;
		d2 = (p2 - s) * n;

		if ((d2 < G0) && (d1 > -r))
		{
			hit = TRUE;
			if (!passive) a->Move(p1 - (n * (d1 + r)));
		}

		if ((d2 > G0) && (d1 <  r))
		{
			hit = TRUE;
			if (!passive) a->Move(p1 - (n * (d1 - r)));
		}

		//! do double sphere

		// truely a 5 star axe award here for this hack

		// do some kind of upperbody collision with fences
		CCharacter* c = IsCharacter() ;
		if (c && 
		    hit == FALSE &&
		    c->GetState() == CS_KNOCKED_OUT &&
		    (!passive))
		{
			d2 = (p1 - s) * n ;
			GVector head = mPos+ (GetRenderOrientation(1) * GVector(G0,G0,-G(3)) );
			d1 = (head - s) * n;
			if ((d2 < G0) && (d1 > -r))
			{
				hit = TRUE;
				a->Move(p1 - (n * (d1 + r)));
			}

			if ((d2 > G0) && (d1 <  r))
			{
				hit = TRUE;
				a->Move(p1 - (n * (d1 - r)));
			}

			if (hit == TRUE)
			{
				GVector v = a->GetVelocity();
				v.ReflectInNormal(n, GetMapDamping());
				a->SetVelocity(v);
			}
			return ;
		}

		if (hit)
		{
			if (!passive)
			{
				GVector v = a->GetVelocity();
				if (a->GetCollideResponse() == CR_BOUNCE)
				{
					GVector v2 = v;
					v.ReflectInNormal(n, G0); // totally damp
					v2 = v - v2;
					v2.Normalise();
					a->SetVelocity(v + (v2 * G(0, 16384)));
				}
				else
				{
					v.ReflectInNormal(n, GetMapDamping());
					a->SetVelocity(v);
				}
			}

			a->Hit(b);
			b->Hit(a);

			a->SetHitFlag();
			b->SetHitFlag();
		}
	}
}



//******************************************************************************************
void	CActor::CollideSphereOBBMovingStatic(CActor *a, CThing *b)
{
	
	CCollisionVolume* b_collision_volume = b->GetComplexCollisionVolume()->mNextPart ;
	GVector axis = b_collision_volume->mAxes;
	GVector b_pos = b->GetPos() ;
	GVector b_origin = b_collision_volume->mOrigin+ b->GetComplexCollisionVolume()->mOrigin ;
	GINT bpx = b_pos.X+axis.X +b_origin.X;
	GINT bpy = b_pos.Y+axis.Y +b_origin.Y;
	GINT bpz = b_pos.Z+axis.Z +b_origin.Z;
	GINT bmx = b_pos.X-axis.X +b_origin.X;
	GINT bmy = b_pos.Y-axis.Y +b_origin.Y;
	GINT bmz = b_pos.Z-axis.Z +b_origin.Z;

	SINT	numspheres = (a->GetCollideShape() == CSH_DOUBLE_SPHERE) ? 2 : 1;
	BOOL something_hit = FALSE ;

	// only do single sphere here at mo cos doen't quite work with 2
	for (SINT sphere = 0; sphere < numspheres ; sphere ++)
	{
		if (something_hit == FALSE) {
		GINT r ;
		GVector a_pos ;
		GVector posoff ;

		if (sphere == 0)
		{
			a_pos = a->mPos;
			r = a->Radius();
			posoff = ZERO_GVECTOR;
		}
		else
		{
			// 1
			a_pos = a->GetPos(1);
			r = a->Radius();
			posoff = a_pos - a->mPos;
		}

		// step 1 : rotate ball into b's axis system
		GMatrix ori = b->GetRenderOrientation(1) ;

		GMatrix inverse = ori;
		inverse.TransposeInPlace() ;
		GVector b_to_apos = a_pos - b_pos ;

		GVector trans_a = inverse * b_to_apos ;
		a_pos = b_pos+trans_a ;


		// check fast AABB collsion
		GINT apx = a_pos.X+r ;
		GINT apy = a_pos.Y+r ;
		GINT apz = a_pos.Z+r ;
		GINT amx = a_pos.X-r ;
		GINT amy = a_pos.Y-r ;
		GINT amz = a_pos.Z-r ;

	    BOOL sphere_hits_aabb = FALSE ;
		BOOL boundary_boxes_hit = TRUE ;

		// pass 1 does the sphere AABB bounding box intersect with AABB ( faster check !!)
		if (apx < bmx ||
			amx > bpx ||
			apy < bmy ||
			amy > bpy ||
			apz < bmz ||
			amz > bpz) boundary_boxes_hit = FALSE; 

		if (boundary_boxes_hit)
		{
			// ok more accurate checks

			GVector new_vel = inverse * mVelocity ;
			GVector st = new_vel ;
			GINT disx, disy, disz ;
			GINT velocity_mult= G(0,52123) ;

			// find closest point on an edge of the aabb to the centre of the sphere
			GVector edge_point ;
			if (a_pos.X < bmx)
			{
				edge_point.X=bmx ;
				disx = a_pos.X - bmx ;
			}
			else if (a_pos.X > bpx)
			{
				edge_point.X = bpx ; 
				disx = a_pos.X - bpx ;
			}
			else
			{
				edge_point.X = a_pos.X ;
				disx=G0 ;
			}

			if (a_pos.Y < bmy) 
			{
				edge_point.Y=bmy ;
				disy = a_pos.Y - bmy;
			}
			else if (a_pos.Y > bpy)
			{
				edge_point.Y = bpy ; 
				disy = a_pos.Y - bpy ;
			}
			else 
			{
				disy=G0 ;
				edge_point.Y = a_pos.Y ;
			}

			if (a_pos.Z < bmz) 
			{
				edge_point.Z=bmz ; 
				disz = a_pos.Z - bmz ;
				mFlags |= TF_ON_OBJECT ;
				mFlags |= TF_ON_GROUND ;
				velocity_mult=G1;
			}
			else if (a_pos.Z > bpz) 
			{
				disz = a_pos.Z - bpz ;
				edge_point.Z = bpz ;
			}
			else 
			{	
				disz = G0 ;
				edge_point.Z = a_pos.Z ;
			}

			if ((a_pos - edge_point).MagnitudeSq() < (r*r))
			{
				sphere_hits_aabb = TRUE ;
				something_hit = TRUE;
		
				GINT gdisx = GABS(disx) ;
				GINT gdisy = GABS(disy) ;
				GINT gdisz = GABS(disz) ;
				GINT possible_new_velX = disx<G0 ? -(r+disx) : r-disx;
				GINT possible_new_velY = disy<G0 ? -(r+disy) : r-disy;
				GINT possible_new_velZ = disz<G0 ? -(r+disz) : r-disz;
				GINT stx = possible_new_velX ;
				GINT sty = possible_new_velY ;
				GINT stz = G0 ;


				// are we on a corner
				if (disx != G0 && disz != G0 && disy!= G0)
				{
					if (gdisx > gdisy && gdisx > gdisz)
					{
						new_vel.X = possible_new_velX ;
						st.X = stx;
					}
					else if (gdisy > gdisx && gdisy > gdisz)
					{
						new_vel.Y = possible_new_velY ;
						st.Y = sty;
					}
					else if (gdisz > gdisx && gdisz > gdisy)
					{
					
						new_vel.Z = possible_new_velZ ;
						st.Z = stz;
					}
				}

				// or an edge ??
				else if (disx != G0 && disy != G0)
				{
					if (gdisx < gdisy)
					{
						new_vel.Y = possible_new_velY;
						st.Y = sty;
					}
					else
					{
						new_vel.X = possible_new_velX;
						st.X = stx;
					}
				}
				else if (disx != G0 && disz != G0)
				{
					if (gdisx < gdisz && possible_new_velZ < G0) 
					{
						new_vel.Z = possible_new_velZ;
				    	  st.Z = stz;
					}
					else 
					{
						new_vel.X = possible_new_velX;
						st.X = stx;
					}
				}
				else if (disy != G0 && disz != G0)
				{
					if (gdisy < gdisz && possible_new_velZ < G0) 
					{
						new_vel.Z = possible_new_velZ;
						st.Z = stz;;
					}
					else
					{
						new_vel.Y = possible_new_velY;
						st.Y = sty;
					}
				}

				// ok we are in the middle of one of the 6 planes
				else
				{
					if (disx!=G0) {new_vel.X = possible_new_velX ; st.X = possible_new_velX;}
					if (disy!=G0) {new_vel.Y = possible_new_velY ; st.Y = possible_new_velY;}
					if (disz!=G0) {new_vel.Z = possible_new_velZ ; st.Z = possible_new_velZ;}
				}

			
				// rotate back to original axis system
				GVector new_b_to_a =  (a_pos+new_vel) - b_pos ;

				Move(b_pos+(ori * new_b_to_a)-posoff) ; 
				mVelocity = ori * st ;
				mVelocity*=velocity_mult;
			}
			}
		}
	}

	if (something_hit)
	{
		a->Hit(b);
		b->Hit(a);

		a->SetHitFlag();
		b->SetHitFlag();
	}
}


//******************************************************************************************
void	CActor::CollideSphereOBBStatic(CActor *a, CThing *b, SINT volume_num)
{
	CCollisionVolume* b_collision_volume = b->GetComplexCollisionVolume();
	if (volume_num ==2)
	{
		b_collision_volume = b_collision_volume->mNextPart ;
	}


	GVector axis = b_collision_volume->mAxes;
	GVector b_pos = b->GetPos() ;
	GVector b_origin = b_collision_volume->mOrigin ;
	if (volume_num ==2)
	{
		b_origin+= b->GetComplexCollisionVolume()->mOrigin ;
	}

	// sorry this is bad
	if (b->GetCollideShape() == CSH_OBB3)
	{
		b_origin = b_collision_volume->mTransformedOrigin;
	}

	GINT bpx = b_pos.X+axis.X +b_origin.X;
	GINT bpy = b_pos.Y+axis.Y +b_origin.Y;
	GINT bpz = b_pos.Z+axis.Z +b_origin.Z;
	GINT bmx = b_pos.X-axis.X +b_origin.X;
	GINT bmy = b_pos.Y-axis.Y +b_origin.Y;
	GINT bmz = b_pos.Z-axis.Z +b_origin.Z;

	SINT	numspheres = (a->GetCollideShape() == CSH_DOUBLE_SPHERE) ? 2 : 1;
	BOOL something_hit = FALSE ;

	// only do single sphere here at mo cos doen't quite work with 2
	for (SINT sphere = 0; sphere < 1 ; sphere ++)
	{
		GINT r ;
		GVector a_pos ;
		GVector a_opos;
		GVector posoff ;

		if (sphere == 0)
		{
			a_pos = a->mPos;
			a_opos = a->mOldPos;
			r = a->Radius() ;
			posoff = ZERO_GVECTOR;
		}
		else
		{
			// 1
			a_pos = a->GetPos(1);
			a_opos = a_pos + (a->mOldPos - a->mPos);
			r = a->Radius();
			posoff = a_pos - a->mPos;
		}


		// step 1 : rotate ball into b's axis system
		GMatrix ori = b->GetRenderOrientation(volume_num-1) ;

		GMatrix inverse = ori ;
		inverse.TransposeInPlace();
		GVector b_to_apos = a_pos - b_pos ;

		GVector trans_a = inverse * b_to_apos ;
		a_pos = b_pos+trans_a ;


		GINT apx = a_pos.X+r ;
		GINT apy = a_pos.Y+r ;
		GINT apz = a_pos.Z+r ;
		GINT amx = a_pos.X-r ;
		GINT amy = a_pos.Y-r ;
		GINT amz = a_pos.Z-r ;
 
	    BOOL sphere_hits_aabb = FALSE ;
		BOOL boundary_boxes_hit = TRUE ;

		// pass 1 does the sphere AABB bounding box intersect with AABB ( faster check !!)
		if (apx < bmx ||
			amx > bpx ||
			apy < bmy ||
			amy > bpy ||
			apz < bmz ||
			amz > bpz) boundary_boxes_hit = FALSE; 

		if (boundary_boxes_hit)
		{
			// ok more accurate checks
			UINT b4flags = mFlags ;

			if (a_pos.X > bmx &&
				a_pos.X < bpx &&
				a_pos.Y > bmy &&
				a_pos.Y < bpy &&
				a_pos.Z > bmz &&
				a_pos.Z < bpz)
			{
				mVelocity/=2 ;
				Move(a_opos) ;
				a->Hit(b);
				b->Hit(a);
				a->SetHitFlag();
				b->SetHitFlag();
				return ;
			}
	
			GVector norm(G0,G0,G0) ;
			GVector new_vel = inverse * mVelocity ;
			GVector new_trans = new_vel ;
			GVector st = new_vel ;
			GINT disx, disy, disz ;

			// find closest point on an edge of the aabb to the centre of the sphere
			GVector edge_point ;
			if (a_pos.X < bmx)
			{
				edge_point.X=bmx ;
				disx = GABS(a_pos.X - bmx) ;
				norm.X = -G1 ;
			}
			else if (a_pos.X > bpx)
			{
				edge_point.X = bpx ; 
				disx = GABS(a_pos.X - bpx) ;
				norm.X = G1 ;
			}
			else
			{
				edge_point.X = a_pos.X ;
				disx=G0 ;
			}

			if (a_pos.Y < bmy) 
			{
				norm.Y = -G1;
				edge_point.Y=bmy ;
				disy = GABS(a_pos.Y - bmy) ;
			}
			else if (a_pos.Y > bpy)
			{
				norm.Y = G1;
				edge_point.Y = bpy ; 
				disy = GABS(a_pos.Y - bpy) ;
			}
			else 
			{
				disy=G0 ;
				edge_point.Y = a_pos.Y ;
			}

			if (a_pos.Z < bmz) 
			{
				norm.Z = -G1;
				edge_point.Z=bmz ; 
				disz = GABS(a_pos.Z - bmz) ;
				mFlags |= TF_ON_OBJECT ;
				mFlags |= TF_ON_GROUND ;
			}
			else if (a_pos.Z > bpz) 
			{
				norm.Z = G1;
				disz = GABS(a_pos.Z - bpz) ;
				edge_point.Z = bpz ;
			}
			else 
			{	
				disz = G0 ;
				edge_point.Z = a_pos.Z ;
			}

			if ((a_pos - edge_point).MagnitudeSq() < (r*r))
			{
				sphere_hits_aabb = TRUE ;
				something_hit = TRUE;
		
				// are we on a corner
				if (disx != G0 && disz != G0 && disy!= G0)
				{
					if (disx < disy)
					{
						if (disy < disz)
						{
							if (b4flags & TF_ON_GROUND)  st.Z = G0 ;
							new_vel.Z = G0 ;
						}
						else 
						{
							if (b4flags & TF_ON_GROUND) st.Y = G0 ;
							new_vel.Y = G0 ;
						}
					}
					else if ( disx < disz) 
					{
						if (b4flags & TF_ON_GROUND) st.Z = G0 ;
						new_vel.Z = G0 ;
					}
					else 
					{
						if (b4flags & TF_ON_GROUND) st.X = G0 ;
						new_vel.X = G0 ;
					}
				}

				// or an edge ??
				else if (disx != G0 && disy != G0)
				{
					if (disx < disy)
					{
						new_vel.Y = G0;
						st.Y = G0;
					}
					else
					{
						new_vel.X = G0;
						st.X = G0;
					}
				}
				else if (disx != G0 && disz != G0)
				{
					if (disx < disz) 
					{
						new_vel.Z = G0;
						st.Z = G0;
					}
					else 
					{
						if (b4flags & TF_ON_GROUND)
						{
							new_vel.X = G0;
						    st.X = G0;
						}
					}
				}
				else if (disy != G0 && disz != G0)
				{
					if (disy < disz) 
					{
						new_vel.Z = G0;
						st.Z = G0;
					}
					else
					{
						if (b4flags & TF_ON_GROUND)
						{
							new_vel.Y = G0;
							st.Y = G0;
						}
					}
				}

				// ok we are in the middle of one of the 6 planes
				else
				{
					if (disx!=G0) {new_vel.X = G0; st.X = G0;}
					if (disy!=G0) {new_vel.Y = G0; st.Y = G0;} 
					if (disz!=G0) {new_vel.Z = G0; st.Z = G0;}
				}

				// ok extra safe check to make sure before we move the sphere that we 
				// were trying to enter the box rather than got stuck and trying to escape
				if ((norm * new_trans) < G0 )
				{
					GVector b_to_aopos = a_opos - b_pos ;
					GVector trans_ao = inverse * b_to_aopos ;
					a_opos = b_pos+trans_ao ;
					GVector new_b_to_a =  (a_opos+new_vel) - b_pos ;
					Move(b_pos+(ori * new_b_to_a)-posoff) ; 
					mVelocity = ori * st ;
				}
			}
		}
	}

	if (something_hit)
	{
		a->Hit(b);
		b->Hit(a);

		a->SetHitFlag();
		b->SetHitFlag();
	}
}


//******************************************************************************************
void	CActor::CollideSphereAABB(CActor *a, CThing *b, BOOL passive)
{

	CCollisionVolume* b_collision_volume = b->GetComplexCollisionVolume() ;
	GVector axis = b_collision_volume->mAxes;
	GVector b_pos = b->GetPos() ;
	GVector b_origin = b_collision_volume->mOrigin ;
	GINT bpx = b_pos.X+axis.X +b_origin.X;
	GINT bpy = b_pos.Y+axis.Y +b_origin.Y;
	GINT bpz = b_pos.Z+axis.Z +b_origin.Z;
	GINT bmx = b_pos.X-axis.X +b_origin.X;
	GINT bmy = b_pos.Y-axis.Y +b_origin.Y;
	GINT bmz = b_pos.Z-axis.Z +b_origin.Z;

	SINT	numspheres = (a->GetCollideShape() == CSH_DOUBLE_SPHERE) ? 2 : 1;
	BOOL something_hit = FALSE ;

	// only do single sphere here at mo cos doen't quite work with 2
	for (SINT sphere = 0; sphere < 1; sphere ++)
	{
		GINT r ;
		GVector a_pos ;
		GVector a_opos;
		GVector posoff ;

		if (sphere == 0)
		{
			a_pos = a->mPos;
			a_opos = a->mOldPos;
			r = a->Radius();
			posoff = ZERO_GVECTOR;
		}
		else
		{
			// 1
			a_pos = a->GetPos(1);
			a_opos = a_pos + (a->mOldPos - a->mPos);
			r = a->Radius();
			posoff = a->mPos - a_pos;
		}

		GINT apx = a_pos.X+r ;
		GINT apy = a_pos.Y+r ;
		GINT apz = a_pos.Z+r ;
		GINT amx = a_pos.X-r ;
		GINT amy = a_pos.Y-r ;
		GINT amz = a_pos.Z-r ;

	    BOOL sphere_hits_aabb = FALSE ;
		BOOL boundary_boxes_hit = TRUE ;

		// pass 1 does the sphere AABB bounding box intersect with AABB ( faster check !!)
		if (apx < bmx ||
			amx > bpx ||
			apy < bmy ||
			amy > bpy ||
			apz < bmz ||
			amz > bpz) boundary_boxes_hit = FALSE;

		if (boundary_boxes_hit)
		{
			// ok more accurate checks
			UINT b4flags = mFlags ;
			GVector norm(G0,G0,G0) ;
			GVector new_vel = mVelocity ;
			GVector st = mVelocity ;
			GINT disx, disy, disz ;

			// find closest point on an edge of the aabb to the centre of the sphere
			GVector edge_point ;
			if (a_pos.X < bmx)
			{
				edge_point.X=bmx ;
				disx = GABS(a_pos.X - bmx) ;
				norm.X = -G1 ;
			}
			else if (a_pos.X > bpx)
			{
				edge_point.X = bpx ;
				disx = GABS(a_pos.X - bpx) ;
				norm.X = G1 ;
			}
			else
			{
				edge_point.X = a_pos.X ;
				disx=G0 ;
			}

			if (a_pos.Y < bmy)
			{
				norm.Y = -G1;
				edge_point.Y=bmy ;
				disy = GABS(a_pos.Y - bmy) ;
			}
			else if (a_pos.Y > bpy)
			{
				norm.Y = G1;
				edge_point.Y = bpy ;
				disy = GABS(a_pos.Y - bpy) ;
			}
			else
			{
				disy=G0 ;
				edge_point.Y = a_pos.Y ;
			}

			if (a_pos.Z < bmz)
			{
				norm.Z = -G1;
				edge_point.Z=bmz ;
				disz = GABS(a_pos.Z - bmz) ;
				mFlags |= TF_ON_OBJECT ;
				mFlags |= TF_ON_GROUND ;
			}
			else if (a_pos.Z > bpz)
			{
				norm.Z = G1;
				disz = GABS(a_pos.Z - bpz) ;
				edge_point.Z = bpz ;
			}
			else
			{
				disz = G0 ;
				edge_point.Z = a_pos.Z ;
			}

			if ((a_pos - edge_point).MagnitudeSq() < (r*r))
			{
				sphere_hits_aabb = TRUE ;
				something_hit = TRUE;

				if (!passive)
				{
					// are we on a corner
					if (disx != G0 && disz != G0 && disy!= G0)
					{
						if (disx < disy)
						{
							if (disy < disz) new_vel.Z = G0;	else  new_vel.Y = G0 ;
						
						}
						else if ( disx < disz) new_vel.Z = G0 ; else new_vel.X = G0 ;
					}

					// or an edge ??
					else if (disx != G0 && disy != G0)
					{
						if (disx < disy)
						{
							new_vel.Y = G0;
							st.Y = G0;
						}
						else
						{
							new_vel.X = G0;
							st.X = G0;
						}
					}
					else if (disx != G0 && disz != G0)
					{
						if (disx < disz)
						{
							new_vel.Z = G0;
							st.Z = G0;
						}
						else
						{
							if (b4flags & TF_ON_GROUND)
							{
								new_vel.X = G0;
								st.X = G0;
							}
						}
					}
					else if (disy != G0 && disz != G0)
					{
						if (disy < disz ) 
						{
							new_vel.Z = G0;
							st.Z = G0;
						}
						else
						{
							if (b4flags & TF_ON_GROUND)
							{
								new_vel.Y = G0;
								st.Y = G0;
							}
						}
					}

					// ok we are in the middle of one of the 6 planes
					else
					{
						if (disx!=G0) {new_vel.X = G0; st.X = G0;}
						if (disy!=G0) {new_vel.Y = G0; st.Y = G0;}
						if (disz!=G0) {new_vel.Z = G0; st.Z = G0;}
					}

					// ok extra safe check to make sure before we move the sphere that we
					// were trying to enter the box rather than got stuck and trying to escape
					if ((norm * mVelocity) < G0 )
					{
						Move(a_opos+posoff+new_vel) ;
						mVelocity = st ;
					}
				}
			}
		}
	}

	if (something_hit)
	{
		a->Hit(b);
		b->Hit(a);

		a->SetHitFlag();
		b->SetHitFlag();
	}
}



//******************************************************************************************
void	CActor::ProcessObjectCollision()
{
	if (this->GetCollideShape() == CSH_NONE)
		return;

	if (!(this->DoISeekCollisions()))
		return;

	CThing *t;

	this->SetHitCheckFlag();

	t = MAP.GetFirstThing(mPos, Radius());

	while (t)
	{
		while (TRUE) // really can't think of a better way...
		{
			if (t == this)
				break ; // don't collide with myself!!

			if (t->GetCollideShape() == CSH_NONE)
				break;

			// do these things hit each other?
			if ((!(this->DoIHit(t))) || (!(t->DoIHit(this))))
				break;

			if (t->DoISeekCollisions())
				if (t->GetNumber() < this->GetNumber())
					break; // only collide one way if both are trying.

			if (!(t->IsValid()))
				break;  // is this going to die in the next frame?

			t->SetHitCheckFlag();

			mCurrentACollideShape = GetCollideShape();
			mCurrentBCollideShape = t->GetCollideShape();

			switch (BYTE_PAIR(mCurrentACollideShape, mCurrentBCollideShape))
			{
			case BYTE_PAIR(CSH_SPHERE		, CSH_SPHERE):
			case BYTE_PAIR(CSH_DOUBLE_SPHERE, CSH_SPHERE):
			case BYTE_PAIR(CSH_SPHERE		, CSH_DOUBLE_SPHERE):
			case BYTE_PAIR(CSH_DOUBLE_SPHERE, CSH_DOUBLE_SPHERE):
				{
					ECollideResponse cra = GetCollideResponse();
					ECollideResponse crb = t->GetCollideResponse();
					if ((cra == CR_PASSIVE) || (crb == CR_PASSIVE))
						CollideSphereSpherePassive(this, t);
					else
					{
						switch (t->GetCollideResponse())
						{
						case CR_BOUNCE:		CollideSphereSphereBounce(this, t);		break;
						case CR_STATIC:		CollideSphereSphereStatic(this, t);		break;
						case CR_PASSIVE:	break;
						default: ASSERT(0); break;
						};
					}
				}
				break;
/*
			case BYTE_PAIR(CSH_SPHERE, CSH_COMPLEX):
			case BYTE_PAIR(CSH_DOUBLE_SPHERE, CSH_COMPLEX):
				switch (t->GetCollideResponse())
				{
				case CR_STATIC:	CollideSphereComplexStatic(this, t);	break;
				default: ASSERT(0); break;
				}
				break;
*/
			case BYTE_PAIR(CSH_SPHERE, CSH_AABB):
			case BYTE_PAIR(CSH_DOUBLE_SPHERE, CSH_AABB):
				switch (t->GetCollideResponse())
				{
				case CR_STATIC:	CollideSphereAABB(this, t, FALSE);	break;
				case CR_PASSIVE: CollideSphereAABB(this, t , TRUE); break ;
				default: ASSERT(0); break;
				}
				break;
		

			case BYTE_PAIR(CSH_SPHERE, CSH_PARTICLE_SYSTEM):
			case BYTE_PAIR(CSH_DOUBLE_SPHERE, CSH_PARTICLE_SYSTEM):
				CollideSphereParticleSystem(this, t);
				break;

			case BYTE_PAIR(CSH_SPHERE, CSH_FENCE):
			case BYTE_PAIR(CSH_DOUBLE_SPHERE, CSH_FENCE):
				{
					ECollideResponse cra = GetCollideResponse();
					ECollideResponse crb = t->GetCollideResponse();
					BOOL passive = FALSE ;
					if ((cra == CR_PASSIVE) || (crb == CR_PASSIVE)) passive = TRUE ;
					CollideSphereFence(this, t, passive);
					break;
				}
			case BYTE_PAIR(CSH_SPHERE, CSH_FENCE_X_ALLIGN):
			case BYTE_PAIR(CSH_DOUBLE_SPHERE, CSH_FENCE_X_ALLIGN):
				{
					ECollideResponse cra = GetCollideResponse();
					ECollideResponse crb = t->GetCollideResponse();
					BOOL passive = FALSE ;
					if ((cra == CR_PASSIVE) || (crb == CR_PASSIVE)) passive = TRUE ;
					CollideSphereFenceXAllign(this, t, passive);
					break;
				}
			case BYTE_PAIR(CSH_SPHERE, CSH_FENCE_Y_ALLIGN):
			case BYTE_PAIR(CSH_DOUBLE_SPHERE, CSH_FENCE_Y_ALLIGN):
				{
					ECollideResponse cra = GetCollideResponse();
					ECollideResponse crb = t->GetCollideResponse();
					BOOL passive = FALSE ;
					if ((cra == CR_PASSIVE) || (crb == CR_PASSIVE)) passive = TRUE ;
					CollideSphereFenceYAllign(this, t, passive);
					break;
				}
			case BYTE_PAIR(CSH_SPHERE, CSH_OBB2):
			case BYTE_PAIR(CSH_DOUBLE_SPHERE, CSH_OBB2):
				switch (t->GetCollideResponse())
				{
				case CR_STATIC:	CollideSphereOBBStatic(this, t, 2);	break;
				default: ASSERT(0); break;
				}
				break;
			case BYTE_PAIR(CSH_SPHERE, CSH_OBB):
			case BYTE_PAIR(CSH_DOUBLE_SPHERE, CSH_OBB):
				switch (t->GetCollideResponse())
				{
				case CR_STATIC:	CollideSphereOBBStatic(this, t, 1);	break;
				default: ASSERT(0); break;
				}
				break;
			case BYTE_PAIR(CSH_SPHERE, CSH_OBB3):
			case BYTE_PAIR(CSH_DOUBLE_SPHERE, CSH_OBB3):
				switch (t->GetCollideResponse())
				{
				case CR_STATIC:	
				{
					CollideSphereOBBStatic(this, t, 1);
					CollideSphereOBBStatic(this, t, 2);
					break;
				}
				default: ASSERT(0); break;
				}
				break;

			case BYTE_PAIR(CSH_SPHERE, CSH_OBBMOVING2):
			case BYTE_PAIR(CSH_DOUBLE_SPHERE, CSH_OBBMOVING2):
				switch (t->GetCollideResponse())
				{
				case CR_STATIC:	CollideSphereOBBMovingStatic(this, t);	break;
				default: ASSERT(0); break;
				}
				break;


			default:;
			}

			break;
		}

		t = MAP.GetNextThing();
	}
}

//******************************************************************************************
CThing	*CActor::FindFirstCollision(GVector &v, GINT r)
{
	mCachedCollisionThing = MAP.GetFirstThing(v, r);
	mCachedCollisionPos = v;
	mCachedCollisionRadiusSq = r * r;

	return FindNextCollision();
}

//******************************************************************************************
CThing	*CActor::FindNextCollision()
{
	while (mCachedCollisionThing)
	{
		if (mCachedCollisionThing != this)
		{
			GINT	d = (mCachedCollisionThing->GetPos() - mCachedCollisionPos).MagnitudeSq();
			GINT	r = mCachedCollisionThing->Radius();

			r = (r * r) + mCachedCollisionRadiusSq;

			if (r > d)
			{
				CThing *t = mCachedCollisionThing;
				mCachedCollisionThing = MAP.GetNextThing();
				return t;
			}

			if (mCachedCollisionThing->GetCollideShape() == CSH_DOUBLE_SPHERE)
			{
				d = (mCachedCollisionThing->GetPos(1) - mCachedCollisionPos).MagnitudeSq();
				r = mCachedCollisionThing->Radius2();

				r = (r * r) + mCachedCollisionRadiusSq;

				if (r > d)
				{
					CThing *t = mCachedCollisionThing;
					mCachedCollisionThing = MAP.GetNextThing();
					return t;
				}
			}
		}

		mCachedCollisionThing = MAP.GetNextThing();
	};

	return NULL;
}

//******************************************************************************************

//efine	ACTOR_MAP_DAMPING	GINT_HALF
//#define	ACTOR_MAP_DAMPING	G(0, 16384)
//#define	ACTOR_MAP_DAMPING	G(0, 1000)

void	CActor::ProcessMapCollision()
{
	//*********************************************
	// collide with stadium edges
	GINT	r = Radius();
	BOOL	hit_arena = FALSE;


	if (mPos.X - r < G0)
	{
		mPos.X = r;
		mVelocity.X = -mVelocity.X * GetMapDamping();
		hit_arena = TRUE;
	}
	if (mPos.Y - r < G0)
	{
		mPos.Y = r;
		mVelocity.Y = -mVelocity.Y * GetMapDamping();
		hit_arena = TRUE;
	}
	if (mPos.X + r > G(MAP_WIDTH - 1))
	{
		mPos.X = G(MAP_WIDTH - 1) - r;
		mVelocity.X = -mVelocity.X * GetMapDamping();
		hit_arena = TRUE;
	}
	if (mPos.Y + r > G(MAP_HEIGHT - 1))
	{
		mPos.Y = G(MAP_HEIGHT - 1) - r;
		mVelocity.Y = -mVelocity.Y * GetMapDamping();
		hit_arena = TRUE;
	}

	if (hit_arena)
		HitArena();

	if (COORDS_INVALID(CCellRef(mPos)))
		return;

	//*********************************************
	// collide with heightfield

	GINT map_height = MAP.GetMapHeight(mPos) - Radius();

	if (mPos.Z > map_height)
	{
		GINT	oldz = mVelocity.Z;

		SINT game_turn = GAME.GetGameTurn() ;
		if (mCurrentMapNormalCalcTime != game_turn )
		{
			mCurrentMapNormalCalcTime = game_turn ;
			mCurrentMapNormal = MAP.GetMapNormal(mPos) ;
		}
		
		//************************************************
		// ** START HACK
		//(JCL) - OK - quick hack to try and stop some of the more obscure popup cases
		
		// what we want to try and do is improve collision when we're going fast or the slope is very steep

		#define HALF_CHECK (-G(0, 15000))

		
		// Are we a character?
		if ((IsCharacter()) &&
			((mCurrentMapNormal.Z > HALF_CHECK) ||			// is the destination slope steep?
			 (mVelocity.MagnitudeSq() > G(0, 30000))))		// are we going fast?
		{
			// is the intermediate point on a steep slope?
			GVector	hp;
			GVector	hmn;

			hp = (mPos + mOldPos) / 2;
			hmn = MAP.GetMapNormal(hp);
			if (hmn.Z > HALF_CHECK)
			{
				// OK - so did we hit it??
				map_height = MAP.GetMapHeight(mPos) - Radius();
				if (hp.Z > map_height)
				{
					// yup - recalibrate to collide here...
					Move(hp);
					mCurrentMapNormalCalcTime = game_turn ;
					mCurrentMapNormal = hmn;
				}			
			}
		}
		
		// ** END HACK
		//************************************************

		GINT	z		= MAP.GetMapVelocity(mPos);

		// correct position
		GINT	d = ((GVector(mPos.X, mPos.Y, map_height) - mPos) * mCurrentMapNormal);
		Move(mPos + mCurrentMapNormal * d);

//		mVelocity.Z -= z;
		mVelocity.Z -= z / 2;
		if (mVelocity * mCurrentMapNormal < G0) // stop obscure pulling down cases.
		{
			GVector foo = mVelocity;
			mVelocity.ReflectInNormal(mCurrentMapNormal, GetMapDamping());
			ASSERT( foo.Z >= mVelocity.Z);
		}
//		mVelocity.Z += z;
		mVelocity.Z += z / 2;
		mFlags |= TF_ON_GROUND;

		HitGround(oldz - mVelocity.Z);
	}
}

#undef __ACTORCOLLISION__

