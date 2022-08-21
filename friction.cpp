#include	"Common.h"

#include	"Friction.h"




#if TARGET == PC

EFrictionType	CFriction::GetFrictionForTexture(char *name)
{
	// + 6 to avoid the "tiles\\"
//	name += 6;

	if (!(strnicmp(name, "hp_", 3)))		return FT_FAST;
	if (!(strnicmp(name + 2, "_hp", 3)))	return FT_FAST;

	if (
		(!(stricmp(name, "1concrete.tga"))) ||
		(!(stricmp(name, "tsheet.tga"))) ||
		(!(stricmp(name, "tsheet2.tga")))
//		(!(stricmp(name, "tplate2.tga"))) ||
//		(!(stricmp(name, "truster.tga"))) ||
//		(!(stricmp(name, "mplate3.tga"))) ||
//		(!(stricmp(name, "mplate2.tga"))) ||
//		(!(stricmp(name, "mplate1.tga"))) ||
//		(!(stricmp(name, "mi_haz.tga")))
		)
		return FT_FAST;

	if (
/*		(!(stricmp(name, "1rock1.tga"))) ||
		(!(stricmp(name, "1rockr.tga"))) ||
		(!(stricmp(name, "1rocky.tga"))) ||
		(!(stricmp(name, "1rocky2.tga"))) ||
		(!(stricmp(name, "frock1.tga"))) ||
		(!(stricmp(name, "frock2.tga"))) ||
		(!(stricmp(name, "frocka.tga"))) ||
		(!(stricmp(name, "hrock.tga"))) ||
		(!(stricmp(name, "hrock2.tga"))) ||*/
		(!(stricmp(name, "hslime.tga"))) ||
//		(!(stricmp(name, "mrock.tga"))) ||
//		(!(stricmp(name, "mrock2.tga"))) ||
//		(!(stricmp(name, "wrock2.tga"))) ||
		(!(stricmp(name, "tkeymud.tga"))) ||
//		(!(stricmp(name, "trust2.tga"))) ||
//		(!(stricmp(name, "trust3.tga"))) ||
		(!(stricmp(name, "wmud.tga")))
//		(!(stricmp(name, "wstoney.tga"))) ||
//		(!(stricmp(name, "wstoney2.tga")))
		)
		return FT_SLOW;

//	if (!(stricmp(name, "")))	return FT_SLOW;

	if (!(strnicmp(name, "ice", 3)))	return FT_ICE;

	return FT_NORMAL;
}




#endif
