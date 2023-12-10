/*
 *  $Id: packet_mem.c,v 1.1.1.1 1999/05/18 15:33:42 dugsong Exp $
 *
 *  libnet
 *  packet_mem.c - Packet memory managment routines.
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

int
init_packet(size_t p_size, u_char **buf)
{
    if (p_size <= 0)
    {
        /*
         *  This is a reasonably good choice.  Aligned for your happiness.
         */
        p_size = MAX_PACKET - 3;
    }

    *buf = (u_char *)malloc(p_size);
    if (!*buf)
    {
#if (__DEBUG)
        perror("init_packet malloc failed:");
#endif
        return (-1);
    }
    else
    {
        memset(*buf, 0, p_size);
        return (1);
    }
}


void
destroy_packet(u_char **buf)
{
    assert(*buf);

    free(*buf);
    *buf = NULL;
}


int
init_packet_arena(struct libnet_arena **arena, u_short p_size, u_short p_num)
{
    u_long arena_size;
#if (__DEBUG)
    int alignment_amt;
#endif

    assert(*arena);

    if (p_num <= 0)
    {
        /* Reasonable AND sensible. */
        p_num = 10;
    }

    if (p_size <= 0)
    {
        /*
         *  This is a reasonably good choice.  Aligned for your happiness.
         */
        p_size = MAX_PACKET - 3;
    }

    arena_size = (p_size * p_num);

    /*
     *  Align the arena on a 4-byte boundary, suitable for strict architectures
     *  (such as SPARC).
     */
    while (arena_size % 4)
    {
        ++arena_size;
    }
#if (__DEBUG)
    if ((alignment_amt = (arena_size - (p_size * p_num))) != 0)
    {
        fprintf(stderr, "init_packet_arena: had to align %d byte(s)\n",
                alignment_amt);
    }
#endif

    (*arena)->memory_pool = (u_char *)malloc(arena_size);
    if (!(*arena)->memory_pool)
    {
#if (__DEBUG)
        perror("init_packet_arena malloc failed:");
#endif
        return (-1);
    }

    memset((*arena)->memory_pool, 0, arena_size);
    (*arena)->current  = 0;
    (*arena)->size     = arena_size;
    return (1);
}


u_char *
next_packet_from_arena(struct libnet_arena **arena, u_short p_size)
{
    assert(*arena);

    if (p_size <= 0)
    {
        /*
         *  This is a reasonably good choice.  Aligned for your happiness.
         */
        p_size = MAX_PACKET - 3;
    }

    /*
     *  Align the arena on a 4-byte boundary, suitable for strict architectures
     *  (such as SPARC).
     */
    while (p_size % 4)
    {
        ++p_size;
    }

    /*
     *  I'm not worried about overflow here.  If you actually have somehow
     *  allocated 4 gigs of memory, you're out of control in the first place.
     */
    if (((*arena)->current + p_size) > (*arena)->size)
    {
#if (__DEBUG)
        fprintf(stderr, "next_packet_arena ran out of memory\n");
#endif
        return (NULL);
    }

    (*arena)->current += p_size;

    return ((*arena)->memory_pool + (*arena)->current);
}


void
destroy_packet_arena(struct libnet_arena **arena)
{
    assert(*arena);

    free((*arena)->memory_pool);

    (*arena)->memory_pool = NULL;
    (*arena)->current     = -1;
    (*arena)->size        = 0;
}


/* EOF */
