
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

#include "espmissingincludes.h"
#include "user_interface.h"
#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "httpd/httpd.h"
#include "httpd/httpdespfs.h"
#include "httpd/cgi.h"
#include "httpd/cgiwifi.h"
#include "driver/stdout.h"
#include "driver/io.h"
#include "driver/uart.h"
#include "lwip/dhcp.h"

#include "led/led.h"

static os_timer_t some_timer;

//timer stuff
//#define OUT_IO_MUX PERIPHS_IO_MUX_GPIO2_U
//#define OUT_IO_NUM 2
//#define OUT_IO_FUNC  FUNC_GPIO2



HttpdBuiltInUrl builtInUrls[] =
{
	{ "/", cgiRedirect, "/index.tpl" },
	{ "/index.tpl", cgiEspFsTemplate, tplMain },
	{ "/steady.html", cgiRedirectProgram, "/steady.tpl" },
	{ "/triple.html", cgiRedirectProgram, "/triple.tpl" },
	{ "/glow.html", cgiRedirectProgram,  "glow.tpl"},

	{ "/steady.cgi", cgiSteady, NULL},

	{ "/refresh.cgi", cgiRefresh, NULL},

//	{ "/flash.bin", cgiReadFlash, NULL },
//	{ "/led.tpl", cgiEspFsTemplate, tplLed },
//	{ "/led.cgi", cgiLed, NULL },

	//Routines to make the /wifi URL and everything beneath it work.
	{ "/wifi", cgiRedirect, "/wifi/wifi.tpl" },
	{ "/wifi/", cgiRedirect, "/wifi/wifi.tpl" },
	{ "/wifi/wifiscan.cgi", cgiWiFiScan, NULL },
	{ "/wifi/wifi.tpl", cgiEspFsTemplate, tplWlan },
	{ "/wifi/connect.cgi", cgiWiFiConnect },

	{ "*", cgiEspFsHook, NULL }, //Catch-all cgi function for the filesystem
	{ NULL, NULL, NULL }
};

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


void user_init(void)
{
	dhcp_set_hostname("roof");		//todo

	//either
	//stdoutInit();
	//ioInit();
	//or
	uart_init(BIT_RATE_115200, BIT_RATE_115200);

//	apa102_init();		//tmp
//	steadySetColor(0,0,0);
//	setProgram(GLOW);

//	wifi_station_disconnect();
//	wifi_set_opmode(STATION_MODE);
	//os_printf("mode: %d\n", wifi_get_opmode());
	//struct station_config stationConfig;
	//strcpy((char*)&stationConfig.ssid, "Ourea");					//WE NEED MORE INFO!!!!
	//strcpy((char*)&stationConfig.password, "srynotmypassword");
//	wifi_station_disconnect();
//	wifi_station_set_config(&stationConfig);

//	wifi_station_scan(NULL,scan_done);
//	wifi_station_connect();

	led_init();
	httpdInit(builtInUrls, 80);
	os_printf("\nReady new\n");


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
