/*
 *  $Id: nit.c,v 1.1.1.1 1999/05/18 15:33:42 dugsong Exp $
 *
 *  libnet
 *  nit.c - network interface tap routines
 *
 *  Copyright (c) 1998, 1999 Mike D. Schiffman <mike@infonexus.com>
 *                           route|daemon9 <route@infonexus.com>
 *  All rights reserved.
 *
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1995, 1996
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#if (HAVE_CONFIG_H)
#include "../include/config.h"
#endif
#include "../include/libnet.h"

#include "../include/gnuc.h"
#ifdef HAVE_OS_PROTO_H
#include "../include/os-proto.h"
#endif

struct link_int *
open_link_interface(char *device, char *ebuf)
{
    struct sockaddr_nit snit;
    register struct link_int *l;

    l = (struct link_int *)malloc(sizeof(*p));
    if (l == NULL)
    {
        strcpy(ebuf, ll_strerror(errno));
        return (NULL);
    }

    memset(l, 0, sizeof(*l));

    l->fd = socket(AF_NIT, SOCK_RAW, NITPROTO_RAW);
    if (l->fd < 0)
    {
        sprintf(ebuf, "socket: %s", ll_strerror(errno));
        goto bad;
    }
    snit.snit_family = AF_NIT;
    strncpy(snit.snit_ifname, device, NITIFSIZ);

    if (bind(l->fd, (struct sockaddr *)&snit, sizeof(snit)))
    {
        sprintf(ebuf, "bind: %s: %s", snit.snit_ifname, ll_strerror(errno));
        goto bad;
    }

    /*
     * NIT supports only ethernets.
     */
    l->linktype = DLT_EN10MB;

    return (l);

bad:
    if (l->fd >= 0)
    {
        close(l->fd);
    }
    free(l);
    return (NULL);
}


int
close_link_interface(struct link_int *l)
{
    return (close(l->fd));
}


int
write_link_layer(struct link_int *l, const u_char *device, u_char *buf, int len)
{
    int c;
    struct sockaddr sa;

    memset(&sa, 0, sizeof(sa));
    strncpy(sa.sa_data, device, sizeof(sa.sa_data));

    c = sendto(l->fd, buf, len, 0, &sa, sizeof(sa));
    if (c != len)
    {
#if (__DEBUG)
        fprintf(stderr, "write_link_layer: %d bytes written (%s)\n", c,
            strerror(errno));
#endif
    }
    return (c);
}

