/*
 *  $Id: libnet_test.h,v 1.1.1.1 1999/05/18 15:33:42 dugsong Exp $
 *
 *  libnet_test.h
 *
 *  Copyright (c) 1998, 1999 Mike D. Schiffman <mike@infonexus.com>
 *                           route|daemon9 <route@infonexus.com>
 */

#ifndef __LIBNET_TEST_H
#define __LIBNET_TEST_H

#include "../include/libnet.h"

u_char enet_src[6] = {0x0d, 0x0e, 0x0a, 0x0d, 0x00, 0x00};
u_char enet_dst[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
u_char ip_src[4]   = {0x0a, 0x00, 0x00, 0x01};
u_char ip_dst[4]   = {0x0a, 0x00, 0x00, 0x02};

int send_arp(struct link_int *, u_char *);
int libnet_do_arp(struct link_int *, u_char *, struct ether_addr *, u_long);
int send_tcp(struct link_int *, u_char *, u_long, u_short, u_long, u_short);
int send_icmp(struct link_int *, u_char *, u_long, u_long);
void usage(u_char *);

#if (__linux__)
#define      IPOPT_SECURITY  130
#endif

#endif  /* __LIBNET_TEST_H */

/* EOF */
