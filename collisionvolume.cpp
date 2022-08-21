#include	"Common.h"
#include	"CollisionVolume.h"

// not much here...

CCollisionVolume UNIT_BOX = CCollisionVolume(GVector(G0, G0, G0),
											 GVector(G1, G1, G1));

CCollisionVolume UNIT_SPHERE = CCollisionVolume(G1, GVector(G0, G0, G0));


