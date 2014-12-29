// Module for interfacing with adc

//#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
//#include "platform.h"
//#include "auxmods.h"
//#include "lrotable.h"

#include "lwip/dhcp.h"
#include "c_types.h"

// Lua: set(hostname) , return 0
static int ICACHE_FLASH_ATTR hostname_set( lua_State* L )
{
	size_t sl;
	char *hostname =(char *) luaL_checklstring( L, 1, &sl );
	if (sl>32 || hostname == NULL)
		return luaL_error( L, "hostname:<32" );

	dhcp_set_hostname(hostname);
	return 0;
}

// Lua: get() , return 0
static int ICACHE_FLASH_ATTR hostname_get( lua_State* L )
{
	lua_pushstring( L, dhcp_get_hostname());
	return 1;
}

// Module function map
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"
const LUA_REG_TYPE hostname_map[] =
{
	  { LSTRKEY( "set" ), LFUNCVAL( hostname_set ) },
	  { LSTRKEY( "get" ), LFUNCVAL( hostname_get ) },
#if LUA_OPTIMIZE_MEMORY > 0

#endif
  { LNILKEY, LNILVAL }
};

LUALIB_API int ICACHE_FLASH_ATTR luaopen_hostname( lua_State *L )
{
#if LUA_OPTIMIZE_MEMORY > 0
  return 0;
#else // #if LUA_OPTIMIZE_MEMORY > 0
  luaL_register( L, AUXLIB_HOSTNAME, hostname_map );
  // Add constants

  return 1;
#endif // #if LUA_OPTIMIZE_MEMORY > 0  
}
