#include "espmissingincludes.h"
#include "c_types.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "osapi.h"
#include "os_type.h"

#include "espconn.h"
#include "httpd/httpd.h"
#include "flash_fs.h"

spiffs_DIR d;
extern spiffs fs;

//This is a catch-all cgi function. It takes the url passed to it, looks up the corresponding
//path in the filesystem and if it exists, passes the file through. This simulates what a normal
//webserver would do with static files.
int ICACHE_FLASH_ATTR fsHook(HttpdConnData *connData)
{
	int len;
	char buff[1024];

	if (connData->conn==NULL)
	{
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	if (connData->file == -1)
	{
		//First call to this cgi. Open the file so we can read it.
		os_printf("fopen %s\n", connData->url);
		connData->file = fs_open(&connData->url[1], FS_RDONLY);
		os_printf("open ret %d\n", connData->file);
		if (connData->file < 0)
		{
			return HTTPD_CGI_NOTFOUND;
		}
		httpdStartResponse(connData, 200);
		httpdHeader(connData, "Content-Type", httpdGetMimetype(connData->url));
		httpdHeader(connData, "Cache-Control", "max-age=3600, must-revalidate");
		httpdEndHeaders(connData);
		return HTTPD_CGI_MORE;
	}

	len = fs_read(connData->file, buff, 1024);
	if (len>0)
		espconn_sent(connData->conn, (uint8 *)buff, len);
	if (len!=1024)
	{
		//We're done.
		fs_close(connData->file);
		return HTTPD_CGI_DONE;
	}
	else
	{
		//Ok, till next time.
		return HTTPD_CGI_MORE;
	}
}


static const char *httpBrowserStart="<!DOCTYPE html><html><head><title>Yaala FS Browser</title></head><body><h1>Yaala FS Browser</h1>"\
		"<form method=\"post\" enctype=\"multipart/form-data\">"\
		"<table cellpadding=\"10\" border=\"1\" rules=\"groups\">"\
		"<thead><tr><td>Name</td><td>Action</td><td>Size</td></tr></thead><tfoot>"\
		"<tr><td><input name=\"file\" size=\"20\" accept=\"*/*\" type=\"file\"></td><td><input type=\"submit\" name=\"add\" value=\"Upload\"></td><td><a href=\"fsbrowse?format\">Format</a></td></tr></tfoot><tbody>";
static const char *httpBrowserFormatErr="<tr><td>FS compromised. Re-flash image.</td></tr>";
static const char *httpBrowserStop="</tbody></table></body></html>";

int ICACHE_FLASH_ATTR fsBrowse(HttpdConnData *connData)
{
	if (connData->conn==NULL)
	{
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	if (connData->file == -1)
	{
		//First call to this cgi.

		httpdStartResponse(connData, 200);
		httpdHeader(connData, "Content-Type", httpdGetMimetype(connData->url));
		httpdHeader(connData, "Cache-Control", "private, max-age=0, no-cache, no-store, must-revalidate");
		httpdEndHeaders(connData);

		httpdSend(connData, httpBrowserStart, -1);
		connData->file = 0;

		if(connData->getArgs != NULL)
		{
			if(!os_strncmp(connData->getArgs, "del", 3))
			{
				char *del = &connData->getArgs[4];
//				os_printf("del: %s\n", del);
				SPIFFS_remove(&fs, del);
			}
			else if(!os_strncmp(connData->getArgs, "format", 6))
			{
				os_printf("format\n");
				if( !fs_format() )
				{
					httpdSend(connData, httpBrowserFormatErr, -1);
				}
			}
		}

		SPIFFS_opendir(&fs, "/", &d);
//		os_printf("dir head\n");
		return HTTPD_CGI_MORE;
	}
	else
	{
		struct spiffs_dirent e;
		struct spiffs_dirent *pe = &e;
//		os_printf("cb\n");
		if((pe = SPIFFS_readdir(&d, pe)))
		{
			char buf[256];
			int l;
//			os_printf("file: %s",pe->name);
			l = os_sprintf(buf, "<tr><td><a href=\"/%s\">%s</a></td><td><a href=\"%s?del=%s\">Del</a></td><td>%d</td></tr>", pe->name, pe->name, connData->url, pe->name, pe->size);
			espconn_sent(connData->conn, buf, l);
			return HTTPD_CGI_MORE;
		}
		else
		{
//			os_printf("dir done\n");
			SPIFFS_closedir(&d);
			httpdSend(connData, httpBrowserStop, -1);
			return HTTPD_CGI_DONE;
		}
	}

}

int ICACHE_FLASH_ATTR fsPost(HttpdConnData *connData)
{
	char *e;

	connData->postLine[connData->postLinePos] = '\0';

	//os_printf("eval\n");
	//eval post line
	if(connData->postLine[0]=='-' && connData->postLine[1]=='-' && os_strncmp(&connData->postLine[2], connData->postBoundary, os_strlen(connData->postBoundary))==0)
	{
		os_printf("bd\n");
		if(connData->postArg !=NULL)
		{
			os_free(connData->postArg);
			connData->postArg = NULL;
		}
		//else...

		//close file write if opened
		if(connData->file != -1)
		{
			os_printf("close\n");
			fs_close(connData->file);
		}
		connData->file = -1;

		if(connData->postLine[2+os_strlen(connData->postBoundary)]=='-' && connData->postLine[2+os_strlen(connData->postBoundary)+1]=='-')
		{
			//Multipart end
			os_printf("me\n");
			//Send the response.
			connData->file = -1;
			return HTTPD_POST_DONE;
		}
	}
	else if(os_strncmp(connData->postLine, "Content-Disposition", 19)==0)
	{
		os_printf("CD\n");
		e=(char*)os_strstr(connData->postLine, "filename=");
		if(e != NULL)
		{
			//os_printf("f fn?\n");
			e += 10;
			((char*)os_strstr(e, "\""))[0] = '\0';
			if(os_strlen(e) && connData->postArg == NULL)
			{
				connData->postArg = (char*)os_malloc(os_strlen(e)+1);
				os_memcpy(connData->postArg, e, os_strlen(e) +1);
				os_printf("filename: '%s'\n", connData->postArg);
			}
		}
	}
	else if(connData->postLinePos==2 && connData->postLine[0]=='\r' && connData->postLine[1]=='\n')
	{
		os_printf("nl\n");
		if(connData->postArg != NULL && connData->file == -1)
		{
			//open file for write
			os_printf("open\n");
			connData->file = fs_open(connData->postArg, FS_RDWR|FS_CREAT|FS_TRUNC);
			if (connData->file < 0)				//wtf
			{
				return HTTPD_POST_DONE;
			}
		}
	}
	else if(connData->file != -1)
	{
		os_printf("store\n");
		if(fs_write(connData->file, connData->postLine, connData->postLinePos) != connData->postLinePos)
		{
			os_printf("write too short");
			fs_close(connData->file);
			connData->file = -1;
			return HTTPD_POST_DONE;
		}
	}
	else
	{
		//connData->postLine[connData->postLinePos] = '\0';
		os_printf("ul: %s", connData->postLine);
	}

	//connData->postLinePos = 0;
	return HTTPD_POST_MORE;
}
