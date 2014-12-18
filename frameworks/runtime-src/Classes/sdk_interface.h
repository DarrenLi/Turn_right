//
//  sdk_interface.h
//  op-client
//
//  Created by xuruoji on 1/13/14.
//
//

#ifndef op_client_sdk_interface_h
#define op_client_sdk_interface_h

class issGameSdk
{
public:
	static const char className[];
	static Lunar<issGameSdk>::RegType methods[];
    
	issGameSdk(lua_State* L) {}
    
	int doPayment(lua_State* L);
    
    int startTOBServer(lua_State* L);
    
	int completeTransactionLua(lua_State* L);
    
    int signIn(lua_State* L);
    
    int signOut(lua_State* L);
    
    int doUserCenter(lua_State* L);
    
    int doShowToolBar(lua_State* L);

    int doHideToolBar(lua_State* L);
    
    int doSendLoginOrRegistr(lua_State* L);
    
    int doPurchase(lua_State* L);
    
//    static NSDictionary* parseURLParams(NSString *query);
    
    static void OpenIOSUrl(const char* url);
    
    static void CopyIOSString(const char *str);
    
    static void PushNotice(const char *str);
    
    static void CleanNotice();
    
    static void shareFacebook_ios(const char *captionStr,const char *desStr,const char *linkStr,const char *pictureStr);
    
    static void flyerTrackEvent_ios(const char *captionStr,const char *desStr);
};

#endif
