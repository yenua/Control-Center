/*
 *  $Id: libnet.h,v 1.1.1.1 1999/05/18 15:33:41 dugsong Exp $
 *
 *  libnet.h - Network routine library header file
 *
 *  Copyright (c) 1998, 1999 Mike D. Schiffman <mike@infonexus.com>
 *                           route|daemon9 <route@infonexus.com>
 *  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef __LIBNET_H
#define __LIBNET_H

#if (__linux__)
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#ifndef __BSD_SOURCE
#define __BSD_SOURCE
#endif
#ifndef _FAVOR_BSD
#define _FAVOR_BSD
#endif
#endif  /* __linux */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#if defined(HAVE_SYS_SOCKIO_H) && !defined(SIOCGIFADDR)
#include <sys/sockio.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <net/if.h>
#if !(__linux__)
#include <netinet/ip_var.h>
#else   /* __linux__ */
#if (HAVE_NET_ETHERNET_H)
#include <net/ethernet.h>
#endif  /* HAVE_NET_ETHERNET_H */
#endif  /* __linux__ */
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#if (__linux__) && !(__GLIBC__)
/*
 *  We get multiple definitions of IGMP_AGE_THRESHOLD if we include netinet.
 */
#include <linux/igmp.h>
#else
#include <netinet/igmp.h>
#endif
#include <arpa/inet.h>
#include <sys/time.h>
#include <netdb.h>
#include <errno.h>
#include <assert.h>

#include "./libnet/libnet-headers.h"
#include "./libnet/libnet-structures.h"
#include "./libnet/libnet-macros.h"
#include "./libnet/libnet-asn1.h"
#include "./libnet/libnet-functions.h"

#define LIBNET_VERSION  "0.99b"


/*
 *  Linux specific issues.
 */
#if (__linux__)
#if (__GLIBC__ != 2)
#define uh_sport source 
#define uh_dport dest
#define uh_ulen  len
#define uh_sum   check
#if (HAVE_STRUCT_IP_CSUM)
#define ip_sum ip_csum
#endif
#endif
#define IP_MAXPACKET 65535
#endif


#endif  /* __LIBNET_H */

/* EOF */
