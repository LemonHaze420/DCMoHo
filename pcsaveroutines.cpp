#include "Common.h"

#if TARGET == PC

#include "PCSaveRoutines.h"
#include "Career.h"
#include <stdio.h>
#include <io.h>
#include <sys/types.h> 
#include <sys/stat.h>

/*
	McErrNone,				// No error
	McErrBlockFull,			// Already have MAX_NUM_MOHO_FILES saves
	McErrAlreadyExist,		// File exists (saving)
	McErrFileDoesntExist,	// File does NOT exist (loading)
	McErrFileError,			// Generic file error
	McWrongVersion			// Wrong version (loading).
*/

#define MOHO_REG_SAVE_PATH_PATH	"SavePath"

tagPCFileList *PCFileList;//[MAX_NUM_MOHO_FILES];	// Array of structures.
SINT PCNumberOfSaveFiles;

// Initialise the file listing system - why the hell don't 
// I make this an object and make this a constructor.
void PCStartFileList()
{
	PCFileList = NULL;

	// Get a list of files.
	PCRefreshFileList();
}

// Free allocated memory.
void PCEndFileList()
{
	tagPCFileList*		i;

	// Delete existing file list.
	while (PCFileList)
	{	
		i = PCFileList;
		PCFileList = PCFileList->next;
		delete i;
	}
}



// Call this method to build/rebuild a list of the save game files.
// Returns the number of files found.
SINT PCRefreshFileList()
{
	struct _finddata_t	foundFile;
	long				findHandle;
	SINT				foundCount = 0;
	char				path[MAX_PATH];
	char				ExpandedSaveDir[MAX_PATH];
	char*				SaveDir;
	DWORD				TypeBuffer, DataLength;
	FILE*				FH;
	CCareer				TempJob; 
	char				FileBuffer[sizeof(CCareer)];
	char				FilePath[MAX_PATH];

	tagPCFileList*		i;
	tagPCFileList*		j;

	// Delete existing file list.
	while (PCFileList)
	{	
		i = PCFileList;
		PCFileList = PCFileList->next;
		delete i;
	}
	i = NULL;
	PCNumberOfSaveFiles = 0;

	// Check buffer.
	if (!FileBuffer) return 0;

	ExpandedSaveDir[0] = 0;

	// Obtain load save path information from registry.
	if (!PCLoadFromRegistry(HKEY_CURRENT_USER, 
							MOHO_REGISTRY_PATH,
							MOHO_REG_SAVE_PATH_PATH,
							&TypeBuffer,
							&SaveDir,
							&DataLength)
		)
	{
		if ((TypeBuffer == REG_EXPAND_SZ) || 
			(TypeBuffer == REG_SZ))
		{
			if (!ExpandEnvironmentStrings(SaveDir, ExpandedSaveDir, MAX_PATH))
			{
				ExpandedSaveDir[0] = 0;
			}	
		}

		delete[] SaveDir;
	}

	if (strlen(ExpandedSaveDir) > 0)
	{
		char testpath[MAX_PATH];
		strcpy(testpath, ExpandedSaveDir);

		if (testpath[strlen(testpath) - 1] == '\\')
		{
			testpath[strlen(testpath) - 1] = 0;
		}
		else
		{
			// No \ eh? Better add one.
			strcat(ExpandedSaveDir, "\\");
		}

		struct _stat buf;
		// Check that the path is valid, else ignore path variable and just save in current dir.
		if (!_stat(testpath, &buf))
		{
			if (!(buf.st_mode & _S_IFDIR))
			{
				// Not a directory, or can't write.
				ExpandedSaveDir[0] = 0;
			}
		}
		else
		{
			// Failed to get info, assume path is invalid.
			ExpandedSaveDir[0] = 0;
		}
	}

	// Get save/load path
	sprintf(path, "%s*.msf", ExpandedSaveDir);
	OutputDebugString("Looking for saved games matching: ");
	OutputDebugString(path);
	OutputDebugString("\n");

	// There must be a better way of getting a directory listing!
	findHandle = _findfirst(path, &foundFile);
	if (findHandle != -1)
	{
		// Compare the file name and save name match - enforcing a joint primary key.
		sprintf(FilePath, "%s%s", ExpandedSaveDir, foundFile.name);
		FH = fopen(FilePath, "rb");
		if (FH)
		{			
			// Read data into buffer and copy buffer into CCareer object.
			fread(FileBuffer, sizeof(CCareer), 1, FH);		
			memcpy(&TempJob, FileBuffer, sizeof(CCareer));
			fclose(FH);

			// Compare strings
			*strrchr(foundFile.name, '.') = 0;
			if (!_stricmp(TempJob.mName, foundFile.name))
			{
				PCFileList = new tagPCFileList;
				PCFileList->next = NULL;
				i = PCFileList;
				strcpy(PCFileList->name, TempJob.mName);
				foundCount = 1;
			}
		}

		// Loop through remaining files that made the match.
		while (		(!_findnext(findHandle, &foundFile))
				//&&	(foundCount < MAX_NUM_MOHO_FILES)
				)
		{
			// Compare the file name and save name match - enforcing a joint primary key.
			sprintf(FilePath, "%s%s", ExpandedSaveDir, foundFile.name);
			FH = fopen(FilePath, "rb");
			if (FH)
			{			
				// Read data into buffer and copy buffer into CCareer object.
				fread(FileBuffer, sizeof(CCareer), 1, FH);		
				memcpy(&TempJob, FileBuffer, sizeof(CCareer));
				fclose(FH);

				// Compare strings
				*strrchr(foundFile.name, '.') = 0;
				if (!_stricmp(TempJob.mName, foundFile.name))
				{	
					if (!i)
					{
						i = new tagPCFileList;
						i->next = NULL;	
						PCFileList = i;
					}
					else
					{
						i->next = new tagPCFileList;
						i = i->next;
						i->next = NULL;	
					}

					strcpy(i->name, TempJob.mName);
					foundCount++;
				}
			}
		}
		_findclose(findHandle);


		if (foundCount > 0)
		{
			tagPCFileList* SortItem;	// Current item
			tagPCFileList* Sorted;		// End up here

			Sorted = PCFileList;
			PCFileList = PCFileList->next;

			Sorted->next = NULL;

			// Now sort.
			for (int count = foundCount - 1; count--;)
			{
				SortItem = PCFileList;			// get pointer to head of unsorted list
				PCFileList = PCFileList->next;	// Move head
				
				SortItem->next = NULL;			// Set chopped of head's next to null for safety.

				i = Sorted;						// i is head of sorted list.
				j = NULL;
				while (i->next)	// whilst not at end of list and list item is greater than insert item.
				{
					if (_stricmp(i->name, SortItem->name) < 0)
						break;
					j = i;
					i = i->next;
				}

				// Insert!
				SortItem->next = i;
				if (j)
					j->next = SortItem;
				else
					Sorted = SortItem;
			}

			PCFileList = Sorted;
		}
	}

	return (PCNumberOfSaveFiles = foundCount);
}



// Access a particular save record. 
// Will return null if value of _Index is out of bounds.
tagPCFileList *PCGetSaveNameByIndex(SINT _Index)
{
	tagPCFileList *i = PCFileList;
	for (SINT Count = PCNumberOfSaveFiles - _Index - 1; Count--;)
	{
		if (i)
			i = i->next;
		else
			return NULL;
	}

	return i;
}

// Loads a given file into the CCareer object referenced by _RefCareer.
SINT PCLoadCareer(char* _FileName, CCareer* _RefCareer)
{
	FILE*	fh;
	char	buffer[sizeof(CCareer)];
	char	filePath[MAX_PATH], ExpandedSaveDir[MAX_PATH];
	char*	SaveDir;
	DWORD	TypeBuffer, DataLength;

	ExpandedSaveDir[0] = 0;
	if (!PCLoadFromRegistry(HKEY_CURRENT_USER, 
							MOHO_REGISTRY_PATH,
							MOHO_REG_SAVE_PATH_PATH,
							&TypeBuffer,
							&SaveDir,
							&DataLength)
		)
	{
		if ((TypeBuffer == REG_EXPAND_SZ) || 
			(TypeBuffer == REG_SZ))
		{
			if (!ExpandEnvironmentStrings(SaveDir, ExpandedSaveDir, MAX_PATH))
			{
				ExpandedSaveDir[0] = 0;
			}
			OutputDebugString(ExpandedSaveDir);
		}
		delete[] SaveDir;
		SaveDir = NULL;
	}

	if (strlen(ExpandedSaveDir) > 0)
	{
		char testpath[MAX_PATH];
		strcpy(testpath, ExpandedSaveDir);

		if (testpath[strlen(testpath) - 1] == '\\')
		{
			testpath[strlen(testpath) - 1] = 0;
		}
		else
		{
			// No \ eh? Better add one.
			strcat(ExpandedSaveDir, "\\");
		}

		struct _stat buf;
		// Check that the path is valid, else ignore path variable and just save in current dir.
		if (!_stat(testpath, &buf))
		{
			if (!(buf.st_mode & _S_IFDIR))
			{
				// Not a directory, or can't write.
				ExpandedSaveDir[0] = 0;
			}
		}
		else
		{
			// Failed to get info, assume path is invalid.
			ExpandedSaveDir[0] = 0;
		}
	}

	// Get save/load path
	sprintf(filePath, "%s%s.msf", ExpandedSaveDir, _FileName);

	if (buffer)
	{
		fh = fopen(filePath, "rb");
		if (fh)
		{
			// Read data into buffer and copy buffer into CCareer object.
			fread(buffer, sizeof(CCareer), 1, fh);		
			memcpy(_RefCareer, buffer, sizeof(CCareer));

			fclose(fh);

			return McErrNone;
		}
		return McErrFileDoesntExist; 
	
	}
	return McErrFileError;
}

// Saves a binary image of the CCareer object referenced 
// by _RefCareer. 
// _OverWriteIfExists specifies whether or not the 
// function should over write an existing file.
SINT PCSaveCareer(char* _FileName, CCareer* _RefCareer, bool _OverWriteIfExists)
{
	FILE*	fh			= NULL;
	SINT	written		= 0;
	SINT	CareerSize	= sizeof(CCareer);
	char	filePath[MAX_PATH], ExpandedSaveDir[MAX_PATH];
	char*	SaveDir;
	DWORD	TypeBuffer, DataLength;

	ExpandedSaveDir[0] = 0;
	if (!PCLoadFromRegistry(HKEY_CURRENT_USER, 
							MOHO_REGISTRY_PATH,
							MOHO_REG_SAVE_PATH_PATH,
							&TypeBuffer,
							&SaveDir,
							&DataLength)
		)
	{
		if ((TypeBuffer == REG_EXPAND_SZ) || 
			(TypeBuffer == REG_SZ))
		{
			if (!ExpandEnvironmentStrings(SaveDir, ExpandedSaveDir, MAX_PATH))
			{
				ExpandedSaveDir[0] = 0;
			}
			OutputDebugString(ExpandedSaveDir);
		}
		delete[] SaveDir;
		SaveDir = NULL;
	}

	if (strlen(ExpandedSaveDir) > 0)
	{
		char testpath[MAX_PATH];
		strcpy(testpath, ExpandedSaveDir);

		if (testpath[strlen(testpath) - 1] == '\\')
		{
			testpath[strlen(testpath) - 1] = 0;
		}
		else
		{
			// No \ eh? Better add one.
			strcat(ExpandedSaveDir, "\\");
		}

		struct _stat buf;
		// Check that the path is valid, else ignore path variable and just save in current dir.
		if (!_stat(testpath, &buf))
		{
			if (!(buf.st_mode & _S_IFDIR))
			{
				// Not a directory, or can't write.
				ExpandedSaveDir[0] = 0;
			}
		}
		else
		{
			// Failed to get info, assume path is invalid.
			ExpandedSaveDir[0] = 0;
		}
	}

	sprintf(filePath, "%s%s.msf", ExpandedSaveDir, _FileName);
	// Only check whether the file exist if we are NOT to 
	// over write an existing file.
	if (!_OverWriteIfExists)
	{
		// Check that the file doesn't already exist.
		fh = fopen(filePath, "rb");
		if (fh)
		{
			// Close handle, and change mode.
			fclose(fh);
			return McErrAlreadyExist;
		}
	}
	
	// Save file.
	fh = fopen(filePath, "wb");
	if (fh)
	{
		written = fwrite(_RefCareer, CareerSize, 1, fh);
		if (written != 1)
		{
			OutputDebugString("Didn't manage to write all data out.\n");
			return McErrFileError;
		}
		fclose(fh);

		PCRefreshFileList();
		
		// Saved successfully.
		return McErrNone;
	}
	return McErrFileError;
}

// Delete a saved game.
void PCDeleteCareer(char* _FileName)
{
	char	filePath[MAX_PATH], ExpandedSaveDir[MAX_PATH];
	char*	SaveDir;
	DWORD	TypeBuffer, DataLength;

	ExpandedSaveDir[0] = 0;
	if (!PCLoadFromRegistry(HKEY_CURRENT_USER, 
							MOHO_REGISTRY_PATH,
							MOHO_REG_SAVE_PATH_PATH,
							&TypeBuffer,
							&SaveDir,
							&DataLength)
		)
	{
		if ((TypeBuffer == REG_EXPAND_SZ) || 
			(TypeBuffer == REG_SZ))
		{
			if (!ExpandEnvironmentStrings(SaveDir, ExpandedSaveDir, MAX_PATH))
			{
				ExpandedSaveDir[0] = 0;
			}
			OutputDebugString(ExpandedSaveDir);
		}
			
		delete[] SaveDir;	
		SaveDir = NULL;
	}

	if (strlen(ExpandedSaveDir) > 0)
	{
		char testpath[MAX_PATH];
		strcpy(testpath, ExpandedSaveDir);

		if (testpath[strlen(testpath) - 1] == '\\')
		{
			testpath[strlen(testpath) - 1] = 0;
		}
		else
		{
			// No \ eh? Better add one.
			strcat(ExpandedSaveDir, "\\");
		}

		struct _stat buf;
		// Check that the path is valid, else ignore path variable and just save in current dir.
		if (!_stat(testpath, &buf))
		{
			if (!(buf.st_mode & _S_IFDIR))
			{
				// Not a directory, or can't write.
				ExpandedSaveDir[0] = 0;
			}
		}
		else
		{
			// Failed to get info, assume path is invalid.
			ExpandedSaveDir[0] = 0;
		}
	}

	// Update file list
	PCRefreshFileList();
	tagPCFileList *i = PCFileList;
	while (i)
	{
		if (!_stricmp(i->name, _FileName))
		{
			sprintf(filePath, "%s%s.msf", ExpandedSaveDir, _FileName);
			unlink(filePath);
		}
		i = i->next;
	}
	// Refresh list.
	PCRefreshFileList();
}

// Delete all saved game files.
void PCPurgeCareers()
{
	char	filePath[MAX_PATH], ExpandedSaveDir[MAX_PATH];
	char*	SaveDir;
	DWORD	TypeBuffer, DataLength;

	ExpandedSaveDir[0] = 0;
	if (!PCLoadFromRegistry(HKEY_CURRENT_USER, 
							MOHO_REGISTRY_PATH,
							MOHO_REG_SAVE_PATH_PATH,
							&TypeBuffer,
							&SaveDir,
							&DataLength)
		)
	{
		if ((TypeBuffer == REG_EXPAND_SZ) || 
			(TypeBuffer == REG_SZ))
		{
			if (!ExpandEnvironmentStrings(SaveDir, ExpandedSaveDir, MAX_PATH))
			{
				ExpandedSaveDir[0] = 0;
			}
			OutputDebugString(ExpandedSaveDir);
		}
		delete[] SaveDir;
		SaveDir = NULL;
	}

	if (strlen(ExpandedSaveDir) > 0)
	{
		char testpath[MAX_PATH];
		strcpy(testpath, ExpandedSaveDir);

		if (testpath[strlen(testpath) - 1] == '\\')
		{
			testpath[strlen(testpath) - 1] = 0;
		}
		else
		{
			// No \ eh? Better add one.
			strcat(ExpandedSaveDir, "\\");
		}

		struct _stat buf;
		// Check that the path is valid, else ignore path variable and just save in current dir.
		if (!_stat(testpath, &buf))
		{
			if (!(buf.st_mode & _S_IFDIR))
			{
				// Not a directory, or can't write.
				ExpandedSaveDir[0] = 0;
			}
		}
		else
		{
			// Failed to get info, assume path is invalid.
			ExpandedSaveDir[0] = 0;
		}
	}

	// Update file list
	PCRefreshFileList();
	tagPCFileList *i = PCFileList;
	while (i)
	{
		sprintf(filePath, "%s%s.msf", ExpandedSaveDir, i->name);
		unlink(filePath);
		i = i->next;
	}

	// Refresh list again.
	PCRefreshFileList();
}


// ----
// Validates that a key exists.
bool PCValidateRegistry(HKEY	_ExistingKey,	// Existing key or default root.
						char*	_SubKey,		// Sub key, relative to _ExistingKey. (Must not start with a '/' char!)
						char*	_ValueName,		// Name of value to retrieve.
						DWORD	_ValueType)
{
	LONG			ret;				// Values returned from function calls.
	HKEY			KeyResult;			// Our key reference.
	DWORD			BufferLength;		// Size of the buffer.
	DWORD			ValueType;
	
	// Open the spefified key.
	ret = RegOpenKeyEx(	_ExistingKey,	// Handle to open key.
						_SubKey,		// Address of name of subkey to open.
						0,				// Reserved.
						KEY_ALL_ACCESS,	// Security access mask.
						&KeyResult);	// Address of handle to open key.

 	if (SUCCEEDED(ret))
	{
		// Accertain the amount of space required...
		BufferLength = 0;
		ret = RegQueryValueEx(	KeyResult,						// handle to key to query
								_ValueName,						// address of name of value to query
								0,								// reserved
								&ValueType,						// address of buffer for value type
								NULL,							// address of data buffer
								(unsigned long*)&BufferLength);	// address of data buffer size
		
		if (	(SUCCEEDED(ret))
			&&	(ValueType == _ValueType)
			)
		{
			return true;
		}
	}
	return false;
}



// Registry load / save routines.
LONG PCLoadFromRegistry(HKEY	_ExistingKey,	// Existing key or default root.
						char*	_SubKey,		// Sub key, relative to _ExistingKey. (Must not start with a '/' char!)
						char*	_ValueName,		// Name of value to retrieve.
						DWORD*	_ValueType,		// A pointer to a DWORD filled-in by the function.
						char**	_Value,			// A pointer to buffer created within the function - you are responsible for the freeing of this resource.
						DWORD*	_DataLength)	// A pointer to a DWORD filled-in by the function that indicates the length of the data (in bytes) returned for _Value.
{
	LONG			ret;				// Values returned from function calls.
	HKEY			KeyResult;			// Our key reference.
	unsigned char*	Buffer;				// Buffer for data (the address of this buffer will be returned to the user).
	DWORD			BufferLength;		// Size of the buffer.
	
	// Open the spefified key.
	ret = RegOpenKeyEx(	_ExistingKey,	// Handle to open key.
						_SubKey,		// Address of name of subkey to open.
						0,				// Reserved.
						KEY_ALL_ACCESS,	// Security access mask.
						&KeyResult);	// Address of handle to open key.

 	if (SUCCEEDED(ret))
	{
		// Accertain the amount of space required...
		BufferLength = 0;
		ret = RegQueryValueEx(	KeyResult,						// handle to key to query
								_ValueName,						// address of name of value to query
								0,								// reserved
								_ValueType,						// address of buffer for value type
								NULL,							// address of data buffer
								(unsigned long*)&BufferLength);	// address of data buffer size
		
		if (SUCCEEDED(ret) && (BufferLength > 0))
		{
			// Allocate sufficient memory.
			Buffer = new unsigned char[BufferLength];
			if (Buffer)
			{
				ret = RegQueryValueEx(	KeyResult,						// handle to key to query
										_ValueName,						// address of name of value to query
										0,								// reserved
										_ValueType,						// address of buffer for value type
										Buffer,							// address of data buffer
										(unsigned long*)&BufferLength);	// address of data buffer size

				// Close key.
				RegCloseKey(KeyResult);	

				if (SUCCEEDED(ret))
				{
					// ValueType is already assigned.
					*_Value			= (char*)Buffer;
					*_DataLength	= BufferLength;	
					return ret;
				}
				else
				{
					**_Value			= NULL;
					*_DataLength	= 0;
					delete[]		Buffer;
					Buffer			= NULL;
					return ret;
				}

				// Will have always returned by now.
			}
		}
		else
		{
			RegCloseKey(KeyResult);	
			return ret;
		}
	}
	
	return ret;
}

LONG PCSaveToRegistry(	HKEY	_ExistingKey,	// Existing key or default root.
						char*	_SubKey,		// Sub key, relative to _ExistingKey. (Must not start with a '/' char!)
						char*	_ValueName,		// Name of the value to store.
						DWORD	_ValueType,		// Specifies the format for storing the data.
						char*	_Value,			// A pointer to the data to store.
						DWORD	_DataLength)	// Length of the _Value data, in bytes.
{
	LONG			ret;				// Values returned from function calls.
	HKEY			KeyResult;			// Our key reference.
	DWORD			KeyState;			// State of that key.
	
	// Create / Open key.
	ret = RegCreateKeyEx(	_ExistingKey,			// handle to an open key
							_SubKey,				// address of subkey name
							0,						// reserved
							"REG_SZ",				// address of class string
							REG_OPTION_NON_VOLATILE,// special options flag
							KEY_ALL_ACCESS,			// desired security access
							NULL,					// address of key security structure
							&KeyResult,				// address of buffer for opened handle
							&KeyState);				// address of disposition value buffer

	if (SUCCEEDED(ret))
	{
		ret = RegSetValueEx(KeyResult,						// handle to key to set value for
							_ValueName,						// name of the value to set
							0,								// reserved
							_ValueType,						// flag for value type
							(const unsigned char*)_Value,	// address of value data
							_DataLength);					// size of value data
		// Free handle.
		RegCloseKey(KeyResult);	
		return ret;
	}
	return ret;
}

// Saves data to the registry.
LONG PCRemoveRegistryValue(	HKEY	_ExistingKey,	// Existing key or default root.
							char*	_SubKey,		// Sub key, relative to _ExistingKey. (Must not start with a '/' char!)
							char*	_ValueName)		// Name of the value to remove
{
	LONG			ret;				// Values returned from function calls.
	HKEY			KeyResult;			// Our key reference.
		
	// Open the spefified key.
	ret = RegOpenKeyEx(	_ExistingKey,	// Handle to open key.
						_SubKey,		// Address of name of subkey to open.
						0,				// Reserved.
						KEY_ALL_ACCESS,	// Security access mask.
						&KeyResult);	// Address of handle to open key.

 	if (SUCCEEDED(ret))
	{
		ret = RegDeleteValue(KeyResult, 
							_ValueName);
	}

	// Close key.
	RegCloseKey(KeyResult);	
	return ret;
}

#endif	// TARGET == PC