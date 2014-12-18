//*******************************************************************
//	created:	21:4:2011   22:27
//	filename: 	platformhelper.h
//	author:		tiamo
//	purpose:	platform helper
//*******************************************************************

#pragma once

//
// platform helper
//
namespace platform
{
	//
	// get last error
	//
	uint32_t getLastErrorCode();

	//
	// get local time
	//
	tm getLocalTime(time_t theTime);

	//
	// setup timezone
	//
	void setupTimeZone();

	//
	// get timezone
	//
	int32_t getTimeZone();

	//
	// set timezone
	//
	void setTimeZone(int32_t tz);

	//
	// string -> time
	//
	uint32_t stringToTime(UniString const& stringTime);

	//
	// time -> string
	//
	UniString timeToString(uint32_t unixTime);

	//
	// vsnprintf
	//
	int vsnwprintf(wchar_t* theBuffer, size_t bufferSize, wchar_t const* format, va_list argList);

	//
	// snprintf
	//
	int snwprintf(wchar_t* theBuffer, size_t bufferSize, wchar_t const* format, ...);

	//
	// wcscasecmp
	//
	int wcsicmp(wchar_t const* s1, wchar_t const* s2);

	//
	// wcsncasecmp
	//
	int wcsnicmp(wchar_t const* s1, wchar_t const* s2, size_t maxCount);

	//
	// vsnprintf
	//
	int vsnprintf(char* theBuffer, size_t bufferSize, char const* format, va_list argList);

	//
	// snprintf
	//
	int snprintf(char* theBuffer, size_t bufferSize, char const* format, ...);

	//
	// atoi64
	//
	int64_t atoi64(char const* s);

	//
	// atou64
	//
	uint64_t atou64(char const* s);

	//
	// wcstoi64
	//
	int64_t wcstoi64(wchar_t const* ptr, wchar_t** end, int base);

	//
	// wcstou64
	//
	uint64_t wcstoui64(wchar_t const* ptr, wchar_t** end, int base);

	//
	// test file exist
	//
	bool fileExist(UniString const& fileName);
    
    template<typename T>
    T move(T val)
    {
        T ret = val;
        return ret;
    }
};
