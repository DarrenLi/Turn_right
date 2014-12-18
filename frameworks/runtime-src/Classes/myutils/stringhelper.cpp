//*******************************************************************
//	created:	28:4:2011   16:02
//	filename: 	stringhelper.cpp
//	author:		tiamo
//	purpose:	string helper
//*******************************************************************
#include "stdafx.h"
#include "stringhelper.h"

namespace myutils
{
	namespace
	{
		//
		// utf8 -> utf32
		//
		uint32_t convertUtf8ToUtf32(uint8_t const*& utf8String, uint32_t& utf8Length)
		{
			uint8_t utf8Char1												= *utf8String;
			utf8String														+= 1;
			utf8Length														-= 1;
            
			//
			// 0zzzzzzz(00-7F)
			//
			if(utf8Char1 < 0x80)
				return utf8Char1;
            
			//
			// 110yyyyy(C0-DF) 10zzzzzz
			//
			if(utf8Char1 < 0xe0)
			{
				if(utf8Length < sizeof(uint8_t))
					return 0x80000000;
                
				uint32_t utf8Char2											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
				return (static_cast<uint32_t>(utf8Char1 & 0x0f) << 6) | utf8Char2;
			}
            
			//
			// 1110xxxx(E0-EF) 10yyyyyy 10zzzzzz
			//
			if(utf8Char1 < 0xf0)
			{
				if(utf8Length < sizeof(uint8_t) + sizeof(uint8_t))
					return 0x80000000;
                
				uint32_t utf8Char2											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				uint32_t utf8Char3											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				return (static_cast<uint32_t>(utf8Char1 & 0x0f) << 12) | (utf8Char2 << 6) | utf8Char3;
			}
            
			//
			// 11110www(F0-F7) 10xxxxxx 10yyyyyy 10zzzzzz
			//
			if(utf8Char1 < 0xf8)
			{
				if(utf8Length < sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t))
					return 0x80000000;
                
				uint32_t utf8Char2											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				uint32_t utf8Char3											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				uint32_t utf8Char4											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				return (static_cast<uint32_t>(utf8Char1 & 0x07) << 18) | (utf8Char2 << 12) | (utf8Char3 << 6) | utf8Char4;
			}
            
			//
			// 111110vv(F8-FB) 10wwwwww 10xxxxxx 10yyyyyy 10zzzzzz
			//
			if(utf8Char1 < 0xfc)
			{
				if(utf8Length < sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t))
					return 0x80000000;
                
				uint32_t utf8Char2											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				uint32_t utf8Char3											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				uint32_t utf8Char4											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				uint32_t utf8Char5											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				return (static_cast<uint32_t>(utf8Char1 & 0x03) << 24) | (utf8Char2 << 18) | (utf8Char3 << 12) | (utf8Char4 << 6) | utf8Char5;
			}
            
			//
			// 1111110u(FC-FD) 10vvvvvv 10wwwwwww 10xxxxxx 10yyyyyy 10zzzzzz
			//
			if(utf8Char1 < 0xfe)
			{
				if(utf8Length < sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t))
					return 0x80000000;
                
				uint32_t utf8Char2											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				uint32_t utf8Char3											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				uint32_t utf8Char4											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				uint32_t utf8Char5											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				uint32_t utf8Char6											= static_cast<uint32_t>(*utf8String & 0x3f);
				utf8String													+= 1;
				utf8Length													-= 1;
                
				return (static_cast<uint32_t>(utf8Char1 & 0x01) << 30) | (utf8Char2 << 24) | (utf8Char3 << 18) | (utf8Char4 << 12) | (utf8Char5 << 6) | utf8Char6;
			}
            
			return 0x80000000;
		}
        
		//
		// utf32 -> utf8
		//
		bool convertUtf32ToUtf8(uint32_t inputChar, Utf8String& retData)
		{
			//
			// 0zzzzzzz(00-7F)
			//
			if(inputChar < 0x0080)
			{
				retData.append(1, static_cast<char>(static_cast<uint8_t>(inputChar)));
				return true;
			}
            
			//
			// 110yyyyy(C0-DF) 10zzzzzz
			//
			if(inputChar < 0x0800)
			{
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >>  6) & 0x0f) | 0xc0)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >>  0) & 0x3f) | 0x80)));
				return true;
			}
            
			//
			// 1110xxxx(E0-EF) 10yyyyyy 10zzzzzz
			//
			if(inputChar < 0x10000)
			{
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >> 12) & 0x0f) | 0xe0)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >>  6) & 0x3f) | 0x80)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >>  0) & 0x3f) | 0x80)));
				return true;
			}
            
			//
			// 11110www(F0-F7) 10xxxxxx 10yyyyyy 10zzzzzz
			//
			if(inputChar < 0x200000)
			{
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >> 18) & 0x07) | 0xf0)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >> 12) & 0x3f) | 0x80)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >>  6) & 0x3f) | 0x80)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >>  0) & 0x3f) | 0x80)));
				return true;
			}
            
			//
			// 111110vv(F8-FB) 10wwwwww 10xxxxxx 10yyyyyy 10zzzzzz
			//
			if(inputChar < 0x4000000)
			{
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >> 24) & 0x03) | 0xf8)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >> 18) & 0x3f) | 0x80)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >> 12) & 0x3f) | 0x80)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >>  6) & 0x3f) | 0x80)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >>  0) & 0x3f) | 0x80)));
				return true;
			}
            
			//
			// 1111110u(FC-FD) 10vvvvvv 10wwwwwww 10xxxxxx 10yyyyyy 10zzzzzz
			//
			if(inputChar < 0x80000000)
			{
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >> 30) & 0x01) | 0xfc)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >> 24) & 0x3f) | 0x80)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >> 18) & 0x3f) | 0x80)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >> 12) & 0x3f) | 0x80)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >>  6) & 0x3f) | 0x80)));
				retData.append(1, static_cast<char>(static_cast<uint8_t>(((inputChar >>  0) & 0x3f) | 0x80)));
			}
            
			return false;
		}
	}
    
	//
	// utf8 -> wchat_t(utf32)
	//
	bool convertChar(uint8_t const*& utf8String, uint32_t& utf8Length, UniString& retValue)
	{
		uint32_t code32														= convertUtf8ToUtf32(utf8String, utf8Length);
		if(code32 > 0x7fffffff)
			return false;
        
        if(sizeof(wchar_t) == sizeof(uint32_t))
		{
            retValue.append(1, static_cast<wchar_t>(code32));
            return true;
        }
        else
        {
            if(code32 < 0x10000)
            {
                retValue.append(1, static_cast<wchar_t>(code32));
                return true;
            }
            
            if(code32 < 0x10ffff)
            {
                retValue.append(1, static_cast<wchar_t>((code32 >> 10) + 0xd7c0));
                retValue.append(1, static_cast<wchar_t>((code32 & 0x03ff) + 0xdc00));
                return true;
            }
            
            return false;
        }
	}
    
	//
	// wchat_t(utf32) -> utf8
	//
	bool convertChar(wchar_t const*& uniString, uint32_t& uniLength, Utf8String& retValue)
	{
		if(sizeof(wchar_t) == sizeof(uint32_t))
		{
            if(!convertUtf32ToUtf8(*uniString, retValue))
                return false;
            
            uniString															+= 1;
            uniLength															-= 1;
            return true;
            
        }
        else
        {
            uint32_t code1														= *uniString;
            uniString															+= 1;
            uniLength															-= 1;
            
            if(code1 >= 0xd800 && code1 <= 0xdbff)
            {
                if(!uniLength)
                    return false;
                
                uint32_t code2													= *uniString;
                uniString														+= 1;
                uniLength														-= 1;
                
                if(code2 >= 0xdc00 && code2 <= 0xdfff)
                    code1														= (code1 << 10) + code2 - 0x35fdc00;
                else
                    return false;
            }
            
            return convertUtf32ToUtf8(code1, retValue);
        }
    }
    
	//
	// utf8 -> wchar_t
	//
	UniString utf8ToWide(char const* inputString, uint32_t inputLength)
	{
		if(inputLength == 0xffffffff)
			inputLength														= static_cast<uint32_t>(strlen(inputString));
        
		UniString retValue;
		uint8_t const* utf8String											= static_cast<uint8_t const*>(static_cast<void const*>(inputString));
		while(inputLength)
		{
			if(!convertChar(utf8String, inputLength, retValue))
				return L"";
		}
        
		return retValue;
	}
    
	//
	// wchar_t -> utf8
	//
	Utf8String wideToUtf8(wchar_t const* inputString, uint32_t inputLength)
	{
		if(inputLength == 0xffffffff)
			inputLength														= static_cast<uint32_t>(wcslen(inputString));
        
		Utf8String retValue;
		while(inputLength)
		{
			if(!convertChar(inputString, inputLength, retValue))
				return "";
		}
        
		return retValue;
	}

	//
	// hex string -> binary
	//
	void hexToBinary(UniString const& inputString, uint8_t* binaryBuffer, uint32_t bufferLength)
	{
		if(bufferLength < inputString.length() / 2 || (inputString.length() % 2))
			ThrowException(L"invalid buffer length");

	#define CH_TO_INT(c)													static_cast<uint8_t>(((c)>=L'0' && (c)<=L'9') ? ((c)-L'0') : (((c)>=L'a' && (c)<=L'f') ? ((c)-L'a'+10) : (((c)>=L'A' && (c)<=L'F') ? ((c)-L'A'+10) : 0)))
		for(size_t i = 0; i < inputString.length(); i += 2)
		{
			uint8_t ch0														= CH_TO_INT(inputString[i]);
			uint8_t ch1														= CH_TO_INT(inputString[i + 1]);
			binaryBuffer[i / 2]												= static_cast<uint8_t>((ch0 << 4) + ch1);
		}
	#undef CH_TO_INT
	}

	//
	// binary -> hex string
	//
	UniString binaryToHex(uint8_t const* binaryBuffer, uint32_t bufferLength)
	{
		UniString retValue;
		retValue.reserve(bufferLength * 2);

		wchar_t temp[3]														= {0};
		for(uint32_t i = 0; i < bufferLength; i ++)
		{
			platform::snwprintf(temp, ARRAYSIZE(temp), L"%02x", binaryBuffer[i]);
			retValue.append(temp);
		}

		return retValue;
	}

	Utf8String binaryToHexUtf8 (uint8_t const* binaryBuffer, uint32_t bufferLength)
	{
		Utf8String retValue;
		retValue.reserve(bufferLength * 2);

		char temp[3] = { 0 };
		for (uint32_t i = 0; i < bufferLength; i++)
		{
			snprintf(temp, ARRAYSIZE(temp), "%02x", binaryBuffer[i]);
			retValue.append(temp);
		}
		return retValue;
	}

	//
	// json encoding
	//
	UniString encodeJSON(UniString const& inputString)
	{
		UniString retValue;
		for(size_t i = 0; i < inputString.length(); ++ i)
		{
			wchar_t ch														= inputString[i];
			if(ch == L'"' || ch == L'\\' || ch == L'/')
			{
				retValue													+= L'\\';
				retValue													+= ch;
			}
			else if(ch == L'\b')
			{
				retValue													+= L"\\b";
			}
			else if(ch == L'\f')
			{
				retValue													+= L"\\f";
			}
			else if(ch == L'\n')
			{
				retValue													+= L"\\n";
			}
			else if(ch == L'\r')
			{
				retValue													+= L"\\r";
			}
			else if(ch == L'\t')
			{
				retValue													+= L"\\t";
			}
			else if(ch < L' ')
			{
				retValue													+= L"\\u";
				for(int j = 0; j < 4; j ++)
				{
					int value												= (ch >> 12) & 0xf;
					if (value >= 0 && value <= 9)
						retValue											+= static_cast<wchar_t>(L'0' + value);
					else if (value >= 10 && value <= 15)
						retValue											+= static_cast<wchar_t>(L'A' + (value - 10));

					ch <<= 4;
				}
			}
			else
			{
				retValue													+= ch;
			}
		}

		return retValue;
	}

	//
	// escape xml
	//
	UniString escapeXml(UniString const& xmlString)
	{
		UniString retValue;
		for(size_t i = 0; i < xmlString.length(); ++ i)
		{
			wchar_t ch														= xmlString[i];
			if(ch == L'<')
				retValue													+= L"&lt;";
			else if(ch == L'>')
				retValue													+= L"&gt;";
			else if(ch == L'&')
				retValue													+= L"&amp;";
			else if(ch == L'\r')
				retValue													+= L"&#x0d;";
			else
				retValue													+= ch;
		}

		return retValue;
	}

	//
	// append path
	//
	UniString appendPathComponent(UniString const& path, UniString const& fileName, wchar_t const* extension)
	{
		UniString retValue;
		if(path.length() && (path[path.length() - 1] != L'\\' || path[path.length() - 1] != L'/'))
			retValue														= path + L'/' + fileName;
		else
			retValue														= path + fileName;

		if(extension)
			retValue														+= extension;

		return retValue;
	}

	//
	// split
	//
	std::vector<UniString> split(UniString const& inputString, wchar_t sep)
	{
		std::vector<UniString> retValue;
		UniString::size_type offset											= 0;
		while(offset < inputString.size())
		{
			auto pos														= inputString.find_first_of(sep, offset);
			if(pos == UniString::npos)
				break;

			retValue.push_back(inputString.substr(offset, pos - offset));
			offset															= pos + 1;
		}

		return retValue;
	}

	//
	// split csv line
	//
	std::vector<Utf8String> splitCSVLine(char const* dataBuffer, uint32_t dataLength, uint32_t* usedLength)
	{
		Utf8String currentValue;
		std::vector<Utf8String> retValue;
		bool inQuotes														= false;
		uint32_t offset														= 0;
		while(offset < dataLength)
		{
			char ch															= *dataBuffer++;
			offset															+= 1;
			if(!ch)
				break;

			if(!inQuotes && ch == '"' && currentValue.empty())
			{
				inQuotes													= true;
			}
			else if(inQuotes && ch == '"')
			{
				if(offset < dataLength && *dataBuffer == '"')
				{
					currentValue.push_back(ch);
					dataBuffer												+= 1;
					offset													+= 1;
				}
				else
				{
					inQuotes												= false;
				}
			}
			else if(!inQuotes && ch == ',')
			{
				retValue.push_back((currentValue));
				currentValue.clear();
			}
			else if(!inQuotes && (ch == '\r' || ch == '\n'))
			{
				if(ch == '\r' && offset < dataLength && *dataBuffer == '\n')
				{
					dataBuffer												+= 1;
					offset													+= 1;
				}
				break;
			}
			else
			{
				currentValue.push_back(ch);
			}
		}

		retValue.push_back((currentValue));
		if(usedLength)
			*usedLength														= offset;

		return retValue;
	}

	//
	// trim
	//
	UniString trim(UniString const& inputString)
	{
		if(inputString.empty())
			return L"";

		UniString::size_type startPos										= inputString.find_first_not_of(L" \t\r\n");
		UniString::size_type endPos											= inputString.find_last_not_of(L" \t\r\n");
		if(startPos == UniString::npos || endPos == UniString::npos)
			return L"";

		if(endPos >= startPos)
			return inputString.substr(startPos, endPos - startPos + 1);

		return L"";
	}
}
