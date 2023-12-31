/*
 *  $Id: build_ip.c,v 1.1.1.1 1999/05/18 15:33:42 dugsong Exp $
 *
 *  libnet
 *  build_ip.c - IP packet assembler
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

#if (HAVE_CONFIG_H)
#include "../include/config.h"
#endif
#include "../include/libnet.h"

void
build_ip(u_short len, u_char tos, u_short id, u_short frag, u_char ttl,
        u_char prot, u_long src, u_long dst, const u_char *payload,
        int payload_s, u_char *buf)
{
    struct ip ip_hdr;

    assert(buf);

    ip_hdr.ip_v    = 4;                            /* version 4 */
    ip_hdr.ip_hl   = 5;                            /* 20 byte header */
    ip_hdr.ip_tos  = tos;                          /* IP tos */
    ip_hdr.ip_len  = htons(IP_H + len);            /* total length */
    ip_hdr.ip_id   = htons(id);                    /* IP ID */
    ip_hdr.ip_off  = htons(frag);                  /* fragmentation flags */
    ip_hdr.ip_ttl  = ttl;                          /* time to live */
    ip_hdr.ip_p    = prot;                         /* transport protocol */
    ip_hdr.ip_sum  = 0;                            /* do this later */
    ip_hdr.ip_src.s_addr = src;
    ip_hdr.ip_dst.s_addr = dst;
    if (payload && payload_s)
    {
        /*
         *  Unchecked runtime error for buf + IP_H + payload to be greater than
         *  the allocated heap memory.
         */
        memcpy(buf + IP_H, payload, payload_s);
    }
    memcpy((u_char *)buf, (u_char *)&ip_hdr, sizeof(ip_hdr));
}


/* EOF */
