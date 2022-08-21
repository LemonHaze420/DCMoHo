#ifndef PSXFRONTENDTHING_H
#define PSXFRONTENDTHING_H

#include	"Thing.h"

//******************************************************************************************
class	CFrontEndThing : public IRenderableThing
{
public:
	virtual	GVector			GetRenderPos(SINT no = 0)		{if (no == 0) return mPos; else return ZERO_GVECTOR;};
	virtual	GMatrix			GetRenderOrientation(SINT no)	{if (no == 0) return mOrientation; else return ID_GMATRIX;};
	virtual	BOOL			GetRenderSelected()				{return FALSE;};
	virtual	SINT			GetRenderColourOffset()			{return 0;};
	virtual	DWORD			GetRenderColour()				{return 0xffffffff;};
	virtual GINT			GetRenderTurn()					{return 1;};
	virtual GINT			GetRenderFrame()				{return mFrame;};
	virtual SINT			GetRealAnimIndex()				{return mAnimIndex;};
	virtual	GINT			GetRenderRadius()				{return G1;};
	virtual	GVector			GetRenderStartPos()				{return mPos;};
	virtual	GVector			GetRenderEndPos()				{return mPos;};
	virtual	CSpringArray	*GetRenderSpringArray()			{return NULL;};
	virtual	CParticleSystem	*GetRenderParticleSystem()		{return NULL;};
	virtual	SINT			GetRenderMesh()					{return -1;};
	virtual	EAnimMode		GetRenderAnimation()			{return mAnimMode;};
	virtual	CTrail			*GetRenderTrail()				{return NULL;};
	virtual	BOOL			GetRenderRotateShadow()			{return FALSE;};
	virtual BOOL			GetRenderCanGoFlatShaded()		{return FALSE;};
	virtual	BOOL			RenderUseHierarchy(SINT pn)		{return TRUE;};
	virtual BOOL			RenderRenderPart(SINT pn)		{return TRUE;};
	virtual	GINT			GetRenderYaw()					{return G0;};
	virtual SINT			GetRenderThingMiscFlags() 		{return (mWhite ? RTMF_WHITE : 0) + (mBlack ? RTMF_BLACK : 0);}
	virtual GINT			GetRenderScale()				{return G1;};

	void	Reset(SINT c);
	void	SetAnimMode(EAnimMode am);
	void	Process();
	void	Select();
	
	GVector		mPos;
	GMatrix 	mOrientation;
	GINT		mFrame;
	SINT		mAnimIndex;
	EAnimMode	mAnimMode;
	
	GINT 		mYaw;
	GINT		mFrameInc;
	SINT		mChar;
	
	BOOL		mWhite;
	BOOL		mBlack;
};

#endif