/*
Some random cgi routines.
*/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


#include <string.h>
#include <osapi.h>
#include <stdlib.h>
#include "user_interface.h"
#include "mem.h"
#include "httpd/httpd.h"
#include "httpd/cgi.h"
#include "driver/io.h"
#include "espmissingincludes.h"


// must be in sync with enum programs in apa102.h
HttpdBuitInPrograms builtInPrograms[] =
{
	{ "/steady.html", "Steady Color" },			//0
//	{ "/triple.html", "Triple Fader" },			//1
	{ "/glow.html", "Glowing Tips"}				//2
};


//Cgi that turns the LED on or off according to the 'led' param in the POST data
/*int ICACHE_FLASH_ATTR cgiLed(HttpdConnData *connData) {
	int len;
	char buff[1024];
	
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->postBuff, "led", buff, sizeof(buff));
	if (len!=0) {
		currLedState=atoi(buff);
		ioLed(currLedState);
	}

	httpdRedirect(connData, "led.tpl");
	return HTTPD_CGI_DONE;
}



//Template code for the led page.
void ICACHE_FLASH_ATTR tplLed(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");
	if (os_strcmp(token, "ledstate")==0) {
		if (currLedState) {
			os_strcpy(buff, "on");
		} else {
			os_strcpy(buff, "off");
		}
	}
	espconn_sent(connData->conn, (uint8 *)buff, os_strlen(buff));
}
*/

//Template code for the counter on the index page.
void ICACHE_FLASH_ATTR tplMain(HttpdConnData *connData, char *token, void **arg) {
	char buff[1024];
	if (token==NULL) return;

	if (os_strcmp(token, "programs")==0)
	{
		uint8_t i;
		uint16_t pos = 0;
		for(i=0; i<sizeof(builtInPrograms)/sizeof(builtInPrograms[0]) && pos < 1000; i++)
		{
//			pos += os_sprintf(&buff[pos], "<option value=\"%s\" %s>%s</option>", builtInPrograms[i].url, getProgram()==i?"selected":"", builtInPrograms[i].name);
		}
	}
	espconn_sent(connData->conn, (uint8 *)buff, os_strlen(buff));
}

int ICACHE_FLASH_ATTR cgiRedirectProgram(HttpdConnData *connData)
{
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	int i;
	for(i=0; i<sizeof(builtInPrograms)/sizeof(builtInPrograms[0]); i++)
	{
		if(os_strcmp(connData->url, builtInPrograms[i].url) == 0)
		{
//			setProgram(i);
			break;
		}
	}
//	os_printf("program changed to: %d\n", getProgram());

	httpdRedirect(connData, (char*)connData->cgiArg);
	return HTTPD_CGI_DONE;
}

int ICACHE_FLASH_ATTR cgiSteady(HttpdConnData *connData)
{
	int len;
	char buff[1024];

	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->postBuff, "steadyColor", buff, sizeof(buff));
	if (len!=0)
	{
//		uint32_t rgb;
//		rgb = strtoul(buff+1, NULL, 16);
		//os_printf("update: %x:%x,%x,%x,\n", rgb, ((uint8_t *) &rgb)[0], ((uint8_t *) &rgb)[1], ((uint8_t *) &rgb)[2]);
//		steadySetColor(((uint8_t *) &rgb)[2], ((uint8_t *) &rgb)[1], ((uint8_t *) &rgb)[0]);
	}

	httpdStartResponse(connData, 200);
	httpdEndHeaders(connData);
	return HTTPD_CGI_DONE;
}

int ICACHE_FLASH_ATTR cgiRefresh(HttpdConnData *connData)
{
//	int len;
//	char buff[1024];
//	rgb_t col;

	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdEndHeaders(connData);

//	len = os_sprintf(buff, "{\"program\":\"%d\", \"data\" : {", getProgram());
//	espconn_sent(connData->conn, (uint8 *)buff, len);
//	switch(getProgram())
//	{
//	case STEADY:
//		col = steadyGetColor();
//		len = os_sprintf(buff, "\"value\" : \"#%02x%02x%02x\"}}", col.red, col.grn, col.blu);
//		break;
//
//	default:
//
//		len = os_sprintf(buff,"}}");		//unknown program
//	}
//	espconn_sent(connData->conn, (uint8 *)buff, len);

	return HTTPD_CGI_DONE;
}

//Cgi that reads the SPI flash. Assumes 512KByte flash.
/*int ICACHE_FLASH_ATTR cgiReadFlash(HttpdConnData *connData) {
	int *pos=(int *)&connData->cgiData;
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	if (*pos==0) {
		os_printf("Start flash download.\n");
		httpdStartResponse(connData, 200);
		httpdHeader(connData, "Content-Type", "application/bin");
		httpdEndHeaders(connData);
		*pos=0x40200000;
		return HTTPD_CGI_MORE;
	}
	espconn_sent(connData->conn, (uint8 *)(*pos), 1024);
	*pos+=1024;
	if (*pos>=0x40200000+(512*1024)) return HTTPD_CGI_DONE; else return HTTPD_CGI_MORE;
}
*/
