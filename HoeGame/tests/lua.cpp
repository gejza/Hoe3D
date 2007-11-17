
#include "../StdAfx.h"
#include "../lua_script.h"

class Console : public HoeGame::BaseConsole
{
public:
	virtual void HOEAPI Con_Print(const char * str)
	{
		printf("%s\n",str);
#if defined(DEBUG) | defined(_DEBUG)
		OutputDebugString( str );OutputDebugString( "\n" );
#endif

	}

};

class Script : HoeGame::LuaScript
{
public:
	void RunDemo()
	{

	}
};

int main()
{
	Console con;
	HoeGame::LuaScript lua(&con);

	lua.Init();
	lua.Load("test.lua");

	lua.func("demo");
	while (lua.Next())
	{
		con.Con_Print("akce");	
	}
	return 0;
}