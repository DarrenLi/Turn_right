//*******************************************************************
//	created:	21:4:2011   22:40
//	filename: 	exceptionhelper.cpp
//	author:		tiamo
//	purpose:	exception helper
//*******************************************************************
#include "stdafx.h"
#include "exceptionhelper.h"

namespace myutils
{
	//
	// constructor
	//
	Exception::Exception(char const* fileName, int fileLine, char const* functionName) throw ()
		: mFileName(fileName), mFileLine(fileLine), mFunctionName(functionName)
	{
		mMessage[0]															= 0;
		mFullMessage[0]														= 0;
	}

	//
	// constructor
	//
	Exception::Exception(char const* fileName, int fileLine, char const* functionName, wchar_t const* format, ...) throw()
		: mFileName(fileName), mFileLine(fileLine), mFunctionName(functionName)
	{
		va_list argList;
		va_start(argList, format);
		platform::vsnwprintf(mMessage, ARRAYSIZE(mMessage), format, argList);
		va_end(argList);

		buildFullMessage();
	}

	//
	// build full message
	//
	void Exception::buildFullMessage() throw()
	{
		//
		// BUG BUG BUG, on windows platform fileName is actually in native code page instead of utf8
		//
		wchar_t fullMessage[4096];
		platform::snwprintf(fullMessage, ARRAYSIZE(fullMessage), L"[%ls] at %ls(%u): %ls", utf8ToWide(mFunctionName).c_str(), utf8ToWide(mFileName).c_str(), mFileLine, mMessage);
		try
		{
			Utf8String tempMessage											= wideToUtf8(fullMessage);
			uint32_t length													= static_cast<uint32_t>(tempMessage.length());
			if(length > ARRAYSIZE(mFullMessage) - 1)
				length														= ARRAYSIZE(mFullMessage) - 1;
			memcpy(mFullMessage, tempMessage.c_str(), length);
			mFullMessage[length]											= 0;
		}
		catch(...)
		{
			mFullMessage[0]													= 0;
		}
	}
}
