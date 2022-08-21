// ListArray.cpp: implementation of the CListArray class.
//
//////////////////////////////////////////////////////////////////////
#include "common.h"
#include "ListArray.h"
#include "globals.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//******************************************************************************************
void GenericListArray::Add(void* item, SINT size)
{
	for (SINT c0=0;c0<size;c0++)
	{
		if (mMem[c0]==NULL)
		{
			mMem[c0]= item ;
			mNumItems++ ;
			return ;
		}
	}

	char temp[100] ;
	sprintf(temp,"Warning: run out of memory in List Array size is %d\n", size) ;
	CONSOLE.AddString(temp) ;
	
	// RUN OUT OF MEM !!!
}

//******************************************************************************************
void GenericListArray::Remove(void* item, SINT size) 
{
	for (SINT c0=0;c0<size;c0++)
	{
		if (mMem[c0]==item)
		{
			mMem[c0]= NULL ;
			mNumItems-- ;
			return ;
		}
	}


	// COULD NOT FIND ITEM
}

//******************************************************************************************
void GenericListArray::Clear(SINT size) 
{
	for (SINT c0=0;c0<size;c0++) mMem[c0] = NULL ;
	mNumItems = 0 ;
}


//******************************************************************************************
void* GenericListArray::First(SINT size) const
{
	for (SINT c0=0;c0<size;c0++)
	{
		if (mMem[c0] != NULL) 
		{
			//naughty naughty
			((GenericListArray*)this)->mIteratorPointer =c0 ;
			return mMem[c0] ;
		}
	}
	return NULL ;
}


//******************************************************************************************
void* GenericListArray::Next(SINT size) const
{
	for (SINT c0=mIteratorPointer+1;c0<size;c0++)
	{
		if (mMem[c0] != NULL) 
		{
			//naughty naughty
			((GenericListArray*)this)->mIteratorPointer = c0 ;
			return mMem[c0] ;
		}
	}
	return NULL ;
}






//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************



//******************************************************************************************
void*	GenericListArrayInstances::First(SINT size, SINT structsize) const 
{	
	for (SINT i=0;i<size;i++) 
	{ 
		if (mUsedP[i] )
		{ 
			((GenericListArrayInstances*)this)->mIteratorPointer= i ;
			char* mem = (char*) mMem ;

			return &mem[i*structsize] ;
		} 
	}
	return NULL ; 
}




//******************************************************************************************
void*	 GenericListArrayInstances::Next(SINT size, SINT structsize)	const
{ 
	for (SINT i=mIteratorPointer+1;i<size;i++) 
	{
		if (mUsedP[i] ) 
		{
			((GenericListArrayInstances*)this)->mIteratorPointer=i ; 
			char* mem = (char*) mMem ;

			return &mem[i*structsize] ;
		}
	}
	return NULL ; 
}


//******************************************************************************************
SINT	GenericListArrayInstances::NextFree(SINT size) const 
{	
	for (SINT i=0;i<size;i++) 
	{ 
		if (!mUsedP[i] ) return i ; 
	}

	CONSOLE.AddString("Run out of mem in instances");
	return 0 ; 
}; 



//******************************************************************************************
void	GenericListArrayInstances::Clear(SINT size)
{	
	for (SINT i=0;i<size;i++) mUsedP[i] = FALSE ;
	mNumItems = 0 ;

}; 


//******************************************************************************************
void	GenericListArrayInstances::Remove(void* item, SINT size, SINT structsize) 
{
	for (SINT i=0;i<size;i++) 
	{ 
		char* mem = (char*) mMem ;
		if ( &mem[i*structsize] == item)
		{
			mUsedP[i] = FALSE ;
			mNumItems-- ;
			return ;
		}
	}

#if TARGET == PC

	char temp[50] ;
	sprintf(temp, "  Instance list could not find item to delete %08x", item) ;
	CONSOLE.AddString(temp);
#endif

}





//******************************************************************************************

// useful for testing the lists


#if TARGET==PC

class bob
{
public:
	bob() {} ;
	bob(SINT f, SINT b) : fred(f), bill(b) {} ;
private:
	SINT fred ;
	SINT bill ;
} ;


void check_list()
{
	CONSOLE.AddString("Doing list check!") ;

	CListArrayInstances<bob, 10> my_instances ;

	my_instances.Add(bob(1,2)) ;

	if (my_instances.Size() != 1 ) CONSOLE.AddString("Error test 1") ;

		
	my_instances.Add(bob(3,2));

	if (my_instances.Size() != 2 ) CONSOLE.AddString("Error test 2") ;


	my_instances.Add(bob(4,5));

	if (my_instances.Size() != 3 ) CONSOLE.AddString("Error test 3") ;


	my_instances.Remove(my_instances.First());

	if (my_instances.Size() != 2 ) CONSOLE.AddString("Error test 4") ;


	my_instances.Add(bob(1,2)) ;

	if (my_instances.Size() != 3 ) CONSOLE.AddString("Error test 5") ;

	my_instances.First() ;
	my_instances.Remove(my_instances.Next());

	if (my_instances.Size() != 2 ) CONSOLE.AddString("Error test 6") ;


}


#endif







