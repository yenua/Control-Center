/*
 *  $Id: tcp+data.c,v 1.1.1.1 1999/05/18 15:33:42 dugsong Exp $
 *
 *  libnet
 *  TCP Packet assembly tester (with payload)
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
#include "../../include/config.h"
#endif
#include "../libnet_test.h"

int
main(int argc, char **argv)
{
    int sock, c;
    u_long src_ip, dst_ip;
    u_short src_prt, dst_prt;
    u_char *cp, *buf; 
    u_char *payload = "hello world";
    int payload_s = strlen(payload);

    printf("TCP + payload packet building/writing test\n");

    src_ip  = 0;
    dst_ip  = 0;
    src_prt = 0;
    dst_prt = 0;
    while((c = getopt(argc, argv, "d:s:")) != EOF)
    {
        switch (c)
        {
            /*
             *  We expect the input to be of the form `ip.ip.ip.ip.port`.  We
             *  point cp to the last dot of the IP address/port string and
             *  then seperate them with a NULL byte.  The optarg now points to
             *  just the IP address, and cp points to the port.
             */
            case 'd':
                if (!(cp = strrchr(optarg, '.')))
                {
                    usage(argv[0]);
                }
                *cp++ = 0;
                dst_prt = (u_short)atoi(cp);
                if (!(dst_ip = name_resolve(optarg, 1)))
                {
                    fprintf(stderr, "Bad destination IP address: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
                if (!(cp = strrchr(optarg, '.')))
                {
                    usage(argv[0]);
                }
                *cp++ = 0;
                src_prt = (u_short)atoi(cp);
                if (!(src_ip = name_resolve(optarg, 1)))
                {
                    fprintf(stderr, "Bad source IP address: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
        }
    }

    if (!src_ip || !src_prt || !dst_ip || !dst_prt)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     *  Get our block of memory for the packet.  This time, we need memory
     *  for the packet headers and payload.
     */
    buf = malloc(TCP_H + IP_H + payload_s);
    if (!buf)
    {
        perror("No memory for packet");
        exit(EXIT_FAILURE);
    }

    /*
     *  Open our raw IP socket and set IP_HDRINCL.
     */
    sock = open_raw_sock(IPPROTO_RAW);
    if (sock == -1)
    {
        perror("No socket");
        exit(EXIT_FAILURE);
    }
    
    /*
     *  Build the IP header (shown exploded for commenting).
     */
    build_ip(TCP_H + payload_s,             /* Size of the payload */
            0,                              /* IP tos */
            242,                            /* IP ID */
            0,                              /* Frag stuff */
            48,                             /* TTL */
            IPPROTO_TCP,                    /* Transport protocol */
            src_ip,                         /* Source IP */
            dst_ip,                         /* Destination IP */
            NULL,                           /* Pointer to payload (none) */
            0,
            buf);                           /* Packet header memory */

    /*
     *  Build the TCP header.
     */
    build_tcp(src_prt,                      /* Source TCP port */
            dst_prt,                        /* Destination TCP port */
            11111,                          /* Sequence number */
            99999,                          /* Acknowledgement number */
            TH_SYN|TH_ACK,                  /* Control flags */
            1024,                           /* Window size */
            0,                              /* Urgent pointer */
            payload,                        /* Pointer to payload */
            payload_s,                      /* Payload size */
            buf + IP_H);                    /* Packet header memory */

    /*
     *  Calculate the TCP header checksum (IP header checksum is *always* done
     *  by the kernel.
     */
    do_checksum(buf, IPPROTO_TCP, TCP_H + payload_s);

    /*
     *  Write the packet to the network.
     */
    c = write_ip(sock, buf, TCP_H + IP_H + payload_s);
    if (c < TCP_H + IP_H + payload_s)
    {
        fprintf(stderr, "write_ip\n");
    }
    printf("Completed, wrote %d bytes\n", c);
    free(buf);

    return (c == -1 ? EXIT_FAILURE : EXIT_SUCCESS);
}


void
usage(u_char *name)
{
    fprintf(stderr,
        "usage: %s -s source_ip:source_port -d destination_ip:destination_port\n",
        name);
}

/* EOF */
