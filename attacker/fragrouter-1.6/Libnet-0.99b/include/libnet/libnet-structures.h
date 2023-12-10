/*
 *  $Id: libnet-structures.h,v 1.1.1.1 1999/05/18 15:33:42 dugsong Exp $
 *
 *  libnet-structures.h - Network routine library structures header file
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

#ifndef __LIBNET_STRUCTURES_H
#define __LIBNET_STRUCTURES_H

/*
 *  prand constants
 */
#define PRAND_MAX   0xffffffff
#define PR2     0
#define PR8     1
#define PR16    2
#define PRu16   3
#define PR32    4
#define PRu32   5


/*
 *  Misc packet sizes for malloc
 */
#define PACKET      IP_H + TCP_H    /* enough for IP, UDP or TCP packet */
#define OPTS        MAX_IPOPTLEN    /* options! options! options! */
#define MAX_PACKET  IP_MAXPACKET    /* as big as ip_len can get */


/*
 *  Low level packet interface struct
 */
struct link_int
{
    int fd;             /* link layer file descriptor */
    int linktype;       /* link type */
    int linkoffset;     /* link header size (offset till network layer) */
    u_char *device;     /* device name */
};

/*
 *  temporary hack until naming scheme changes
 */


/*
 *  Arena structure.
 */
struct libnet_arena
{
    int tag;                /* arena tag */
    u_char *memory_pool;    /* the memory */
    u_long current;         /* the current amount of memory allocated */
    u_long size;            /* the size of the pool in bytes */
};


/*
 *  Interface selection stuff
 */
struct libnet_ifaddr_list
{
    u_long addr;
    char *device;
};

#endif  /* __LIBNET_STRUCTURES_H */

/* EOF */
