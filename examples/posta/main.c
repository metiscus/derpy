#include <stdio.h>
#include <stdlib.h>

#include "Material.h"
#include "Object.h"

#include <lua.h>
#include <lualib.h>

int main( int argc, char** argv )
{
	if(argc !=2 )
	{
		printf("argument required\n");
		return 1;
	}

	lua_State *lua = lua_open();
	luaL_openlibs(lua);

	if( luaL_loadfile(lua, argv[1]) || lua_pcall(lua, 0, LUA_MULTRET, 0) )
	{
		printf("lua error: '%s'\n", lua_tostring(lua, -1));
		return 1;
	}
    
    return 0;
}
