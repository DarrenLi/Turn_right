//*******************************************************************
//	created:	21:4:2011   22:32
//	filename: 	platformhelper.cpp
//	author:		tiamo
//	purpose:	platform helper
//*******************************************************************
#include "stdafx.h"
#include "platformhelper.h"
#include <cwchar>

namespace platform
{
	//
	// get last error
	//
	uint32_t getLastErrorCode()
	{
	#ifdef _WIN32
		return GetLastError();
	#else
		return errno;
	#endif
	}

	//
	// get local time
	//
	tm getLocalTime(time_t theTime)
	{
		tm theTm;
	#ifdef _WIN32
		localtime_s(&theTm, &theTime);
	#else
		theTm																= *localtime(&theTime);
	#endif
		return theTm;
	}

	//
	// setup timezone
	//
	void setupTimeZone()
	{
	#ifdef _WIN32
		getLocalTime(0);
	#else
		tzset();
	#endif
	}

	//
	// get timezone
	//
	int32_t getTimeZone()
	{
	#ifdef _WIN32
		long tc;
		_get_timezone(&tc);
		return static_cast<int32_t>(tc);
	#else
		return static_cast<int32_t>(timezone);
	#endif
	}

	//
	// set timezone
	//
	void setTimeZone(int32_t tz)
	{
#ifdef _WIN32
		_timezone = tz;
#else
		timezone = tz;
#endif
	}

	//
	// string -> time
	//
	uint32_t stringToTime(UniString const& stringTime)
	{
		tm theTm															= {0};
	#ifdef _WIN32
		swscanf_s(stringTime.c_str(), L"%04d-%04d-%04d %02d:%02d:%02d", &theTm.tm_year, &theTm.tm_mon, &theTm.tm_mday, &theTm.tm_hour, &theTm.tm_min, &theTm.tm_sec);
	#else
		swscanf(stringTime.c_str(), L"%04d-%04d-%04d %02d:%02d:%02d", &theTm.tm_year, &theTm.tm_mon, &theTm.tm_mday, &theTm.tm_hour, &theTm.tm_min, &theTm.tm_sec);
	#endif
		theTm.tm_year														-= 1900;
		theTm.tm_mon														-= 1;
		return static_cast<uint32_t>(mktime(&theTm));
	}

	//
	// time -> string
	//
	UniString timeToString(uint32_t unixTime)
	{
		tm localTime														= getLocalTime(unixTime);
		wchar_t timeString[64];
		snwprintf(timeString, ARRAYSIZE(timeString), L"%04d-%02d-%02d %02d:%02d:%02d", localTime.tm_year + 1900, localTime.tm_mon + 1, localTime.tm_mday, localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
		return timeString;
	}

	//
	// vsnprintf
	//
	int vsnwprintf(wchar_t* theBuffer, size_t bufferSize, wchar_t const* format, va_list argList)
	{
	#ifdef _WIN32
		extern int mvsnprintf(wchar_t *buffer, size_t maxlength, const wchar_t *format, va_list ap_save);
		return mvsnprintf(theBuffer, bufferSize, format, argList);
	#else
		return vswprintf(theBuffer, bufferSize, format, argList);
	#endif
	}

	//
	// snprintf
	//
	int snwprintf(wchar_t* theBuffer, size_t bufferSize, wchar_t const* format, ...)
	{
		va_list argList;
		va_start(argList, format);
		int length															= vsnwprintf(theBuffer, bufferSize, format, argList);
		va_end(argList);

		return length;
	}

	//
	// wcsncasecmp
	//
	int wcsnicmp(wchar_t const* s1, wchar_t const* s2, size_t maxCount)
	{
	#ifdef _WIN32
		return _wcsnicmp(s1, s2, maxCount);
	#else
		#ifdef ANDROID
			return wcsncmp(s1, s2, maxCount);
		#else
			return wcsncasecmp(s1, s2, maxCount);
		#endif
	#endif
	}

	//
	// wcscasecmp
	//
	int wcsicmp(wchar_t const* s1, wchar_t const* s2)
	{
	#ifdef _WIN32
		return _wcsicmp(s1, s2);
	#else
		#ifdef ANDROID
		return wcscmp(s1, s2);
		#else
		return wcscasecmp(s1, s2);
		#endif
	#endif
	}

	//
	// vsnprintf
	//
	int vsnprintf(char* theBuffer, size_t bufferSize, char const* format, va_list argList)
	{
	#ifdef _WIN32
		return _vsnprintf_s(theBuffer, bufferSize, _TRUNCATE, format, argList);
	#else
		return ::vsnprintf(theBuffer, bufferSize, format, argList);
	#endif
	}

	//
	// snprintf
	//
	int snprintf(char* theBuffer, size_t bufferSize, char const* format, ...)
	{
		va_list argList;
		va_start(argList, format);
		int length															= vsnprintf(theBuffer, bufferSize, format, argList);
		va_end(argList);

		return length;
	}

	//
	// atoi64
	//
	int64_t atoi64(char const* s)
	{
	#ifdef _WIN32
		return _atoi64(s);
	#else
		return static_cast<int64_t>(atoll(s));
	#endif
	}

	//
	// atou64
	//
	uint64_t atou64(char const* s)
	{
	#ifdef _WIN32
		return _strtoui64(s, nullptr, 0);
	#else
		return strtoul(s, nullptr, 0);
	#endif
	}

	//
	// wcstoi64
	//
	int64_t wcstoi64(wchar_t const* ptr, wchar_t** end, int base)
	{
	#ifdef _WIN32
		return _wcstoi64(ptr, end, base);
	#else
		return wcstol(ptr, end, base);
	#endif
	}

	//
	// wcstoui64
	//
	uint64_t wcstoui64(wchar_t const* ptr, wchar_t** end, int base)
	{
	#ifdef _WIN32
		return _wcstoui64(ptr, end, base);
	#else
		return wcstoul(ptr, end, base);
	#endif
	}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
	//
	// test file exist
	//
	bool fileExist(UniString const& fileName)
	{
	#ifdef _WIN32
		struct _stat fileStat;
		return !_wstat(fileName.c_str(), &fileStat);
	#else
		 struct stat fileStat;
		 return !stat(myutils::wideToUtf8(fileName).c_str(), &fileStat);
	#endif
	}
}
