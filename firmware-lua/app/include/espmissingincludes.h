#ifndef ESPMISSINGINCLUIDES_H
#define ESPMISSINGINCLUIDES_H

#include <ets_sys.h>
#include "lwip/ip_addr.h"

//Missing function prototypes in include folders. Gcc will warn on these if we don't define 'em anywhere.
//MOST OF THESE ARE GUESSED! but they seem to swork and shut up the compiler.

int atoi(const char *nptr);
void ets_install_putc1(void *routine);
void ets_isr_attach(int intr, void *handler, void *arg);
void ets_isr_mask(unsigned intr);
void ets_isr_unmask(unsigned intr);
int ets_memcmp(const void *s1, const void *s2, size_t n);
void *ets_memcpy(void *dest, const void *src, size_t n);
void *ets_memset(void *s, int c, size_t n);
int ets_sprintf(char *str, const char *format, ...)  __attribute__ ((format (printf, 2, 3)));
int ets_str2macaddr(void *, void *);
int ets_strcmp(const char *s1, const char *s2);
char *ets_strcpy(char *dest, const char *src);
size_t ets_strlen(const char *s);
int ets_strncmp(const char *s1, const char *s2, int len);
char *ets_strncpy(char *dest, const char *src, size_t n);
char *ets_strstr(const char *haystack, const char *needle);
void ets_timer_arm_new(ETSTimer *a, int b, int c, int isMstimer);
void ets_timer_disarm(ETSTimer *a);
void ets_timer_setfn(ETSTimer *t, ETSTimerFunc *fn, void *parg);
int os_printf(const char *format, ...)  __attribute__ ((format (printf, 1, 2)));
int os_snprintf(char *str, size_t size, const char *format, ...) __attribute__ ((format (printf, 3, 4)));
void pvPortFree(void *ptr);
void *pvPortMalloc(size_t xWantedSize);
void *pvPortZalloc(size_t);
void uart_div_modify(int no, unsigned int freq);
void vPortFree(void *ptr);
void *vPortMalloc(size_t xWantedSize);

void ets_delay_us(unsigned int d);
int wifi_softap_set_station_info(uint8_t chaddr[16], void *ip); //struct ip_addr *ip
void ets_bzero(void *ptr, unsigned int size);

void system_station_got_ip_set(ip_addr_t *ip_addr, ip_addr_t *sn_mask, ip_addr_t *gw_addr);
void system_pp_recycle_rx_pkt(void *ptr);

uint32_t system_relative_time(uint32_t time);

#endif
