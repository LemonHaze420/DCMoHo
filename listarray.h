// ListArray.h: interface for the CListArray class.
//
//////////////////////////////////////////////////////////////////////

#ifndef LISTARRAY_H
#define LISTARRAY_H


#if TARGET==PC
extern void check_list() ;
#endif



class GenericListArray
{
protected:
	GenericListArray(void **mem) : mMem(mem), mIteratorPointer(0), mNumItems(0) {} 
	void Add(void *item, SINT size) ;
	void Remove(void *item, SINT size) ;
	void Clear(SINT size) ;
	void* First(SINT size) const ;
	void* Next(SINT size) const ;

protected:

	SINT mNumItems; 
	void **mMem;
	SINT mIteratorPointer;
} ;

template <class T, SINT ASIZE>
class CListArray : public GenericListArray
{
public:
	CListArray() : GenericListArray((void**) mItems) { Clear(); } ;
	~CListArray() {}

	void Add(T *item) { GenericListArray::Add((void*)item, ASIZE) ; };
	void Remove( T *item) { GenericListArray::Remove((void*)item, ASIZE); };
	SINT Size() const { return mNumItems ; };
	void Clear() { GenericListArray::Clear(ASIZE) ; };
	T* At(SINT i) const { return i < ASIZE ? mItems[i] : NULL ; };
	T* First() const { return (T*) GenericListArray::First(ASIZE) ; };
	T* Next() const { return (T*) GenericListArray::Next(ASIZE) ; };

private:
	T* mItems[ASIZE] ;
};



// another template hack
// When u want a list of things that don't exist yet!! arse!


class GenericListArrayInstances
{
protected:
	GenericListArrayInstances(void* mem, BOOL* used) : mMem(mem), mUsedP(used), mIteratorPointer(0), mNumItems(0) {} 

	void Clear(SINT size) ;
	void Remove(void* item, SINT size, SINT structsize) ;
	SINT NextFree(SINT size) const ;
	void* First(SINT size, SINT structsize) const ;
	void* Next(SINT size, SINT structsize) const ;

protected:

	SINT mNumItems; 
	BOOL *mUsedP;
	void* mMem;
	SINT mIteratorPointer;
} ;

template <class T, SINT ASIZE>
class CListArrayInstances : public GenericListArrayInstances
{
public:
	CListArrayInstances() :  GenericListArrayInstances((void*)&mItems, mUsed) { Clear() ;}
	~CListArrayInstances() {}

	void Add(T item) { Set(NextFree(), item) ;  };
	void Set(SINT num, T item) { if(!mUsed[num]) mNumItems++; mItems[num] = item; mUsed[num] = TRUE ; };
	void Remove(T* item) { GenericListArrayInstances::Remove((void*) item, ASIZE, sizeof(T)) ; } 
	SINT NextFree()	const { return GenericListArrayInstances::NextFree(ASIZE) ; } 
	BOOL IsUsed(SINT num) { return mUsed[num] ; }

	SINT Size() { return mNumItems ; }
	void Clear() { GenericListArrayInstances::Clear(ASIZE) ; };

	T* First() const { return (T*) GenericListArrayInstances::First(ASIZE, sizeof(T)) ; };
	T* Next() const { return (T*) GenericListArrayInstances::Next(ASIZE, sizeof(T)) ; };


private:
	T mItems[ASIZE] ;
	BOOL mUsed[ASIZE] ;
};





#endif 