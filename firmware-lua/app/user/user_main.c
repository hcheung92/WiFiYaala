/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2014/1/1, v1.0 create this file.
*******************************************************************************/
#include "lua.h"
#include "platform.h"
#include "c_string.h"
#include "c_stdlib.h"
#include "c_stdio.h"

#include "romfs.h"
 
#include "user_interface.h"

#include "ets_sys.h"
#include "driver/uart.h"
#include "mem.h"

//temp
#include "led/led.h"
//temp
static os_timer_t some_timer;

#define SIG_LUA 0
#define TASK_QUEUE_LEN 4
os_event_t *taskQueue;

void task_lua(os_event_t *e){
    char* lua_argv[] = { (char *)"lua", (char *)"-i", NULL };
    NODE_DBG("Task task_lua started.\n");
    switch(e->sig){
        case SIG_LUA:
            NODE_DBG("SIG_LUA received.\n");
            lua_main( 2, lua_argv );
            break;
        default:
            break;
    }
}

void task_init(void){
    taskQueue = (os_event_t *)os_malloc(sizeof(os_event_t) * TASK_QUEUE_LEN);
    system_os_task(task_lua, USER_TASK_PRIO_0, taskQueue, TASK_QUEUE_LEN);
}

extern void spiffs_mount();
// extern void test_spiffs();
// extern int test_romfs();


//temp
void some_timerfunc(void *arg)
{
	static int i=0;

	if(i<4)
	{
		rgb8_t col = {{255, 0, 0}};
		led_set(i, col, 1000);
	}
	else
	{
		rgb8_t col = {{0, 0, 0}};
		led_set(i-4, col, 1000);
	}
	if(++i==8)
		i=0;

/*	if(!i)
	{
		rgb8_t col = {{255, 0, 0}};
		led_set(0, col, 1000);
		led_set(1, col, 1000);
		led_set(2, col, 1000);
		i=1;
	}
	else
	{
		rgb8_t col = {{0, 0, 0}};
		led_set(0, col, 1000);
		led_set(1, col, 1000);
		led_set(2, col, 1000);
		i=0;
	}
*/
}


/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
    // NODE_DBG("SDK version:%s\n", system_get_sdk_version());
    // system_print_meminfo();
    // os_printf("Heap size::%d.\n",system_get_free_heap_size());
    // os_delay_us(50*1000);   // delay 50ms before init uart

#ifdef DEVELOP_VERSION
    uart_init(BIT_RATE_74880, BIT_RATE_74880);
#else
    uart_init(BIT_RATE_9600, BIT_RATE_9600);
#endif
    // uart_init(BIT_RATE_115200, BIT_RATE_115200);
    
    #ifndef NODE_DEBUG
    system_set_os_print(0);
    #endif
    
    NODE_ERR("\n");
    // Initialize platform first for lua modules.   
    if( platform_init() != PLATFORM_OK )
    {
        // This should never happen
        NODE_DBG("Can not init platform for modules.\n");
        return;
    }
#if defined( BUILD_WOFS )
    romfs_init();

    // if( !wofs_format() )
    // {
    //     NODE_ERR( "\ni*** ERROR ***: unable to erase the flash. WOFS might be compromised.\n" );
    //     NODE_ERR( "It is advised to re-flash the NodeWifi image.\n" );
    // }
    // else
    //     NODE_ERR( "format done.\n" );

    // test_romfs();
#elif defined ( BUILD_SPIFFS )
    spiffs_mount();
    // test_spiffs();
#endif
    // endpoint_setup();

    // char* lua_argv[] = { (char *)"lua", (char *)"-e", (char *)"print(collectgarbage'count');ttt={};for i=1,100 do table.insert(ttt,i*2 -1);print(i);end for k, v in pairs(ttt) do print('<'..k..' '..v..'>') end print(collectgarbage'count');", NULL };
    // lua_main( 3, lua_argv );
    // char* lua_argv[] = { (char *)"lua", (char *)"-i", NULL };
    // lua_main( 2, lua_argv );
    // char* lua_argv[] = { (char *)"lua", (char *)"-e", (char *)"pwm.setup(0,100,50) pwm.start(0) pwm.stop(0)", NULL };
    // lua_main( 3, lua_argv );
    
    task_init();
    system_os_post(USER_TASK_PRIO_0,SIG_LUA,'s');

	led_init();
	//Disarm timer
	os_timer_disarm(&some_timer);

	//Setup timer
	os_timer_setfn(&some_timer, (os_timer_func_t *) some_timerfunc, NULL);

	//Arm the timer
	//1000 is the fire time in ms
	//0 for once and 1 for repeating
	some_timerfunc(NULL);
	some_timerfunc(NULL);
	some_timerfunc(NULL);
	os_timer_arm(&some_timer, 1000, 1);
}
