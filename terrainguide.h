// PathFinder.h: interface for the CTerrainGuide class.
//
//////////////////////////////////////////////////////////////////////

#ifndef PATH_FINDER_H 
#define PATH_FINDER_H

#include "Character.h"

class CTerrainGuide  
{
public:
	CTerrainGuide();
	virtual ~CTerrainGuide();

	void SetForCharacter(CCharacter* for_character) { mForCharacter = for_character ;  mGoalPoint = for_character->GetPos(); } 
	void SetNewGoalPoint(GVector point) { mGoalPoint = point ; } ;
	void MoveMe(GINT CurrentAcell, BOOL MustTouchGoalPoint = FALSE, BOOL terrain_check = TRUE) ;
	void ChewTerrainCheck(GINT CurrentAcell, BOOL CheckTerrain = TRUE) ;
	const GVector& GoalPoint() { return mGoalPoint ; }
	BOOL	IsTerrainInDirectionSafe(GINT angle);


private:

	GVector mGoalPoint;
	CCharacter* mForCharacter;
	GVector	mPos_b4;

};

#endif 
