//*******************************************************************
//	created:	21:4:2011   22:36
//	filename: 	exceptionhelper.h
//	author:		tiamo
//	purpose:	exception helper
//*******************************************************************

#pragma once

namespace myutils
{
	//
	// exception
	//
	class Exception : public std::exception
	{
	public:
		//
		// constructor
		//
		Exception(char const* fileName, int fileLine, char const* functionName) throw ();

		//
		// constructor
		//
		Exception(char const* fileName, int fileLine, char const* functionName, wchar_t const* format, ...) throw ();

		//
		// destructor
		//
		virtual ~Exception() throw()										{}

		//
		// get description
		//
		virtual char const* what() const throw() override					{return mFullMessage;}

		//
		// get message
		//
		wchar_t const* getMessage() const throw()							{return mMessage;}

		//
		// get unicode message
		//
		static UniString getMessage(std::exception& e)						{return utf8ToWide(e.what());}

	protected:
		//
		// build full message
		//
		void buildFullMessage() throw();

	protected:
		//
		// file name
		//
		char const*															mFileName;

		//
		// file line
		//
		int																	mFileLine;

		//
		// function name
		//
		char const*															mFunctionName;

		//
		// message
		//
		wchar_t																mMessage[4096];

		//
		// full message
		//
		char																mFullMessage[4096];
	};

#define ThrowException(...)												throw myutils::Exception(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
}
