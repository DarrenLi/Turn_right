#include "stdafx.h"
#include "clientservice_lua.h"
#include "sdk_interface.h"
#include "EGPBridge.h"

//#define RECHARGE_DATA_IP "http://www.gamehetu.com/getGameGoodsList.php?gameid=%d&cid=%d&gid=%d&pt=%s&userid=%s&roleid=%s"
//#define APPORDER_ID_IP  "http://www.gamehetu.com/pay/makeorder.php"
//#define APPORDER_ID_POST  "gameid=%d&gid=%d&userid=%s&roleid=%s&goodsid=%d&pt=%s&mac=%s&idfa=%s"

//#define LOGIN_GAME_IP  "http://clientdata.gamehetu.com/api/clientdata.php"
//#define LOGIN_GAME_POST  "action=logingame&gameid=%d&versions=%s&os=%d&mac=%s&idfa=%s&pt=%s&info=%s&gid=%d&userid=%s&roleid=%s&imei=%s&openudid=%s"
//#define SCRATCH_CARD_IP  "http://tuiguang.gamehetu.com/api/card.php"
//#define SCRATCH_CARD_POST  "number=%s&cardid=%d&userid=%s&roleid=%d&gid=%s&gameid=%s&cid=%s&memberid=%s&idfa=%s&mac=%s"

//#define ANNOUNCEMENT_IP  "http://opadmin.gamehetu.com/client/notice/os/%d/pt/%s/"


static int service_id2str(lua_State* L)
{
	const char* idBinary = lua_tostring(L, 1);
	uint64_t numId = 0;
	for (int i = 7, c = 0; i >= 0; --i, ++c)
	{
		memcpy(((unsigned char*)&numId) + c, idBinary + i, sizeof(unsigned char));
	}
	char szBuff[64] = "";
	platform::snprintf(szBuff, sizeof(szBuff)-1, "%llu", numId);
	lua_pushstring(L, szBuff);
	return 1;
}

static int service_str2id(lua_State* L)
{
	const char* idStr = lua_tostring(L, 1);
	uint64_t numId = platform::atou64(idStr);
	char szBuff[64] = "";
	for (int i = 7, c = 0; i >= 0; --i, ++c)
	{
		memcpy(szBuff + c, ((unsigned char*)&numId) + i, sizeof(unsigned char));
	}
	lua_pushlstring(L, szBuff, 8);
	return 1;
}

static int service_dataversion(lua_State* L)
{
	ssize_t localFileSize = 0;
	FileUtils* pFileUtils = FileUtils::getInstance();

	Utf8String fileNameAndMD5;

	myutils::MD5Helper md5Helper;
	uint8_t md5Result[16] = { 0 };
	std::string nameXml;
	int32_t count = 0;

	lua_getglobal(L, "getVersionTable");
	if (lua_isfunction(L, -1))
	{
		lua_call(L, 0, 1);
		if (lua_istable(L, -1))
		{
			lua_pushnil(L); 
			while (lua_next(L, -2) != 0)
			{
				nameXml = lua_tostring(L, -1);

				char* fileData = (char*)pFileUtils->getFileData(pFileUtils->fullPathForFilename(("table/" + nameXml).c_str()).c_str(), "rb", &localFileSize);

				md5Helper.hashData(fileData, localFileSize, md5Result);

				CC_SAFE_DELETE_ARRAY(fileData);
				fileNameAndMD5 += nameXml + std::string("=") + myutils::binaryToHexUtf8(md5Result, sizeof(md5Result)) + "\r\n";

				lua_pop(L, 1);
			}
		}
		Utf8String temp = fileNameAndMD5;
		md5Helper.hashData(temp.c_str(), temp.length(), md5Result);
		lua_pushstring(L, myutils::binaryToHexUtf8(md5Result, sizeof(md5Result)).c_str());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

static int service_fromCSV(lua_State* L)
{
	const char* filename = lua_tostring(L, 1);
	std::string fullFileName = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);
	ssize_t bufferLength = 0;
	unsigned char* fileBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullFileName.c_str(), "r", &bufferLength);
	const char* charBuffer = (const char*)fileBuffer;
	lua_newtable(L);
	uint32_t valueCount = 0;
	for (uint32_t leftLength = bufferLength, rowIndex = 0; leftLength; rowIndex++)
	{
		uint32_t usedLength = 0;
		std::vector<Utf8String> lineContent = myutils::splitCSVLine(charBuffer, leftLength, &usedLength);
		leftLength -= usedLength;
		charBuffer += usedLength;

		valueCount = static_cast<uint32_t>(lineContent.size());
		
		if (lineContent.size() != valueCount)
		{
			break;
		}

		lua_newtable(L);
		for (uint32_t i = 0; i < valueCount; ++i)
		{
			if (lineContent[i].size() == 0)
			{
				lua_pushinteger(L, 0);
			}
			else
			{
				lua_pushinteger(L, atoi(lineContent[i].c_str()));
			}
			lua_rawseti(L, -2, i+1);
		}
		lua_rawseti(L, -2, rowIndex + 1);
	}
	CC_SAFE_DELETE_ARRAY(fileBuffer);
	return 1;
}

static int service_utf8_len(lua_State* L)
{
	const char* utf8str = lua_tostring(L, 1);
	UniString uniStr = myutils::utf8ToWide(utf8str);
	lua_pushinteger(L, uniStr.length());
	return 1;
}

static int service_set_timezone(lua_State* L)
{
	return 0;
}

static int service_set_timezone2(lua_State* L)
{
    const char* utf8str = lua_tostring(L, 1);

#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    if(utf8str && *utf8str)
        setenv("TZ", utf8str, 1);
    tzset();
#endif
	return 0;
}

static const struct luaL_Reg thislib[] = {
	{ "id2str", service_id2str },
	{ "str2id", service_str2id },
	{ "dataVersion", service_dataversion },
	{ "fromCSV", service_fromCSV },
	{ "utf8strlen", service_utf8_len },
	{ "setTimezone", service_set_timezone },
    { "setTimezone2", service_set_timezone2 },
	{ NULL, NULL }
};

LUALIB_API int luaopen_service(lua_State *L) {
	luaL_register(L, "service", thislib);
	Lunar<RandomLua>::Register(L);
	Lunar<SdkValues>::Register(L);
	Lunar<issGameSdk>::Register(L);
	return 1;
}

const char RandomLua::className[] = "randomhelper";

Lunar<RandomLua>::RegType RandomLua::methods[] = 
{
	LUNAR_DECLARE_METHOD(RandomLua, getRandomValue),
	{0, 0}
};

int32_t RandomLua::getRandomValue(lua_State* L)
{
	lua_pushnumber(L, mRandomHelper.getRandomValue(luaL_optinteger(L, 1, 999999), luaL_optinteger(L, 2, 0)));
	return 1;
}

const char SdkValues::className[] = "sdkvalue";

Lunar<SdkValues>::RegType SdkValues::methods[] = 
{
	LUNAR_DECLARE_METHOD(SdkValues, getGameId),
	LUNAR_DECLARE_METHOD(SdkValues, getPlatformName),
	LUNAR_DECLARE_METHOD(SdkValues, getVersion),
	LUNAR_DECLARE_METHOD(SdkValues, getDeviceType),
	LUNAR_DECLARE_METHOD(SdkValues, getChannelSDKId),
	LUNAR_DECLARE_METHOD(SdkValues, openURL),
	LUNAR_DECLARE_METHOD(SdkValues, copyStr),
	LUNAR_DECLARE_METHOD(SdkValues, getConfigType),
	LUNAR_DECLARE_METHOD(SdkValues, getCurrencytype),
	LUNAR_DECLARE_METHOD(SdkValues, cleanNotice),
	LUNAR_DECLARE_METHOD(SdkValues, startNotice),
	LUNAR_DECLARE_METHOD(SdkValues, shareFacebook),
	LUNAR_DECLARE_METHOD(SdkValues, flyerTrackEvent),
	{ 0, 0 }
};

int32_t SdkValues::getGameId(lua_State* L)
{
	lua_pushinteger(L, platform_game_id);
	return 1;
}

int32_t SdkValues::getPlatformName(lua_State* L)
{
	lua_pushstring(L, platform_name);
	return 1;
}

int32_t SdkValues::getChannelSDKId(lua_State* L)
{
	lua_pushstring(L, CHANNELSDKID);
	return 1;
}

int32_t SdkValues::getVersion(lua_State* L)
{
	//lua_pushstring(L, BundleManager::getBundleManager()->getVersion().c_str());
	return 1;
}

int32_t SdkValues::getDeviceType(lua_State* L)
{
	lua_pushinteger(L, platform_device_type);
	return 1;
}

int32_t SdkValues::getConfigType(lua_State* L)
{
	lua_pushstring(L, Channle_Config_Type);
	return 1;
}

int32_t SdkValues::getCurrencytype(lua_State* L)
{
	lua_pushinteger(L, Currency_type);
	return 1;
}

int32_t SdkValues::getGameId()
{
	return platform_game_id;
}

std::string SdkValues::getPlatformName()
{
	return platform_name;
}

std::string SdkValues::getVersion()
{
	return APPVERSION;
}

int32_t SdkValues::getDeviceType()
{
	return platform_device_type;
}

int SdkValues::openURL(lua_State* L)
{
	const char* utf8str = lua_tostring(L, 1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    issGameSdk::OpenIOSUrl(utf8str);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EGPBridge::openURLAndroid(utf8str);
#endif
	return 0;
}

int SdkValues::copyStr(lua_State* L)
{
    const char* utf8str = lua_tostring(L, 1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    issGameSdk::CopyIOSString(utf8str);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EGPBridge::accessClipboardAndroid(utf8str);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (OpenClipboard(NULL))
	{
		int len = strlen(utf8str) + 1;
		HGLOBAL hmem = GlobalAlloc(GHND, len);
		char *pmem = (char *)GlobalLock(hmem);

		EmptyClipboard();
		memcpy(pmem, utf8str, len);
		SetClipboardData(CF_TEXT, hmem);
		CloseClipboard();
		GlobalFree(hmem);
	}
#endif
	return 0;
}

int SdkValues::cleanNotice(lua_State* L)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	issGameSdk::CleanNotice();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#endif
	return 0;
}

int SdkValues::startNotice(lua_State* L)
{
	const char* utf8str = lua_tostring(L, 1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	issGameSdk::PushNotice(utf8str);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#endif
	return 0;
}

int SdkValues::shareFacebook(lua_State* L)
{
	const char* captionStr = lua_tostring(L, 1);
	const char* desStr = lua_tostring(L, 2);
	const char* linkStr = lua_tostring(L, 3);
	const char* pictureStr = lua_tostring(L, 4);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	issGameSdk::shareFacebook_ios(captionStr,desStr,linkStr,pictureStr);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    EGPBridge::shareFacebookAndroid(captionStr, desStr, linkStr, pictureStr);
#endif
	return 0;
}

int SdkValues::flyerTrackEvent(lua_State* L)
{
	const char* eventStr = lua_tostring(L, 1);
	const char* valueStr = lua_tostring(L, 2);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    issGameSdk::flyerTrackEvent_ios(eventStr,valueStr);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    EGPBridge::flyerTrackEventAndroid(eventStr, valueStr);
#endif
	return 0;
}