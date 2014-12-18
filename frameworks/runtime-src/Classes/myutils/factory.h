//********************************************************************
//	created:	20:3:2012   14:26
//	filename: 	factory.h
//	author:		tiamo
//	purpose:	factory
//********************************************************************

#pragma once

namespace myutils
{
	//
	// factory
	//
	template<typename Key, typename Type>
	class Factory
	{
	public:
		//
		// creator
		//
		typedef std::function<Type* (Key)>                                  Creator;

	public:
		//
		// constructor
		//
		Factory()															{}

		//
		// destructor
		//
		virtual ~Factory()													{}

		//
		// create
		//
		Type* create(Key key)												{auto it = mCreatorList.find(key); if(it == mCreatorList.end()) return nullptr; return it->second(key);}

		//
		// register creator
		//
		void registerType(Key key, Creator&& creator)						{mCreatorList[key] = std::move(creator);}

	private:
		//
		// creator list
		//
		std::map<Key, Creator>												mCreatorList;
	};
}
