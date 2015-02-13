/*
 * httpd.h
 *
 *  Created on: Feb 12, 2015
 *      Author: sid
 */

#ifndef HTTPD_H_
#define HTTPD_H_

#define HTTPDVER "0.2"

#define HTTPD_CGI_MORE 0
#define HTTPD_CGI_DONE 1
#define HTTPD_CGI_NOTFOUND 2		//todo remove

typedef struct HttpdPriv HttpdPriv;
typedef struct HttpdConnData HttpdConnData;

typedef int (* httpdSendCallback)(HttpdConnData *connData);

//A struct describing a http connection. This gets passed to cgi functions.
struct HttpdConnData {
	struct espconn *conn;
	char *url;
	char *getArgs;
	const void *cgiArg;
	//void *cgiData;
	int file;
	HttpdPriv *priv;
	httpdSendCallback cb;			//former cgi
	int postLen;
	char *postBuff;
};

//A struct describing an url. This is the main struct that's used to send different URL requests to
//different routines.
typedef struct {
	const char *url;
	httpdSendCallback httpdCb;
} HttpdBuiltInUrl;

void ICACHE_FLASH_ATTR httpdInit(int port);

#endif /* HTTPD_H_ */
