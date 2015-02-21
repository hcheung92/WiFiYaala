// Module for interfacing with adc

//#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "lwip/dhcp.h"
#include "c_types.h"

#include "led/led.h"

// Lua: led.init(count) , return 0
static int ICACHE_FLASH_ATTR lled_init( lua_State* L )
{
	uint16_t leds = (uint16_t)luaL_checkinteger( L, 1 );

	uint16_t count = led_init(leds);
	if (count == 0) luaL_error( L, "Out of memory" );
	  
	lua_pushinteger(L, count);	
	return 1;
}

// Lua: led.deinit() , return 0
static int ICACHE_FLASH_ATTR lled_deinit( lua_State* L )
{
	led_deinit();
	return 0;
}

// Lua: led.inited() , return count
static int ICACHE_FLASH_ATTR lled_inited( lua_State* L )
{
	uint16_t leds = led_inited();
	lua_pushinteger(L, leds);
	return 1;
}

// Lua: led.set(from, to, r, g, b, time) , return 0
static int ICACHE_FLASH_ATTR lled_set( lua_State* L )
{
	ledrange_t range;	
	rgb8_t led;
	
	uint16_t count = led_checkRange(luaL_checkinteger( L, 1 ),
					luaL_checkinteger( L, 2 ),
					&range);	
	if (!count) return luaL_error( L, "Not initialized" );

	led.red = (uint8_t) luaL_checkinteger( L, 3 );
	led.grn = (uint8_t) luaL_checkinteger( L, 4 );
	led.blu = (uint8_t) luaL_checkinteger( L, 5 );
	unsigned int time = (unsigned int) luaL_checkinteger( L, 6 );

	if (!led_set(range, led, time)) return luaL_error( L, "Type not set" );

	lua_pushinteger(L, count);	
	return 1;
}

// Lua: led.get(from, to) , return r, g, b
static int ICACHE_FLASH_ATTR lled_get( lua_State* L )
{
	uint16_t count;
	ledrange_t range;	
	rgb8_t value;	

	count = led_checkRange(luaL_checkinteger( L, 1 ),
	                       luaL_checkinteger( L, 2 ),
	                       &range);	
	if (!count) return luaL_error( L, "Not initialized" );

	led_get(range, &value);

	lua_pushinteger(L, value.red);
	lua_pushinteger(L, value.grn);
	lua_pushinteger(L, value.blu);
	return 3;
}

// Lua: led.dim(from, to, dim) , return 0
static int ICACHE_FLASH_ATTR lled_dim( lua_State* L )
{
	uint16_t count;
	ledrange_t range;
	uint8_t dim;

	count = led_checkRange(luaL_checkinteger( L, 1 ),
	                       luaL_checkinteger( L, 2 ),
	                       &range);	
	if (!count) return luaL_error( L, "Not initialized" );

	dim = (uint8_t) luaL_checkinteger( L, 3 );
  	if (!led_setDim(range, dim)) luaL_error( L, "Invalid dim value" );
  	
	lua_pushinteger(L, count);	
	return 1;
}

// Lua: led.setType(from, to, type) , return 0
static int ICACHE_FLASH_ATTR lled_setType( lua_State* L )
{
	ledrange_t range;
	uint8_t type;
	uint16_t count;
	
	count = led_checkRange(luaL_checkinteger( L, 1 ),
	                       luaL_checkinteger( L, 2 ),
	                       &range);	
	if (!count) return luaL_error( L, "Not initialized" );

	type = (uint8_t)luaL_checkinteger( L, 3 );

	if( !led_setType(range, type) ) return luaL_error( L, "Invalid type" );

	lua_pushinteger(L, count);	
	return 1;
}

// Lua: led.setType(ch0, ch1, ch2, ch3) , return 0
static int ICACHE_FLASH_ATTR lled_setWhiteBehav( lua_State* L )
{
	uint8_t ch0 = (uint8_t)luaL_checkinteger( L, 1 );
	uint8_t ch1 = (uint8_t)luaL_checkinteger( L, 2 );
	uint8_t ch2 = (uint8_t)luaL_checkinteger( L, 3 );
	uint8_t ch3 = (uint8_t)luaL_checkinteger( L, 4 );

	if (!led_setWhiteBehaviour(ch0, ch1, ch2, ch3)) return luaL_error( L, "Invalid behaviour" );
	return 0;
}

// Module function map
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"
const LUA_REG_TYPE led_map[] =
{
	{ LSTRKEY( "init" ), LFUNCVAL( lled_init ) },
	{ LSTRKEY( "deinit" ), LFUNCVAL( lled_deinit ) },
	{ LSTRKEY( "inited" ), LFUNCVAL( lled_inited ) },
	{ LSTRKEY( "set" ), LFUNCVAL( lled_set ) },
	{ LSTRKEY( "get" ), LFUNCVAL( lled_get ) },
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
