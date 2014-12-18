//
// Prefix header for all source files of the 'op-client' target in the 'op-client' project
//

#ifdef __OBJC__
	#import <Foundation/Foundation.h>
	#import <UIKit/UIKit.h>
#endif


#ifdef __cplusplus
//#include <cstdint>
#include <ctime>
#include <cstdio>
#include <clocale>
#include <string.h>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <exception>
#include <memory>
#include <algorithm>
#include <functional>
#include <sstream>

typedef std::wstring UniString;
typedef std::string Utf8String;

#ifdef _WIN32
    #include <process.h>
    #include <ws2tcpip.h>
    #include <tchar.h>
#pragma comment(lib, "ws2_32")
#else
    #include <errno.h>
    #include <unistd.h>
    #include <wctype.h>
    #include <fcntl.h>
    #include <pthread.h>
    #include <syslog.h>
    #include <signal.h>
    #include <sys/stat.h>
    #include <sys/select.h>
    #include <sys/time.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <dirent.h>
    typedef unsigned int													SOCKET;
    #define INVALID_SOCKET													(SOCKET)(~0)
    #define ARRAYSIZE(a)													(sizeof((a))/ sizeof((a)[0]))
    #define random(x)(abs(rand()%x))
#endif

#include "cocos2d.h"
#include "cocos-ext.h"
#include "myutils/callback.h"
#include "myutils/platformhelper.h"
#include "myutils/randomhelper.h"
#include "myutils/stringhelper.h"
#include "myutils/exceptionhelper.h"
#include "myutils/smartpointer.h"
#include "myutils/singleton.h"
#include "myutils/databuffer.h"
#include "myutils/md5helper.h"
#include "myutils/rc4.h"
//#include "myutils/addresshelper.h"
//#include "myutils/asyncsocket.h"
//#include "myutils/networkservice.h"
#include "myutils/rapidxml.hpp"
#include "myutils/rapidxml_print.hpp"

#include "cocos2d.h"
#include "cocos-ext.h"
extern "C" {
#include <lua.h>  
}
//#include "lua.h"
#include "lunar.h"
#include "CCLuaEngine.h"

#include "batchconfig.h"
#endif