#ifndef FRICTION_H
#define FRICTION_H

enum	EFrictionType
{
	FT_NORMAL	= 0,
	FT_SLOW		= 1,
	FT_FAST		= 2,
	FT_ICE		= 3,
};

#if TARGET == PC
class	CFriction
{
public:
	static	EFrictionType	GetFrictionForTexture(char *name);
};
#endif






#endif