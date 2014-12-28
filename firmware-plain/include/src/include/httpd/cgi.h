#ifndef CGI_H
#define CGI_H

#include "httpd.h"

typedef struct
{
	const char *url;
	const char *name;
} HttpdBuitInPrograms;

//int cgiLed(HttpdConnData *connData);
//void tplLed(HttpdConnData *connData, char *token, void **arg);
//int cgiReadFlash(HttpdConnData *connData);

void tplMain(HttpdConnData *connData, char *token, void **arg);
int cgiSteady(HttpdConnData *connData);
int cgiRefresh(HttpdConnData *connData);
int cgiRedirectProgram(HttpdConnData *connData);

#endif
