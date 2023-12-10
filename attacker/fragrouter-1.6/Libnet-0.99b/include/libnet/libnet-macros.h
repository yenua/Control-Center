/*
 *  $Id: libnet-macros.h,v 1.1.1.1 1999/05/18 15:33:42 dugsong Exp $
 *
 *  libnet-macros.h - Network routine library macro header file
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

#ifndef __LIBNET_MACROS_H
#define __LIBNET_MACROS_H

/*
 *  Some BSD variants have this endianess problem.
 */
#if (BSD_BYTE_SWAP || LIBNET_BSD_BYTE_SWAP)
#define FIX(n)      ntohs(n)
#define UNFIX(n)    htons(n)
#else
#define FIX(n)      (n)
#define UNFIX(n)    (n)
#endif

#define LIBNET_GET_ARENA_SIZE(a)               (a.size)
#define LIBNET_GET_ARENA_REMAINING_BYTES(a)    (a.size - a.current)

/*
 *  Not all systems have IFF_LOOPBACK
 *  Used by if_addr.c
 */
#ifdef IFF_LOOPBACK
#define ISLOOPBACK(p) ((p)->ifr_flags & IFF_LOOPBACK)
#else
#define ISLOOPBACK(p) (strcmp((p)->ifr_name, "lo0") == 0)
#endif

#define LIBNET_PRINT_ETH_ADDR(e) \
{ \
    int i = 0; \
    for (i = 0; i < 6; i++) \
    { \
        printf("%x", e.ether_addr_octet[i]); \
        if (i != 5) \
        { \
            printf(":"); \
        } \
    } \
    printf("\n"); \
}

#endif  /* __LIBNET_MACROS_H */

/* EOF */
