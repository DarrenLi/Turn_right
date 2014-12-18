#include "stdafx.h"
#include "EGPBridge.h"
#include "CCLuaEngine.h"

#include <jni.h>
#include "platform/android/jni/JniHelper.h"

#include "MobClickCpp.h"

bool EGPBridge::_isFirst;
EGPBridge* EGPBridge::_sharedEGPBridge;

EGPBridge* EGPBridge::sharedEGPBridge() {
	if (!_isFirst){
		_sharedEGPBridge = new EGPBridge();
	}
	return _sharedEGPBridge;
}

int EGPBridge::Register(LuaStack *pStack) {
	if (pStack == NULL) return -1;
	lua_State* L = pStack->getLuaState();
	lua_register(L, "doSignInAndroid", doSignInAndroid);
	lua_register(L, "doSignOutAndroid", doSignOutAndroid);
	lua_register(L, "doUserCenterAndroid", doUserCenterAndroid);
	lua_register(L, "doShowToolBarAndroid", doShowToolBarAndroid);
	lua_register(L, "doHideToolBarAndroid", doHideToolBarAndroid);
	lua_register(L, "doPay", doPay);
	//appsflyer
	lua_register(L, "registrationAndroid", registrationAndroid);
	lua_register(L, "loginAndroid", loginAndroid);
	lua_register(L, "purchaseAndroid", purchaseAndroid);

	//thai logout
	lua_register(L, "doThaiLogoutCallBack", doThaiLogoutCallBack);

	return 0;
}

int EGPBridge::initEGPBridge(lua_State* ls) {
	return 0;
}

int EGPBridge::doSignInAndroid(lua_State* ls) {
	JniMethodInfo mInfo;
	jobject jTarget;

	bool flag = JniHelper::getStaticMethodInfo(mInfo, "com/hetu/op/gump/opclient4android", "signIn", "()V");
	if (!flag){
		CCLOG("JniHelper::getMethodInfo error...");
		return 0;
	}
	mInfo.env->CallStaticObjectMethod(mInfo.classID, mInfo.methodID);
	mInfo.env->DeleteLocalRef(mInfo.classID);
	return 0;
}

int EGPBridge::doSignOutAndroid(lua_State* ls) {

	MobClickCpp::end();

	JniMethodInfo mInfo;
	jobject jTarget;

	bool flag = JniHelper::getStaticMethodInfo(mInfo, "com/hetu/op/gump/opclient4android", "signOut", "()V");
	if (!flag){
		CCLOG("JniHelper::getMethodInfo error...");
		return 0;
	}
	mInfo.env->CallStaticObjectMethod(mInfo.classID, mInfo.methodID);
	mInfo.env->DeleteLocalRef(mInfo.classID);
	return 0;
}

int EGPBridge::doUserCenterAndroid(lua_State* ls) {

	return 0;
}

int EGPBridge::doShowToolBarAndroid(lua_State* ls) {

	return 0;
}

int EGPBridge::doHideToolBarAndroid(lua_State* ls) {

	return 0;
}

int EGPBridge::doPay(lua_State* ls) {
	const char *luaStr = lua_tostring(ls, 1);

	JniMethodInfo mInfo;
	jobject jTarget;

	bool flag = JniHelper::getStaticMethodInfo(mInfo, "com/hetu/op/gump/opclient4android", "doPay", "(Ljava/lang/String;)V");
	if (!flag){
		CCLOG("JniHelper::getMethodInfo error...");
		return 0;
	}
	jstring OrderString = mInfo.env->NewStringUTF(luaStr);
	mInfo.env->CallStaticObjectMethod(mInfo.classID, mInfo.methodID, OrderString);
	mInfo.env->DeleteLocalRef(OrderString);
	mInfo.env->DeleteLocalRef(mInfo.classID);
	return 0;
}

int EGPBridge::registrationAndroid(lua_State* ls) {
	const char *luaStr = lua_tostring(ls, 1);

	JniMethodInfo mInfo;
	jobject jTarget;

	bool flag = JniHelper::getStaticMethodInfo(mInfo, "com/hetu/op/gump/opclient4android", "registrationAppsFlyer", "(Ljava/lang/String;)V");
	if (!flag){
		CCLOG("JniHelper::getMethodInfo error...");
		return 0;
	}
	jstring OrderString = mInfo.env->NewStringUTF(luaStr);
	mInfo.env->CallStaticObjectMethod(mInfo.classID, mInfo.methodID, OrderString);
	mInfo.env->DeleteLocalRef(OrderString);
	mInfo.env->DeleteLocalRef(mInfo.classID);
	return 0;
}

int EGPBridge::loginAndroid(lua_State* ls) {
	const char *luaStr = lua_tostring(ls, 1);

	JniMethodInfo mInfo;
	jobject jTarget;

	bool flag = JniHelper::getStaticMethodInfo(mInfo, "com/hetu/op/gump/opclient4android", "loginAppsFlyer", "(Ljava/lang/String;)V");
	if (!flag){
		CCLOG("JniHelper::getMethodInfo error...");
		return 0;
	}
	jstring OrderString = mInfo.env->NewStringUTF(luaStr);
	mInfo.env->CallStaticObjectMethod(mInfo.classID, mInfo.methodID, OrderString);
	mInfo.env->DeleteLocalRef(OrderString);
	mInfo.env->DeleteLocalRef(mInfo.classID);
	return 0;
}

int EGPBridge::purchaseAndroid(lua_State* ls) {
	const char *luaStr = "gump";

	MobClickCpp::pay(10, 2, 10);
	return 0;
}

void EGPBridge::openURLAndroid(const char* url) {
	JniMethodInfo mInfo;
	jobject jTarget;

	bool flag = JniHelper::getStaticMethodInfo(mInfo, "com/hetu/op/gump/opclient4android", "openURLAndroid", "(Ljava/lang/String;)V");
	if (!flag){
		CCLOG("JniHelper::getMethodInfo error...");
		return;
	}
	jstring urlString = mInfo.env->NewStringUTF(url);
	mInfo.env->CallStaticObjectMethod(mInfo.classID, mInfo.methodID, urlString);
	mInfo.env->DeleteLocalRef(urlString);
	mInfo.env->DeleteLocalRef(mInfo.classID);
}

int EGPBridge::doThaiLogoutCallBack(lua_State* ls) {
	JniMethodInfo mInfo;
	jobject jTarget;

	bool flag = JniHelper::getStaticMethodInfo(mInfo, "com/hetu/op/gump/opclient4android", "doThaiLogoutCallBack", "()V");
	if (!flag){
		CCLOG("JniHelper::getMethodInfo error...");
		return 0;
	}
	mInfo.env->CallStaticObjectMethod(mInfo.classID, mInfo.methodID);
	mInfo.env->DeleteLocalRef(mInfo.classID);
	return 0;
}

void EGPBridge::accessClipboardAndroid(const char* strToCopy) {
	JniMethodInfo mInfo;
	jobject jTarget;

	bool flag = JniHelper::getStaticMethodInfo(mInfo, "com/hetu/op/gump/opclient4android", "accessClipboardAndroid", "(Ljava/lang/String;)V");
	if (!flag){
		CCLOG("JniHelper::getMethodInfo error...");
		return;
	}
	jstring toCopyStr = mInfo.env->NewStringUTF(strToCopy);
	mInfo.env->CallStaticObjectMethod(mInfo.classID, mInfo.methodID, toCopyStr);
	mInfo.env->DeleteLocalRef(toCopyStr);
	mInfo.env->DeleteLocalRef(mInfo.classID);
}


//java callback
void EGPBridge::doCallBack(const char* API, const char* Response, const char* Message) {
	lua_State*  ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();

	cocos2d::LuaEngine* pEngine = cocos2d::LuaEngine::defaultEngine();

	cocos2d::LuaStack *pStack = pEngine->getLuaStack();

	std::string userName = "";
	std::string userToken = "";

	if (strcmp(API, "signinsuccess") == 0) {
		lua_State* L = pStack->getLuaState();
		lua_getglobal(L, "onSDKOtherLoginCallBack");
		if (lua_isfunction(L, -1))
		{
			char *result = NULL;
			char *buf = new char[strlen(Response) + 1];
			strcpy(buf, Response);

			char *token;
			int j = 0;
			char* arr[2];
			char seps[] = "|";
			token = strtok(buf, seps);
			while (token != NULL)
			{
				printf("%s\n", token);
				arr[j++] = token;
				token = strtok(NULL, seps);
			}

			int userid = atoi(arr[0]);
			const char* tokenkey = arr[1];

			userToken = tokenkey;

			stringstream ss;
			ss << userid;
			userName = ss.str();

			lua_pushinteger(L, userid);
			lua_pushstring(L, tokenkey);
			pStack->executeFunction(2);
		}

		if (userName != "" && userToken != "") {
			cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("sdk_user_name", userName);
			cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("sdk_user_pass", userToken);
		}
	}
	else if (strcmp(API, "signoutsuccess") == 0) {
		cocos2d::LuaStack *pStack = pEngine->getLuaStack();

		lua_State* L = pStack->getLuaState();
		lua_getglobal(L, "onSDKSignOutCallBack");
		if (lua_isfunction(L, -1))
		{
			pStack->executeFunction(0);
		}
		cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("sdk_user_name", "");
		cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("sdk_user_pass", "");
	}
	else if (strcmp(API, "paysuccess") == 0) {
		cocos2d::LuaStack *pStack = pEngine->getLuaStack();

		lua_State* L = pStack->getLuaState();
		lua_getglobal(L, "onSDKRechargeCallBack");
		if (lua_isfunction(L, -1))
		{
			lua_pushstring(L, Response);
			pStack->executeFunction(1);
		}
	}
}

EGPBridge::~EGPBridge() {
	CC_SAFE_DELETE(_sharedEGPBridge);
	_sharedEGPBridge = NULL;
}

// Java回调方法
extern "C"
{
	void Java_com_hetu_op_gump_opclient4android_EGPSDKCallBack(JNIEnv*  env, jobject thiz, jstring APIType, jstring Response, jstring StatusMessage)    // 命名规则：Java_Java的包名_类名
	{
		const char* APITypeChars = env->GetStringUTFChars(APIType, NULL);
		const char* ResponseChars = env->GetStringUTFChars(Response, NULL);
		const char* StatusMessageChars = env->GetStringUTFChars(StatusMessage, NULL);
		// 调用Lua中注册的回调方法
		EGPBridge::sharedEGPBridge()->doCallBack(APITypeChars, ResponseChars, StatusMessageChars);

		env->ReleaseStringUTFChars(APIType, APITypeChars);
		env->ReleaseStringUTFChars(Response, ResponseChars);
		env->ReleaseStringUTFChars(StatusMessage, StatusMessageChars);
	}
}
