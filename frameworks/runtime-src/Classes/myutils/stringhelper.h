//*******************************************************************
//	created:	21:4:2011   19:52
//	filename: 	stringhelper.h
//	author:		tiamo
//	purpose:	string helper
//*******************************************************************

#pragma once

//
// string helper
//
namespace myutils
{
	//
	// utf8 -> wchar_t
	//
	UniString utf8ToWide(char const* inputString, uint32_t inputLength = -1);

	//
	// utf8 -> wchar_t
	//
	inline UniString utf8ToWide(Utf8String const& inputString)				{return utf8ToWide(inputString.c_str(), static_cast<uint32_t>(inputString.length()));}

	//
	// wchar_t -> utf8
	//
	Utf8String wideToUtf8(wchar_t const* inputString, uint32_t inputLength = -1);

	//
	// wchar_t -> utf8
	//
	inline Utf8String wideToUtf8(UniString const& inputString)				{return wideToUtf8(inputString.c_str(), static_cast<uint32_t>(inputString.length()));}

	//
	// hex string -> binary
	//
	void hexToBinary(UniString const& inputString, uint8_t* binaryBuffer, uint32_t bufferLength);

	//
	// binary -> hex string
	//
	UniString binaryToHex(uint8_t const* binaryBuffer, uint32_t bufferLength);

	Utf8String binaryToHexUtf8(uint8_t const* binaryBuffer, uint32_t bufferLength);

	//
	// json encoding
	//
	UniString encodeJSON(UniString const& inputString);

	//
	// escape xml
	//
	UniString escapeXml(UniString const& xmlString);

	//
	// append path
	//
	UniString appendPathComponent(UniString const& path, UniString const& fileName, wchar_t const* extension = nullptr);

	//
	// split
	//
	std::vector<UniString> split(UniString const& inputString, wchar_t sep);

	//
	// split csv line
	//
	std::vector<Utf8String> splitCSVLine(char const* lineBuffer, uint32_t lineLength = 0xffffffff, uint32_t* usedLength = nullptr);

	//
	// trim
	//
	UniString trim(UniString const& inputString);
};
