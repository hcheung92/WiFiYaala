// Module for interfacing with adc

//#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "lwip/dhcp.h"
#include "c_types.h"

#include "led/led.h"

// Lua: init(leds) , return 0
static int ICACHE_FLASH_ATTR lled_init( lua_State* L )
{
	  unsigned int leds = (unsigned int)luaL_checkinteger( L, 1 );

	  if(!led_init(leds))
		  return luaL_error( L, "init leds" );
	  else
		  return 0;
}

// Lua: deinit() , return 0
static int ICACHE_FLASH_ATTR lled_deinit( lua_State* L )
{
	led_deinit();
	return 0;
}

// Lua: set(channel, red, green, blue, time) , return 0
static int ICACHE_FLASH_ATTR lled_set( lua_State* L )
{
	  rgb8_t led;
	  unsigned int channel = (unsigned int) luaL_checkinteger( L, 1 );
	  led.red = (uint8_t) luaL_checkinteger( L, 2 );
	  led.grn = (uint8_t) luaL_checkinteger( L, 3 );
	  led.blu = (uint8_t) luaL_checkinteger( L, 4 );
	  unsigned int time = (unsigned int) luaL_checkinteger( L, 5 );

	  if(!led_set(channel, led, time))
		  return luaL_error( L, "set channel,r,g,b,time" );
	  else
		  return 0;
}

// Lua: dim(channel, dim) , return 0
static int ICACHE_FLASH_ATTR lled_dim( lua_State* L )
{
	  unsigned int channel = (unsigned int)luaL_checkinteger( L, 1 );
	  unsigned int dim = (uint8_t) luaL_checkinteger( L, 2 );

	  if(!led_setDim(channel, dim))
		  return luaL_error( L, "set channel,r,g,b,time" );
	  else
		  return 0;
}

// Lua: setTypeApa(from, to) , return 0
static int ICACHE_FLASH_ATTR lled_setType( lua_State* L )
{
	uint8_t type = (uint8_t)luaL_checkinteger( L, 1 );
	unsigned int from = (unsigned int)luaL_checkinteger( L, 2 );
	unsigned int to = (unsigned int) luaL_checkinteger( L, 3 );

	if(!led_setType(type, from, to))
		return luaL_error( L, "setTypeApa type,from,to" );
	else
		return 0;
}
// Lua: setTypeApa(from, to) , return 0
static int ICACHE_FLASH_ATTR lled_setWhiteBehav( lua_State* L )
{
	uint8_t ch0 = (uint8_t)luaL_checkinteger( L, 1 );
	uint8_t ch1 = (uint8_t)luaL_checkinteger( L, 2 );
	uint8_t ch2 = (uint8_t)luaL_checkinteger( L, 3 );
	uint8_t ch3 = (uint8_t)luaL_checkinteger( L, 4 );

	led_setWhiteBehaviour(ch0, ch1, ch2, ch3);
	return 0;
}

// Module function map
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"
const LUA_REG_TYPE led_map[] =
{
	  { LSTRKEY( "init" ), LFUNCVAL( lled_init ) },
	  { LSTRKEY( "deinit" ), LFUNCVAL( lled_deinit ) },
	  { LSTRKEY( "set" ), LFUNCVAL( lled_set ) },
	  { LSTRKEY( "dim" ), LFUNCVAL( lled_dim ) },
	  { LSTRKEY( "setType" ), LFUNCVAL( lled_setType ) },
	  { LSTRKEY( "setWhiteBehav" ), LFUNCVAL( lled_setWhiteBehav ) },
	  { LSTRKEY( "PCA9685" ), LNUMVAL( LED_PCA9685 ) },
	  { LSTRKEY( "APA102" ), LNUMVAL( LED_APA102 ) },
	  { LSTRKEY( "WS2801" ), LNUMVAL( LED_WS2801 ) },
	  { LSTRKEY( "WS281X" ), LNUMVAL( LED_WS281X ) },
	  { LSTRKEY( "WHITE_NA" ), LNUMVAL( WHITE_NA ) },
	  { LSTRKEY( "WHITE_ADD" ), LNUMVAL( WHITE_ADD ) },
	  { LSTRKEY( "WHITE_ADJUST" ), LNUMVAL( WHITE_ADJUST ) },
	  { LSTRKEY( "WHITE_EXTRA" ), LNUMVAL( WHITE_EXTRA ) },


//	  { LSTRKEY( "get" ), LFUNCVAL( led_get ) },
#if LUA_OPTIMIZE_MEMORY > 0

#endif
  { LNILKEY, LNILVAL }
};

LUALIB_API int ICACHE_FLASH_ATTR luaopen_led( lua_State *L )
{
#if LUA_OPTIMIZE_MEMORY > 0
  return 0;
#else // #if LUA_OPTIMIZE_MEMORY > 0
  luaL_register( L, AUXLIB_LED, led_map );
  // Add constants

  return 1;
#endif // #if LUA_OPTIMIZE_MEMORY > 0  
}
