#ifndef  _EGP_BRIDGE_H_
#define  _EGP_BRIDGE_H_

#include "cocos2d.h"
#include "CCLuaEngine.h"
#include "CCLuaStack.h"

using namespace cocos2d;
using namespace std;

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

class  EGPBridge
{
public:
	static EGPBridge* sharedEGPBridge();			// 获取单实例
	static int Register(LuaStack *pStack);		// 注册EGPBridge到Lua中
	static int initEGPBridge(lua_State* ls);		// 注册支付方法到Lua中
	// 回调事件
	void doCallBack(const char* API, const char* Response, const char* Message);

	//open url
	static void openURLAndroid(const char* url);
	// 拷贝
	static void accessClipboardAndroid(const char* strToCopy);

private:
	static bool _isFirst;
	static EGPBridge* _sharedEGPBridge;

	// 登录
	static int doSignInAndroid(lua_State* ls);
	// 退出
	static int doSignOutAndroid(lua_State* ls);
	// 用户中心
	static int doUserCenterAndroid(lua_State* ls);
	// 显示工具栏
	static int doShowToolBarAndroid(lua_State* ls);
	// 隐藏工具栏
	static int doHideToolBarAndroid(lua_State* s);
	// 支付按钮事件
	static int doPay(lua_State* ls);

	// appsflyer 注册事件
	static int registrationAndroid(lua_State* ls);
	// appsflyer 登录事件
	static int loginAndroid(lua_State* ls);
	// appsflyer 支付事件
	static int purchaseAndroid(lua_State* ls);

	// thai 注销事件
	static int doThaiLogoutCallBack(lua_State* ls);


	~EGPBridge();
};

#endif /* defined(_EGP_BRIDGE_H_) */
