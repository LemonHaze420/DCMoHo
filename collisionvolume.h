#ifndef COLLISION_VOLUME_H
#define COLLISION_VOLUME_H

//******************************************************************************************
enum EVolumeType
{
	VT_BOUNDING_BOX,
	VT_SPHERE
};

//******************************************************************************************
class	IMeshPart;

class	CCollisionVolume
{
public:	

	// Box constructor
	CCollisionVolume(const GVector &origin,
					 const GVector &axes,
					 GMatrix *orientation = NULL,
					 CCollisionVolume *next = NULL,
					 CCollisionVolume *nextpart = NULL,
					 IMeshPart *meshpart = NULL)
	{
		mType = VT_BOUNDING_BOX;
		mOrigin = origin;
		mAxes = axes;
		mOrientation = orientation ? *orientation : ID_GMATRIX;
		mTransformedOrigin = origin;
		mTransformedOrientation = mOrientation;
		mNext = next;
		mNextPart = nextpart;
		mMeshPart = meshpart;
	};

	// Sphere constructor
	CCollisionVolume(GINT radius,
					 const GVector &origin,
					 CCollisionVolume *next = NULL,
					 CCollisionVolume *nextpart = NULL,
					 IMeshPart *meshpart = NULL)
	{
		mType = VT_SPHERE;
		mOrigin = origin;
		mAxes = GVector(radius, G(0), G(0));
		mOrientation = ID_GMATRIX;
		mTransformedOrigin = origin;
		mTransformedOrientation = mOrientation;
		mNext = next;
		mNextPart = nextpart;
		mMeshPart = meshpart;
	};

	EVolumeType	mType;
	GVector		mOrigin;
	GVector		mAxes;
	GMatrix		mOrientation;

	// cached transforms
	GVector		mTransformedOrigin;
	GMatrix		mTransformedOrientation;

	CCollisionVolume	*mNext; // for chaining
	CCollisionVolume	*mNextPart;
	
	IMeshPart	*mMeshPart;
};

//******************************************************************************************
extern CCollisionVolume UNIT_BOX;
extern CCollisionVolume UNIT_SPHERE;

//******************************************************************************************

#endif