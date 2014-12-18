//
//  sdk_interface.c
//  op-client
//
//  Created by xuruoji on 1/13/14.
//
//
#import "stdafx.h"
#import "sdk_interface.h"
#import <StoreKit/StoreKit.h>
#import "TOBServer.h"
#import "LetsGameAPI.h"
#import "AppsFlyerTracker.h"
#import "MobClickGameAnalytics.h"
#import <FacebookSDK/FacebookSDK.h>

const char issGameSdk::className[] = "issGameSdk";

Lunar<issGameSdk>::RegType issGameSdk::methods[] =
{
	LUNAR_DECLARE_METHOD(issGameSdk, doPayment),
	LUNAR_DECLARE_METHOD(issGameSdk, startTOBServer),
	LUNAR_DECLARE_METHOD(issGameSdk, completeTransactionLua),
	LUNAR_DECLARE_METHOD(issGameSdk, signIn),
	LUNAR_DECLARE_METHOD(issGameSdk, signOut),
	LUNAR_DECLARE_METHOD(issGameSdk, doUserCenter),
	LUNAR_DECLARE_METHOD(issGameSdk, doShowToolBar),
	LUNAR_DECLARE_METHOD(issGameSdk, doHideToolBar),
	LUNAR_DECLARE_METHOD(issGameSdk, doSendLoginOrRegistr),
	LUNAR_DECLARE_METHOD(issGameSdk, doPurchase),
	{0, 0}
};


int issGameSdk::signIn(lua_State* L)
{    
    [[LetsGameAPI instance] showLoginView];
    [LetsGameAPI instance].succBlock = ^(NSString *userId, NSString *sessionKey, LSGAccountType type) {
        NSLog(@"login succ: userId = %@, sessionKey = %@, accountType = %d", userId, sessionKey, type);
        cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
        
        cocos2d::CCLuaStack *pStack = pEngine->getLuaStack();
        
        lua_State* L = pStack->getLuaState();
        lua_getglobal(L, "onSDKOtherLoginCallBack");
        if (lua_isfunction(L, -1))
        {
            lua_pushstring(L, [userId UTF8String]);
            lua_pushstring(L, [sessionKey UTF8String]);
            pStack->executeFunction(2);
        }
        
        cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("sdk_user_name", [userId UTF8String]);
        cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("sdk_user_pass", [sessionKey UTF8String]);
    };
    [LetsGameAPI instance].dismissBlock = ^() {
        
    };
    return 0;
}

int issGameSdk::doPayment(lua_State* L)
{
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    CCTextureCache::sharedTextureCache()->removeAllTextures();
    cocos2d::CCDirector::sharedDirector()->purgeCachedData();
  //  const char* appOrderID = lua_tostring(L, 1);
    const char* productId = lua_tostring(L, 2);
//    int price = lua_tointeger(L, 3);
//    const char* userName = lua_tostring(L, 4);
//    const char* privateInfo = lua_tostring(L, 5);
    
    [[TOBServer instance] buy:[NSString stringWithUTF8String:productId]];
    return 0;
}

int issGameSdk::startTOBServer(lua_State *L)
{
    [TOBServer instance];
    return 0;
}

int issGameSdk::completeTransactionLua(lua_State* L)
{
    [[TOBServer instance] completeTransaction];
    return 0;
}

int issGameSdk::doSendLoginOrRegistr(lua_State *L)
{
    const char* eventStr = lua_tostring(L, 1);
    const char* userId = lua_tostring(L, 2);
    [[AppsFlyerTracker sharedTracker] trackEvent:[NSString stringWithUTF8String:eventStr] withValue:@""];
    [[AppsFlyerTracker sharedTracker] setCustomerUserID: [NSString stringWithUTF8String:userId]];
    return 0;
}

int issGameSdk::doPurchase(lua_State *L)
{
    NSString* moneyCount = [NSString stringWithUTF8String:lua_tostring(L, 1)];
    NSString* goldCount = [NSString stringWithUTF8String:lua_tostring(L, 2)];
    NSString* moneyType = [NSString stringWithUTF8String:lua_tostring(L, 3)];
    [[AppsFlyerTracker sharedTracker] setCurrencyCode:moneyType ];
    [[AppsFlyerTracker sharedTracker] trackEvent:@"purchase" withValue:moneyCount];
    NSLog(@"money=%@,gold=%@",moneyCount,goldCount);
    [MobClickGameAnalytics pay: [moneyCount doubleValue] source:1 coin:[goldCount doubleValue]];
    return 0;
}

int issGameSdk::signOut(lua_State* L)
{
    return 0;
}

int issGameSdk::doUserCenter(lua_State* L)
{    return 0;
}

int issGameSdk::doShowToolBar(lua_State* L)
{
    return 0;
}

int issGameSdk::doHideToolBar(lua_State* L)
{
    return 0;
}

void issGameSdk::OpenIOSUrl(const char *url)
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString: [NSString stringWithFormat:@"%s", url]]];
}

void issGameSdk::CopyIOSString(const char *str)
{
    UIPasteboard *pboard = [UIPasteboard generalPasteboard];
    pboard.string = [NSString stringWithUTF8String:str];
}

void issGameSdk::PushNotice(const char *str)
{
    NSString *dataStr = [NSString stringWithUTF8String:str];
    NSArray *dataArray = [dataStr componentsSeparatedByString:@"|"];
    for (int i = 0; i< [dataArray count]; i+=2) {
        NSString *time = [dataArray objectAtIndex:i];
        NSString *str =[dataArray objectAtIndex:i+1];
        NSDate *pushData = [NSDate dateWithTimeIntervalSinceNow: [time intValue]];
        UILocalNotification *notification = [[[UILocalNotification alloc] init] autorelease];
        if (notification != nil){
            notification.fireDate = pushData;
            notification.timeZone = [NSTimeZone defaultTimeZone];
            notification.repeatInterval = kCFCalendarUnitDay;
            notification.soundName = UILocalNotificationDefaultSoundName;
            notification.alertBody = str;
            notification.applicationIconBadgeNumber = 1;
            NSDictionary *info = [NSDictionary dictionaryWithObject:[NSString stringWithFormat:@"notice%d",i] forKey:@"onePieceKey"];
            notification.userInfo = info;
            UIApplication *app = [UIApplication sharedApplication];
            [app scheduleLocalNotification:notification];
            NSLog(@"start tui song");
        }
    }
}

void issGameSdk::CleanNotice()
{
    UIApplication *app =[UIApplication sharedApplication];
    
    [app cancelAllLocalNotifications];
    
    //    NSArray *localArray = [app scheduledLocalNotifications];
    //    if (localArray) {
    //        for (UILocalNotification *noti in localArray) {
    //            NSDictionary *dict = noti.userInfo;
    //            if (dict) {
    //                NSString *inKey = [dict objectForKey:@"onePieceKey"];
    //                if (inKey) {
    //                    [app cancelLocalNotification:noti];
    //                    break;
    //                }
    //            }
    //        }
    //    }
    
}

void issGameSdk::shareFacebook_ios(const char *captionStr,const char *desStr,const char *linkStr,const char *pictureStr)
{
    NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];
    NSString *appName = [infoDictionary objectForKey:@"CFBundleDisplayName"];
    NSString *captionNStr = [NSString stringWithUTF8String:captionStr];
    NSString *desNStr = [NSString stringWithUTF8String:desStr];
    NSString *linkNStr = [NSString stringWithUTF8String:linkStr];
    NSString *pictureNStr = [NSString stringWithUTF8String:pictureStr];
    // Check if the Facebook app is installed and we can present the share dialog
    FBLinkShareParams *params = [[FBLinkShareParams alloc] init];
    params.link = [NSURL URLWithString:linkNStr];
    params.name = appName;
    params.caption = captionNStr;
    params.picture = [NSURL URLWithString:pictureNStr];
    params.linkDescription = desNStr;
    
    // If the Facebook app is installed and we can present the share dialog
    if ([FBDialogs canPresentShareDialogWithParams:params]) {
        
        // Present share dialog
        [FBDialogs presentShareDialogWithLink:params.link
                                      handler:^(FBAppCall *call, NSDictionary *results, NSError *error) {
                                          if(error) {
                                              // An error occurred, we need to handle the error
                                              // See: https://developers.facebook.com/docs/ios/errors
                                              NSLog(@"Error publishing story: %@", error.description);
                                          } else {
                                              // Success
                                              NSLog(@"result %@", results);
                                          }
                                      }];
        
        // If the Facebook app is NOT installed and we can't present the share dialog
    } else {
        // FALLBACK: publish just a link using the Feed dialog
        
        // Put together the dialog parameters
        NSMutableDictionary *params1 = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                        appName, @"name",
                                        captionNStr, @"caption",
                                        desNStr, @"description",
                                        linkNStr, @"link",
                                        pictureNStr, @"picture",
                                        nil];
        // Show the feed dialog
        [FBWebDialogs presentFeedDialogModallyWithSession:nil
                                               parameters:params1
                                                  handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
                                                      if (error) {
                                                          // An error occurred, we need to handle the error
                                                          // See: https://developers.facebook.com/docs/ios/errors
                                                          NSLog(@"Error publishing story: %@", error.description);
                                                      } else {
                                                          if (result == FBWebDialogResultDialogNotCompleted) {
                                                              // User canceled.
                                                              NSLog(@"User cancelled.");
                                                          } else {
                                                              // Handle the publish feed callback
                                                              NSArray *pairs = [resultURL.query componentsSeparatedByString:@"&"];
                                                              NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
                                                              for (NSString *pair in pairs) {
                                                                  NSArray *kv = [pair componentsSeparatedByString:@"="];
                                                                  NSString *val =
                                                                  [kv[1] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
                                                                  params[kv[0]] = val;
                                                              }

                                                              NSDictionary *urlParams = params;
                                                              //[self parseURLParams:[resultURL query]];
                                                              
                                                              if (![urlParams valueForKey:@"post_id"]) {
                                                                  // User canceled.
                                                                  NSLog(@"User cancelled.");
                                                                  
                                                              } else {
                                                                  // User clicked the Share button
                                                                  NSString *result1 = [NSString stringWithFormat: @"Posted story, id: %@", [urlParams valueForKey:@"post_id"]];
                                                                  NSLog(@"result %@", result1);
                                                              }
                                                          }
                                                      }
                                                  }];
    }

}

void issGameSdk::flyerTrackEvent_ios(const char *eventStr,const char *valueStr)
{
    NSString *eventNStr = [NSString stringWithUTF8String:eventStr];
    NSString *valueNStr = [NSString stringWithUTF8String:valueStr];
    [[AppsFlyerTracker sharedTracker] trackEvent:eventNStr withValue:valueNStr];
}

// A function for parsing URL parameters returned by the Feed Dialog.
/*NSDictionary* issGameSdk::parseURLParams(NSString *query)
{
    NSArray *pairs = [query componentsSeparatedByString:@"&"];
    NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
    for (NSString *pair in pairs) {
        NSArray *kv = [pair componentsSeparatedByString:@"="];
        NSString *val =
        [kv[1] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        params[kv[0]] = val;
    }
    return params;
}*/
