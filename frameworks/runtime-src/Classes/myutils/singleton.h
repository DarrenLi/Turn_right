//********************************************************************
//	created:	19:2:2012   15:39
//	filename: 	singleton.h
//	author:		tiamo
//	purpose:	singleton
//********************************************************************

#pragma once

namespace myutils
{
	template<typename T>
	class Singleton
	{
	public:
		//
		// constructor
		//
		Singleton()															{mSingleton = static_cast<T*>(this);}

		//
		// destructor
		//
		virtual ~Singleton()												{mSingleton = nullptr;}

		//
		// get singleton
		//
		static T* getSingletonPtr()											{if(!mSingleton) new T;
            return mSingleton;}

		//
		// get singleton
		//
		static T& getSingleton()											{if(!mSingleton) new T;
            return *mSingleton;}
        
        static void release()                                               {if(mSingleton) delete mSingleton;}
	private:
		//
		// disable copy constructor, assignment operator
		//
		Singleton(Singleton const& rhs);
		Singleton(Singleton&& rhs);
		Singleton& operator=(Singleton const& rhs);
		Singleton& operator=(Singleton&& rhs);

	private:
		//
		// singleton
		//
		static T*															mSingleton;
	};
}
