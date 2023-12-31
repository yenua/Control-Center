/*
 *  $Id: build_rip.c,v 1.1.1.1 1999/05/18 15:33:42 dugsong Exp $
 *
 *  libnet
 *  build_rip.c - RIP packet assembler
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
build_rip(u_char cmd, u_char ver, u_short rd, u_short af, u_short rt,
        u_long addr, u_long mask, u_long next_hop, u_long metric,
        const u_char *payload, int payload_s, u_char *buf)
{
    struct libnet_rip_hdr rip_hdr;

    assert(buf);

    rip_hdr.cmd      = cmd;
    rip_hdr.ver      = ver;
    rip_hdr.rd       = htons(rd);
    rip_hdr.af       = htons(af);
    rip_hdr.rt       = htons(rt);
    rip_hdr.addr     = htonl(addr);
    rip_hdr.mask     = htonl(mask);
    rip_hdr.next_hop = htonl(next_hop);
    rip_hdr.metric   = htonl(metric);

    if (payload && payload_s)
    {
        /*
         *  Unchecked runtime error for buf + RIP_H payload to be greater than
         *  the allocated heap memory.
         */
        memcpy(buf + RIP_H, payload, payload_s);
    }
    memcpy(buf, &rip_hdr, sizeof(rip_hdr));
}


/* EOF */
