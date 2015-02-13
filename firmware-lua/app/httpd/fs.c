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

struct EspFsFile {
//	EspFsHeader *header;
//	char decompressor;
//	int32_t posDecomp;
//	char *posStart;
//	char *posComp;
//	void *decompData;
};


//This is a catch-all cgi function. It takes the url passed to it, looks up the corresponding
//path in the filesystem and if it exists, passes the file through. This simulates what a normal
//webserver would do with static files.
int ICACHE_FLASH_ATTR fsHook(HttpdConnData *connData)
{
//	EspFsFile *file=connData->cgiData;
	int len;
	char buff[1024];

	if (connData->conn==NULL)
	{
		//Connection aborted. Clean up.
//		espFsClose(file);
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
