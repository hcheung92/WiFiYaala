/*
 * httpd.h
 *
 *  Created on: Feb 12, 2015
 *      Author: sid
 */

#ifndef HTTPD_H_
#define HTTPD_H_

#define HTTPDVER "0.3"

#define HTTPD_CGI_MORE 0
#define HTTPD_CGI_DONE 1
#define HTTPD_CGI_NOTFOUND 2

#define HTTPD_POST_MORE 0
#define HTTPD_POST_DONE 1

typedef struct HttpdPriv HttpdPriv;
typedef struct HttpdConnData HttpdConnData;

typedef int (* httpdCallback)(HttpdConnData *connData);

//A struct describing a http connection. This gets passed to cgi functions.
struct HttpdConnData {
	struct espconn *conn;
	char *url;
	char *getArgs;
	const void *cgiArg;
	//void *cgiData;
	int file;
	HttpdPriv *priv;
	httpdCallback sendCb;
	httpdCallback postCb;
	int postLen;
	char *postBoundary;
	int postLinePos;
	char *postLine;
	char *postArg;
};

//A struct describing an url. This is the main struct that's used to send different URL requests to
//different routines.
typedef struct {
	const char *url;
	httpdCallback httpdCb;
	httpdCallback postCb;
} HttpdBuiltInUrl;

void ICACHE_FLASH_ATTR httpdInit(int port);



int ICACHE_FLASH_ATTR redirIndexHtml(HttpdConnData *conn);

#endif /* HTTPD_H_ */
