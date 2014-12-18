//*******************************************************************
//	created:	25:4:2011   19:30
//	filename: 	smartpointer.h
//	author:		tiamo
//	purpose:	smart pointer
//*******************************************************************

#pragma once


namespace myutils
{
    template<typename T>
	class SmartPointer;
    
	//
	// ref counted pointer
	//
	template<typename Lock, typename T>
	class RefCountedObject
	{
        friend class SmartPointer<T>;
	public:
		//
		// constructor
		//
		RefCountedObject()													{mRefCount = 0; mLock.initialize();}

		//
		// destructor
		//
		virtual ~RefCountedObject()											{}

		//
		// add ref
		//
		long addRef()														{loki::Locker<Lock> locker(mLock); mRefCount += 1; return mRefCount;}

		//
		// release ref
		//
		long releaseRef()														{long ret;{loki::Locker<Lock> locker(mLock); mRefCount -= 1; ret = mRefCount;} if(!ret) delete this; return ret;}

		//
		// get ref count
		//
		long getRefCount() const											{return mRefCount;}

	private:
		//
		// disable copy constructor, assignment operator
		//
		RefCountedObject(RefCountedObject const& rhs);
		RefCountedObject(RefCountedObject&& rhs);
		RefCountedObject& operator=(RefCountedObject const& rhs);
		RefCountedObject& operator=(RefCountedObject&& rhs);

	private:
		//
		// ref count
		//
		long																mRefCount;

		//
		// lock
		//
		Lock																mLock;
	};

	//
	// smart pointer
	//
	template<typename T>
	class SmartPointer
	{
	public:
		//
		// constructor
		//
		SmartPointer(T* theObject = nullptr)								{mObject = theObject; if(mObject) mObject->addRef();}

		//
		// copy constructor
		//
		SmartPointer(SmartPointer const& theOther)							{mObject = theOther.mObject; if(mObject) mObject->addRef();}

		//
		// copy constructor
		//
		SmartPointer(SmartPointer&& theOther)								{mObject = theOther.mObject; theOther.mObject = nullptr;}

		//
		// assignment operator
		//
		SmartPointer& operator=(SmartPointer const& theOther)				{T* p = theOther.mObject; if(p) p->addRef(); if(mObject) mObject->releaseRef(); mObject = p; return *this;}

		//
		// assignment operator
		//
		SmartPointer& operator=(SmartPointer&& theOther)					{if(this != &theOther){if(mObject) mObject->releaseRef(); mObject = theOther.mObject; theOther.mObject = nullptr;} return *this;}

		//
		// destructor
		//
		~SmartPointer()														{if(mObject) mObject->releaseRef();}

		//
		// t*
		//
		operator T* ()														{return mObject;}

		//
		// t const*
		//
		operator T const* () const											{return mObject;}

		//
		// get
		//
		T* get() const														{return mObject;}

		//
		// release
		//
		T* release()														{T* p = mObject; mObject = nullptr; return p;}

		//
		// operator *
		//
		T& operator*() const												{return *mObject;}

		//
		// operator ->
		//
		T* operator->() const												{return mObject;}

		//
		// operator !
		//
		bool operator!() const												{return !mObject;}

		//
		// operator bool
		//
		//operator bool () const												{return mObject != nullptr;}

	private:
		//
		// the object
		//
		T*																	mObject;
	};

	//
	// operator ==
	//
	template<typename T, typename U>
	inline bool operator==(SmartPointer<T> const& a, SmartPointer<U> const& b)
	{
		return a.get() == b.get();
	}

	//
	// operator ==
	//
	template<typename T, typename U>
	inline bool operator==(SmartPointer<T> const& a, U* b)
	{
		return a.get() == b;
	}

	//
	// operator ==
	//
	template<typename T, typename U>
	inline bool operator==(T* a, SmartPointer<U> const& b)
	{
		return a == b.get();
	}

	//
	// operator !=
	//
	template<typename T, typename U>
	inline bool operator!=(SmartPointer<T> const& a, SmartPointer<U> const& b)
	{
		return a.get() != b.get();
	}

	//
	// operator !=
	//
	template<typename T, typename U>
	inline bool operator!=(SmartPointer<T> const& a, U* b)
	{
		return a.get() != b;
	}

	//
	// operator !=
	//
	template<typename T, typename U>
	inline bool operator!=(T* a, SmartPointer<U> const& b)
	{
		return a != b.get();
	}
}
