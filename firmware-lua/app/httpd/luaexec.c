/*
 * luaexec.c
 *
 *  Created on: Feb 14, 2015
 *      Author: sid
 */

#include "espmissingincludes.h"
#include "c_types.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "osapi.h"
#include "os_type.h"

#include "httpd/httpd.h"
#include "lua.h"

extern lua_Load gLoad;
extern os_timer_t lua_timer;
extern void dojob(lua_Load *load);

//hack
char *lua2httpdret = NULL;
int lua2httpretPos;

int ICACHE_FLASH_ATTR luaPost(HttpdConnData *connData)
{
	char *e;

	connData->postLine[connData->postLinePos] = '\0';

	//eval post line
	if(connData->postLine[0]=='-' && connData->postLine[1]=='-' && os_strncmp(&connData->postLine[2], connData->postBoundary, os_strlen(connData->postBoundary))==0)
	{
//		os_printf("bd\n");
		if(connData->postArg !=NULL)
		{
			os_free(connData->postArg);
			connData->postArg = NULL;
		}

		connData->file = -1;

		if(connData->postLine[2+os_strlen(connData->postBoundary)]=='-' && connData->postLine[2+os_strlen(connData->postBoundary)+1]=='-')
		{
			//Multipart end
//			os_printf("me\n");
			return HTTPD_POST_DONE;
		}
	}
	else if(os_strncmp(connData->postLine, "Content-Disposition", 19)==0)
	{
//		os_printf("CD\n");
		e=(char*)os_strstr(connData->postLine, "lua");
		if(e != NULL)
		{
			((char*)os_strstr(e, "\""))[0] = '\0';
			if(os_strlen(e) && connData->postArg == NULL)
			{
				connData->postArg = (char*)os_malloc(os_strlen(e)+1);
				os_memcpy(connData->postArg, e, os_strlen(e) +1);
//				os_printf("name: '%s'\n", connData->postArg);
			}
		}
	}
	else if(connData->postLinePos==2 && connData->postLine[0]=='\r' && connData->postLine[1]=='\n' && connData->postArg != NULL && connData->file == -1)
	{
//		os_printf("nl\n");
		connData->file = 0;
		httpdStartResponse(connData, 200);
		httpdHeader(connData, "Content-Type", "text/plain");
		httpdHeader(connData, "Cache-Control", "max-age=0, no-cache, no-store, must-revalidate");
		httpdEndHeaders(connData);
	}
	else if(connData->file != -1)
	{
//		os_printf("lua: %s", connData->postLine);

		if(connData->postLine[connData->postLinePos-1] != '\n')
		{
//			os_printf("no LF -- abort\n");
			return HTTPD_POST_DONE;
		}
		else if(connData->postLine[connData->postLinePos-2] == '\r')
		{
			connData->postLinePos -= 2;
			connData->postLine[connData->postLinePos] = '\0';
		}
		else
		{
			connData->postLine[--connData->postLinePos] = '\0';
		}

		lua_Load *load = &gLoad;
		if(load->line_position == 0)
		{
			//prepare lua output redirect
			if(lua2httpdret != NULL)
				os_free(lua2httpdret);
			lua2httpdret = os_malloc(1024);
			lua2httpretPos = 0;

			//execute job
			os_memcpy(load->line, connData->postLine, connData->postLinePos);
			load->line_position = os_strlen(load->line)+1;
			load->done = 1;
			dojob(load);

			//return result
			lua2httpdret[lua2httpretPos] = '\0';
			httpdSend(connData, lua2httpdret, -1);

			//cleanup
			os_free(lua2httpdret);		//within in those 2 lines no output_redirect() should take place!
			lua2httpdret = NULL;

		}
	}
	else
	{
//		os_printf("ul: %s", connData->postLine);
	}


	return HTTPD_POST_MORE;
}
