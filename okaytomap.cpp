#include "Common.h"

#if TARGET == PC
#include "OkayToMap.h"
#include <dinput.h>
#include "text.h"

extern struct tagDIKToString DIKToString[DIK7KEYLISTLENGTH] = 
{		
	{DIK_ESCAPE,		0,								"Escape",			"Escape",			"Escape",			"Escape",			"Escape"			},
																																			
	{DIK_1,				-1,								"1",				"1",				"1",				"1",				"1"					},
	{DIK_2,				-1,								"2",				"2",				"2",				"2",				"2"					},
	{DIK_3,				-1,								"3",				"3",				"3",				"3",				"3"					},
	{DIK_4,				-1,								"4",				"4",				"4",				"4",				"4"					},
	{DIK_5,				-1,								"5",				"5",				"5",				"5",				"5"					},
	{DIK_6,				-1,								"6",				"6",				"6",				"6",				"6"					},
	{DIK_7,				-1,								"7",				"7",				"7",				"7",				"7"					},
	{DIK_8,				-1,								"8",				"8",				"8",				"8",				"8"					},
	{DIK_9,				-1,								"9",				"9",				"9",				"9",				"9"					},
	{DIK_0,				-1,								"0",				"0",				"0",				"0",				"0"					},
																																							
	{DIK_MINUS,			-1,								"-",				"-",				"-",				"-",				"-"					},
	{DIK_EQUALS,		-1,								"=",				"=",				"=",				"=",				"="					},
	{DIK_BACK,			-1,								"Backspace",		"Retour arrière,",	"Backspace",		"Retroceso",		"Indietro"			},
	{DIK_TAB,			-1,								"Tab",				"Tabulation",		"Tab",				"Tabulador",		"Tab"				},
																																							
	{DIK_Q,				-1,								"Q",				"Q",				"Q",				"Q",				"Q"					},
	{DIK_W,				-1,								"W",				"W",				"W",				"W",				"W"					},
	{DIK_E,				-1,								"E",				"E",				"E",				"E",				"E"					},
	{DIK_R,				-1,								"R",				"R",				"R",				"R",				"R"					},
	{DIK_T,				-1,								"T",				"T",				"T",				"T",				"T"					},
	{DIK_Y,				-1,								"Y",				"Y",				"Y",				"Y",				"Y"					},
	{DIK_U,				-1,								"U",				"U",				"U",				"U",				"U"					},
	{DIK_I,				-1,								"I",				"I",				"I",				"I",				"I"					},
	{DIK_O,				-1,								"O",				"O",				"O",				"O",				"O"					},
	{DIK_P,				-1,								"P",				"P",				"P",				"P",				"P"					},
																																							
	{DIK_LBRACKET,		-1,								"[",				"[",				"[",				"[",				"["					},
	{DIK_RBRACKET,		-1,								"]",				"]",				"]",				"]",				"]"					},
	{DIK_RETURN,		-1,								"Return",			"Validation",		"Return",			"Retorno",			"Invio"				},
	{DIK_LCONTROL,		-1,								"Left Ctrl",		"Ctrl gauche",		"Strg links",		"Control izqda.",	"Crtl Sinistro"		},
																																							
	{DIK_A,				-1,								"A",				"A",				"A",				"A",				"A"					},
	{DIK_S,				-1,								"S",				"S",				"S",				"S",				"S"					},
	{DIK_D,				-1,								"D",				"D",				"D",				"D",				"D"					},
	{DIK_F,				-1,								"F",				"F",				"F",				"F",				"F"					},
	{DIK_G,				-1,								"G",				"G",				"G",				"G",				"G"					},
	{DIK_H,				-1,								"H",				"H",				"H",				"H",				"H"					},
	{DIK_J,				-1,								"J",				"J",				"J",				"J",				"J"					},
	{DIK_K,				-1,								"K",				"K",				"K",				"K",				"K"					},
	{DIK_L,				-1,								"L",				"L",				"L",				"L",				"L"					},
																																							
	{DIK_SEMICOLON,		-1,								";",				";",				";",				";",				";"					},
	{DIK_APOSTROPHE,	-1,								"'",				"'",				"'",				"'",				"'"					},
	{DIK_GRAVE,			-1,								"#",				"#",				"#",				"#",				"#"					},
	{DIK_LSHIFT,		-1,								"Left Shift",		"MAJ gauche",		"Shift links",		"Mayúsculas izqda",	"Shift Sinistro"	},
	{DIK_BACKSLASH,		-1,								"\\",				"\\",				"\\",				"\\",				"\\"				},
																																							
	{DIK_Z,				-1,								"Z",				"Z",				"Z",				"Z",				"Z"					},
	{DIK_X,				-1,								"X",				"X",				"X",				"X",				"X"					},
	{DIK_C,				-1,								"C",				"C",				"C",				"C",				"C"					},
	{DIK_V,				-1,								"V",				"V",				"V",				"V",				"V"					},
	{DIK_B,				-1,								"B",				"B",				"B",				"B",				"B"					},
	{DIK_N,				-1,								"N",				"N",				"N",				"N",				"N"					},
	{DIK_M,				-1,								"M",				"M",				"M",				"M",				"M"					},
																																							
	{DIK_COMMA,			-1,								",",				",",				",",				",",				","					},
	{DIK_PERIOD,		-1,								".",				".",				".",				".",				"."					},
	{DIK_SLASH,			-1,								"/",				"/",				"/",				"/",				"/"					},
	{DIK_RSHIFT,		-1,								"Right Shift",		"MAJ droite",		"Shift rechts",		"Mayúsculas dcha",	"Shift Destro"		},
	{DIK_MULTIPLY,		-1,								"*",				"*",				"*",				"*",				"*"					},
	{DIK_LMENU,			0,								"Left Alt",			"Left Alt",			"Left Alt",			"Left Alt",			"Left Alt"			},
	{DIK_SPACE,			-1,								"Space",			"Espace",			"Space",			"Espacio",			"Spazio"			},
	{DIK_CAPITAL,		0,								"Caps Lock",		"Caps Lock",		"Caps Lock",		"Caps Lock",		"Caps Lock"			},
																																							
	{DIK_F1,			-1,								"F1",				"F1",				"F1",				"F1",				"F1"				},
	{DIK_F2,			-1,								"F2",				"F2",				"F2",				"F2",				"F2"				},
	{DIK_F3,			-1,								"F3",				"F3",				"F3",				"F3",				"F3"				},
	{DIK_F4,			-1,								"F4",				"F4",				"F4",				"F4",				"F4"				},
	{DIK_F5,			-1,								"F5",				"F5",				"F5",				"F5",				"F5"				},
	{DIK_F6,			-1,								"F6",				"F6",				"F6",				"F6",				"F6"				},
	{DIK_F7,			-1,								"F7",				"F7",				"F7",				"F7",				"F7"				},
	{DIK_F8,			-1,								"F8",				"F8",				"F8",				"F8",				"F8"				},
	{DIK_F9,			-1,								"F9",				"F9",				"F9",				"F9",				"F9"				},
	{DIK_F10,			-1,								"F10",				"F10",				"F10",				"F10",				"F10"				},
																																							
	{DIK_NUMLOCK,		-1,								"Num lock",			"Ver. num",			"Zahlenblock sperren",	"Bloq. Num",	"Bloc Num"			},
	{DIK_SCROLL,		-1,								"Scroll lock",		"Défil",			"Scroll lock",		"Bloq desplazamiento",		"Bloc Scorr"},
	{DIK_NUMPAD7,		-1,								"Numpad 7",			"Pavé numérique 7",			"Zahlenblock 7",			"Teclado numérico 7",			"Numpad 7"			},
	{DIK_NUMPAD8,		-1,								"Numpad 8",			"Pavé numérique 8",			"Zahlenblock 8",			"Teclado numérico 8",			"Numpad 8"			},
	{DIK_NUMPAD9,		-1,								"Numpad 9",			"Pavé numérique 9",			"Zahlenblock 9",			"Teclado numérico 9",			"Numpad 9"			},
	{DIK_SUBTRACT,		-1,								"Numpad -",			"Pavé numérique -",			"Zahlenblock -",			"Teclado numérico -",			"Numpad -"			},
	{DIK_NUMPAD4,		-1,								"Numpad 4",			"Pavé numérique 4",			"Zahlenblock 4",			"Teclado numérico 4",			"Numpad 4"			},
	{DIK_NUMPAD5,		-1,								"Numpad 5",			"Pavé numérique 5",			"Zahlenblock 5",			"Teclado numérico 5",			"Numpad 5"			},
	{DIK_NUMPAD6,		-1,								"Numpad 6",			"Pavé numérique 6",			"Zahlenblock 6",			"Teclado numérico 6",			"Numpad 6"			},
	{DIK_ADD,			-1,								"Numpad +",			"Pavé numérique +",			"Zahlenblock +",			"Teclado numérico +",			"Numpad +"			},
	{DIK_NUMPAD1,		-1,								"Numpad 1",			"Pavé numérique 1",			"Zahlenblock 1",			"Teclado numérico 1",			"Numpad 1"			},
	{DIK_NUMPAD2,		-1,								"Numpad 2",			"Pavé numérique 2",			"Zahlenblock 2",			"Teclado numérico 2",			"Numpad 2"			},
	{DIK_NUMPAD3,		-1,								"Numpad 3",			"Pavé numérique 3",			"Zahlenblock 3",			"Teclado numérico 3",			"Numpad 3"			},
	{DIK_NUMPAD0,		-1,								"Numpad 0",			"Pavé numérique 0",			"Zahlenblock 0",			"Teclado numérico 0",			"Numpad 0"			},
	{DIK_DECIMAL,		-1,								"Numpad .",			"Pavé numérique .",			"Zahlenblock .",			"Teclado numérico .",			"Numpad ."			},
																																							
	{DIK_OEM_102,		0,								"OEM 102",			"OEM 102",			"OEM 102",			"OEM 102",			"OEM 102"			},
	{DIK_F11,			-1,								"F11",				"F11",				"F11",				"F11",				"F11"				},
	{DIK_F12,			-1,								"F12",				"F12",				"F12",				"F12",				"F12"				},

	// DIDEVTYPEKEYBOARD_NEC98 only
	{DIK_F13,			0,								"F13",				"F13",				"F13",				"F13",				"F13"				},
	{DIK_F14,			0,								"F14",				"F14",				"F14",				"F14",				"F14"				},
	{DIK_F15,			0,								"F15",				"F15",				"F15",				"F15",				"F15"				},

	{DIK_KANA,			0,								"Kana",				"Kana",				"Kana",				"Kana",				"Kana"				},	
	{DIK_ABNT_C1,		0,								"Abtn C1",			"Abtn C1",			"Abtn C1",			"Abtn C1",			"Abtn C1"			},
	{DIK_CONVERT,		0,								"Convert",			"Convert",			"Convert",			"Convert",			"Convert"			},
	{DIK_NOCONVERT,		0,								"NoConvert",		"NoConvert",		"NoConvert",		"NoConvert",		"NoConvert"			},	
	{DIK_YEN,			0,								"Yen",				"Yen",				"Yen",				"Yen",				"Yen"				},
	{DIK_ABNT_C2,		0,								"Abtn C2",			"Abtn C2",			"Abtn C2",			"Abtn C2",			"Abtn C2"			},
	{DIK_NUMPADEQUALS,	0,								"Numpad =",			"Numpad =",			"Numpad =",			"Numpad =",			"Numpad ="			},
	{DIK_PREVTRACK,		0,								"Previous Track",	"Previous Track",	"Previous Track",	"Previous Track",	"Previous Track"	},
	{DIK_AT,			0,								"AT",				"AT",				"AT",				"AT",				"AT"				},
	{DIK_COLON,			0,								":",				":",				":",				":",				":"					},
	{DIK_UNDERLINE,		0,								"_",				"_",				"_",				"_",				"_"					},
	{DIK_KANJI,			0,								"Kanji",			"Kanji",			"Kanji",			"Kanji",			"Kanji"				},	

	{DIK_STOP,			0,								"Stop",				"Stop",				"Stop",				"Stop",				"Stop"				},
	{DIK_AX,			0,								"AX",				"AX",				"AX",				"AX",				"AX"				},
	{DIK_UNLABELED,		0,								"",					"",					"",					"",					""					},
	{DIK_NEXTTRACK,		0,								"Next Track",		"Next Track",		"Next Track",		"Next Track",		"Next Track"		},
	{DIK_NUMPADENTER,	-1,								"Numpad Enter",		"Numpad Enter",		"Numpad Enter",		"Numpad Enter",		"Numpad Enter"		},
	{DIK_RCONTROL,		-1,								"Right Ctrl",		"Ctrl droite",		"Strg rechts",		"Control dcha",		"Ctrl Destro"		},
	{DIK_MUTE,			0,								"Mute",				"Mute",				"Mute",				"Mute",				"Mute"				},
	{DIK_CALCULATOR,	0,								"Calculator",		"Calculator",		"Calculator",		"Calculator",		"Calculator"		},
	{DIK_PLAYPAUSE,		0,								"Play / Pause",		"Play / Pause",		"Play / Pause",		"Play / Pause",		"Play / Pause"		},
	{DIK_MEDIASTOP,		0,								"Media Stop",		"Media Stop",		"Media Stop",		"Media Stop",		"Media Stop"		},
	{DIK_VOLUMEDOWN,	0,								"Volume Down",		"Volume Down",		"Volume Down",		"Volume Down",		"Volume Down"		},
	{DIK_VOLUMEUP,		0,								"Volume Up",		"Volume Up",		"Volume Up",		"Volume Up",		"Volume Up"			},
	{DIK_WEBHOME,		0,								"Web Home",			"Web Home",			"Web Home",			"Web Home",			"Web Home"			},
	{DIK_NUMPADCOMMA,	0,								"Numpad ,",			"Numpad ,",			"Numpad ,",			"Numpad ,",			"Numpad ,"			},
	{DIK_DIVIDE,		-1,								"Numpad /",			"Numpad /",			"Numpad /",			"Numpad /",			"Numpad /"			},
	{DIK_SYSRQ,			-1,								"System Req",		"Requête du système","System Req",		"Solicitud del sistema",		"Requisito di sistema"		},
	{DIK_RMENU,			0,								"Right Alt",		"Right Alt",		"Right Alt",		"Right Alt",		"Right Alt"			},
	{DIK_PAUSE,			-1,								"Pause",			"Pause",			"Pause",			"Pausa",			"Pausa"				},
	{DIK_HOME,			-1,								"Home",				"Début",			"Home",				"Inicio",			"Home"				},
	{DIK_UP,			-1,								"Cursor Up",		"Curseur haut",		"Cursor nach oben",	"Cursor arriba",	"Cursore sù"		},
	{DIK_PRIOR,			-1,								"Page Up",			"Pg. Suiv",			"Bild aufwärts",	"Re Pág",			"Pagina sù"			},
	{DIK_LEFT,			-1,								"Cursor Left",		"Curseur gauche",	"Cursor links",		"Cursor izqda",		"Cursore a sinistra"},
	{DIK_RIGHT,			-1,								"Cursor Right",		"Curseur droite",	"Cursor rechts",	"Cursor dcha",		"Cursore a destra"	},
	{DIK_END,			-1,								"End",				"Fin",				"Ende",				"Fin",				"Fine"				},
	{DIK_DOWN,			-1,								"Cursor Down",		"Curseur bas",		"Cursor nach unten","Cursor aPagina giù","Cursore giù"		},
	{DIK_NEXT,			-1,								"Page Down",		"Pg. Préc",			"Bild abwärts",		"Av Pág",			"Pagina giù"		},
	{DIK_INSERT,		-1,								"Insert",			"Insertion",		"Einlegen",			"Insertar",			"Inserire"			},
	{DIK_DELETE,   		-1,								"Delete",			"Suppr",			"Löschen",			"Eliminar",			"Cancellare"		},
	{DIK_LWIN,			0,								"Left Start",		"Left Start",		"Left Start",		"Left Start",		"Left Start"		},
	{DIK_RWIN,			0,								"Right Start",		"Right Start",		"Right Start",		"Right Start",		"Right Start"		},
	{DIK_APPS,			0,								"Apps",				"Apps",				"Apps",				"Apps",				"Apps"				},
	{DIK_POWER,			0,								"Power",			"Power",			"Power",			"Power",			"Power"				},
	{DIK_SLEEP,			0,								"Sleep",			"Sleep",			"Sleep",			"Sleep",			"Sleep"				},
	{DIK_WAKE,			0,								"Num Lock",			"Num Lock",			"Num Lock",			"Num Lock",			"Num Lock"			},
	{DIK_WEBSEARCH,		0,								"Web Search",		"Web Search",		"Web Search",		"Web Search",		"Web Search"		},
	{DIK_WEBFAVORITES,	0,								"Web Favourites",	"Web Favourites",	"Web Favourites",	"Web Favourites",	"Web Favourites"	},
	{DIK_WEBREFRESH,	0,								"Web Refresh",		"Web Refresh",		"Web Refresh",		"Web Refresh",		"Web Refresh"		},
	{DIK_WEBSTOP,		0,								"Web Stop",			"Web Stop",			"Web Stop",			"Web Stop",			"Web Stop"			},
	{DIK_WEBFORWARD,	0,								"Web Forward",		"Web Forward",		"Web Forward",		"Web Forward",		"Web Forward"		},
	{DIK_WEBBACK,		0,								"Web Back",			"Web Back",			"Web Back",			"Web Back",			"Web Back"			},
	{DIK_MYCOMPUTER,	0,								"My Computer",		"My Computer",		"My Computer",		"My Computer",		"My Computer"		},
	{DIK_MAIL,			0,								"Mail",				"Mail",				"Mail",				"Mail",				"Mail"				},
	{DIK_MEDIASELECT,	0,								"Media Select",		"Media Select",		"Media Select",		"Media Select",		"Media Select"		}
};

char* DIScanCodeToKeyString(int _DIScanCode, int _Language, DWORD _DevType)
{
	// Scan code in known range.
	if (	(_DIScanCode >=0) 
		&&	(_DIScanCode < MAX_KEY_INDEX)
		)
	{
		if (GET_DIDEVICE_TYPE(_DevType) == DIDEVTYPE_KEYBOARD)	// Is it a keyboard device
		{
			for (int i = min(_DIScanCode, DIK7KEYLISTLENGTH); i >= 0; i--)
			{
				if (DIKToString[i].DIK == _DIScanCode)
				{
					if 	(	(DIKToString[i].KeyboardType == -1)										// The key is either on all keyboards,
						||	((2 << GET_DIDEVICE_SUBTYPE(_DevType)) & DIKToString[i].KeyboardType)	// or is defined for this keyboard
						)
				
					{
						switch (_Language)
						{
						case LANG_ENGLISH:
							return DIKToString[i].English;
						case LANG_FRENCH:
							return DIKToString[i].French;
						case LANG_GERMAN:
							return DIKToString[i].German;
						case LANG_SPANISH:
							return DIKToString[i].Spanish;
						case LANG_ITALIAN:
							return DIKToString[i].Italian;
						default:
							return NULL;
						}
					}
				}
			}
		}
	}
	return NULL;
}

#endif