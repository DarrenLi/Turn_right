#include "stdafx.h"
#include "sdk_interface.h"
const char issGameSdk::className[] = "issGameSdk";

Lunar<issGameSdk>::RegType issGameSdk::methods[] =
{
	LUNAR_DECLARE_METHOD(issGameSdk, doPayment),
	{ 0, 0 }
};


int issGameSdk::doPayment(lua_State* L)
{

	return 0;
}