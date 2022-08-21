#include	"Common.h"

#include	"Thing.h"
#include	"Light.h"

//******************************************************************************************
void	CEditorLight::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	GET_COUNTER(0);

	if (mCounter0 == -1)
		mCounter0 = rand() % 7;

	if (mCounter0 > 6)
		mCounter0 = 6;

	SitOnGround();
	mFlags |= TF_RENDER_IN_EDITOR_ONLY;
}


//********************************************************************************************************
#if TARGET == PC
void	CEditorLight::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
}
#endif


