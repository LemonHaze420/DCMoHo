#include "Common.h"

#if TARGET == PC
#ifndef	_PCSAVEROUTINES_H_
#define	_PCSAVEROUTINES_H_

#include "Career.h"
#include <stdio.h>
#include <windows.h>

// Max number of files.
//#define MAX_NUM_MOHO_FILES	256
#define MOHO_REGISTRY_PATH	"Software\\LostToys\\MoHo"

enum tagPCFileRoutinesReturnCodes
{
	McErrNone,				// No error
	McErrBlockFull,			// Already have MAX_NUM_MOHO_FILES saves
	McErrAlreadyExist,		// File exists (saving)
	McErrFileDoesntExist,	// File does NOT exist (loading)
	McErrFileError,			// Generic file error
	McWrongVersion			// Wrong version (loading).
};

// Structure to store information about a single save game file.
struct tagPCFileList
{
	char			name[MAX_PATH];
	SINT			size;
	tagPCFileList*	next;
};

extern tagPCFileList* PCFileList;//[MAX_NUM_MOHO_FILES];	// Array of structures.
extern SINT PCNumberOfSaveFiles;		// Guess!

// Initialise the file listing system - why the hell don't 
// I make this an object and make this a constructor.
void PCStartFileList();

// Free allocated memory.
void PCEndFileList();

// Returns the number of saved game files found.
// NOTE:	This function does not check the directory 
//			when called. You will need to call 
//			PCRefreshFileList inorder to refresh the file 
//			listing.
inline SINT PCNumFiles() {return PCNumberOfSaveFiles;}

// Call this method to build/rebuild a list of the save game files.
// Returns the number of files found.
SINT PCRefreshFileList();

// Access a particular save record. 
// Will return null if value of _Index is out of bounds.
tagPCFileList *PCGetSaveNameByIndex(SINT _Index);

// Alias for PCRefreshFileList.
inline SINT PCAcquireCard() {return PCRefreshFileList();}

// Loads a given file into the CCareer object referenced by _RefCareer.
SINT PCLoadCareer(char* _FileName, CCareer* _RefCareer);

// Saves a binary image of the CCareer object referenced 
// by _RefCareer. 
// _OverWriteIfExists specifies whether or not the 
// function should over write an existing file.
SINT PCSaveCareer(char* _FileName, CCareer* _RefCareer, bool _OverWriteIfExists);

// Delete a saved game.
void PCDeleteCareer(char* _FileName);

// Delete all saved game files.
void PCPurgeCareers();

// Validates that a key exists.
bool PCValidateRegistry(HKEY	_ExistingKey,	// Existing key or default root.
						char*	_SubKey,		// Sub key, relative to _ExistingKey. (Must not start with a '/' char!)
						char*	_ValueName,		// Name of value to retrieve.
						DWORD	_ValueType);


// Registry load / save routines.
// Both functions return the values returned by the calls to the registry. 
// Zero indicates success, and a non-zero value indicates an error.
LONG PCLoadFromRegistry(HKEY	_ExistingKey,	// Existing key or default root.
						char*	_SubKey,		// Sub key, relative to _ExistingKey. (Must not start with a '/' char!)
						char*	_ValueName,		// Name of value to retrieve.
						DWORD*	_ValueType,		// A pointer to a DWORD filled-in by the function.
						char**	_Value,			// A pointer to buffer created within the function - you are responsible for the freeing of this resource.
						DWORD*	_DataLength);	// A pointer to a DWORD filled-in by the function that indicates the length of the data (in bytes) returned for _Value.

LONG PCSaveToRegistry(	HKEY	_ExistingKey,	// Existing key or default root.
						char*	_SubKey,		// Sub key, relative to _ExistingKey. (Must not start with a '/' char!)
						char*	_ValueName,		// Name of the value to store.
						DWORD	_ValueType,		// Specifies the format for storing the data.
						char*	_Value,			// A pointer to the data to store.
						DWORD	_DataLength);	// Length of the _Value data, in bytes.

LONG PCRemoveRegistryValue(	HKEY	_ExistingKey,	// Existing key or default root.
							char*	_SubKey,		// Sub key, relative to _ExistingKey. (Must not start with a '/' char!)
							char*	_ValueName);		// Name of the value to remove

#endif	// _PCSAVEROUTINES_H_
#endif	// TARGET == PC