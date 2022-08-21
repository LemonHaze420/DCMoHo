#ifndef MESH_COMMON_H
#define MESH_COMMON_H


#define		MAX_CHILDREN 25
#define		MAX_SKINS	3

// interface for mesh stuff

class	CThing;

class	IMeshPart
{
public:
	virtual	void	EvaluatePointVelocity(GVector pos, GVector &velocity, CThing *t) = 0;
};

// numbered mesh defines

#define NM_STADIUM_WALL1	0
#define NM_STADIUM_WALL2	1
#define NM_STADIUM_WALL3	2
#define NM_STADIUM_WALL4	3
#define NM_STADIUM_CORNER	4
#define NM_STADIUM_WALL60	5

#define	NM_NUMBER			6

#endif