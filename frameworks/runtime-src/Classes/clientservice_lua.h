
LUALIB_API int luaopen_service (lua_State *L);

extern "C" 
{
	LUALIB_API int luaopen_lpeg (lua_State *L);
	LUALIB_API int luaopen_struct(lua_State *L);
	LUALIB_API int luaopen_memoryfile(lua_State *L);
}

class RandomLua
{
public:
	static const char className[];
	static Lunar<RandomLua>::RegType methods[];

	RandomLua(lua_State* L) { mRandomHelper.initialize(lua_tointeger(L, 1)); }

	int32_t getRandomValue(lua_State* L);
private:
	myutils::Random mRandomHelper;
};

class SdkValues
{
public:
	static const char className[];
	static Lunar<SdkValues>::RegType methods[];

	SdkValues(lua_State* L) {}

	int32_t getGameId(lua_State* L);
	int32_t getPlatformName(lua_State* L);
	int32_t getChannelSDKId(lua_State* L);
	int32_t getVersion(lua_State* L);
	int32_t getDeviceType(lua_State* L);
    int32_t openURL(lua_State* L);
    int32_t copyStr(lua_State* L);
	int32_t getConfigType(lua_State* L);
	int32_t getCurrencytype(lua_State* L);
	int32_t cleanNotice(lua_State* L);
	int32_t startNotice(lua_State* L);
	int32_t shareFacebook(lua_State* L);
    int32_t flyerTrackEvent(lua_State* L);

	static int32_t getGameId();
	static std::string getPlatformName();
	static std::string getVersion();
	static int32_t getDeviceType();
};