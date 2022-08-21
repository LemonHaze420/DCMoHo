#include	"Common.h"

#ifdef LINK_EDITOR

#include	"Editors.h"

#if EDITOR_VERSION == 1
CLevelEditor	EDITOR;
#elif EDITOR_VERSION == 2
CLevelEditor2	EDITOR;
#endif

#endif