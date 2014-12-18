//********************************************************************
//	created:	26:1:2010   1:04
//	filename:	callback.h
//	author:		tiamo	
//	purpose:	callback
//********************************************************************

#pragma once

//
// copy from loki
//
namespace loki
{

	//
	// no lock
	//
	class NoLock
	{
	public:
		//
		// constructor
		//
		NoLock()															{}

		//
		// destructor
		//
		~NoLock()															{}

		//
		// initialize
		//
		void initialize()													{}

		//
		// lock
		//
		void lock()															{}

		//
		// unlock
		//
		void unlock()														{}
	};

	//
	// locker
	//
	template<typename Lock>
	class Locker
	{
	public:
		//
		// constructor
		//
		Locker(Lock& lock) : mLock(lock)									{mLock.lock();}

		//
		// destructor
		//
		~Locker()															{mLock.unlock();}

	private:
		//
		// disable copy constructor
		//
		Locker(Locker const& rhs);
		Locker(Locker&& rhs);
		Locker& operator=(Locker const& rhs);
		Locker& operator=(Locker&& rhs);

	private:
		//
		// mutex
		//
		Lock&																mLock;
	};
}
