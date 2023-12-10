/*
 *  $Id: libnet-functions.h,v 1.2 1999/05/27 02:32:18 dugsong Exp $
 *
 *  libnet-functions.h - Network routine library function prototype header file
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

#ifndef __LIBNET_FUNCTIONS_H
#define __LIBNET_FUNCTIONS_H

/*
 *  Seeds the pseudorandom number generator with gettimeofday.
 */

int
seed_prand();


/*
 *  Returns a psuedorandom positive integer.
 */

u_long
get_prand(
    int                 /* One of the PR* constants */
    );


/*
 *  Calculates IPv4 family checksum on packet headers.
 */

int                     /* 1 on success, -1 on failure */
do_checksum(
    u_char *,           /* Pointer to the packet buffer */
    int,                /* Protocol */
    int                 /* Packet size */
    );


/*
 *  Network byte order into IP address
 *  Previous versions had a memory leak (returned a strdup'd pointer -- strdup
 *  has an implicit malloc which wasn't getting freed).  This static var hack
 *  thingy was used to preserve existing code without having to change much.
 *  You can simply use the return value of the function directly allowing you
 *  to write tighter, more obvious code (rather then having to do allocate an
 *  additional buffer for the output).
 *  Thanks to Red for the idea.
 */

u_char *                /* Pointer to hostname or dotted decimal IP address */
host_lookup(
    u_long,             /* Network byte ordered (big endian) IP address */
    u_short             /* Use domain names or no */
    );


/*
 *  Network byte order into IP address
 *  Threadsafe version.
 */

void
host_lookup_r(
    u_long,             /* Network byte ordered (big endian) IP address */
    u_short,            /* Use domain names or no */
    u_char *            /* Pointer to hostname or dotted decimal IP address */
    );


/*
 *  IP address into network byte order
 */

u_long                  /* Network byte ordered IP address or -1 on error */
name_resolve(
    u_char *,           /* Pointer the hostname or dotted decimal IP address */
    u_short             /* Use domain names or no */
    );


/*
 *  Fast x86 TCP checksum.  This function builds it's own psuedoheader.
 */
 
u_short                 /* Standard TCP checksum of header and data */
tcp_check(
    struct tcphdr *,    /* TCP header pointer */
    int,                /* Packet length */
    u_long,             /* Source IP address */
    u_long              /* Destination IP address */
    );


/*
 *  Fast x86 IP checksum.
 */

u_short                 /* Standard IP checksum of header and data */
ip_check(
    u_short *,          /* Pointer to the buffer to be summed */
    int                 /* Packet length */
    );


/*
 *  Opens a socket for writing raw IP datagrams to.  Set IP_HDRINCL to let the 
 *  kernel know we've got it all under control.
 */

int                     /* Opened file desciptor, or -1 on error */
open_raw_sock(
    int                 /* Protocol of raw socket (from /etc/protocols) */
    );


int                     /* 1 upon success, or -1 on error */
close_raw_sock(
    int                 /* File descriptor */
    );


int
libnet_select_device(
    struct sockaddr_in *sin,
    u_char **device,
    u_char *ebuf
    );

/*
 *  Ethernet packet assembler.
 */

void
build_ethernet(
    u_char *,           /* Pointer to a 6 byte ethernet address */
    u_char *,           /* Pointer to a 6 byte ethernet address */
    u_short,            /* Packet IP type */
    const u_char *,     /* Payload (or NULL) */
    int,                /* Payload size */
    u_char *            /* Packet header buffer */
    );


/*
 *  ARP packet assembler.
 */

void
build_arp(
    u_short,            /* hardware address type */
    u_short,            /* protocol address type */
    u_char,             /* hardware address length */
    u_char,             /* protocol address length */
    u_short,            /* ARP operation type */
    u_char *,           /* sender hardware address */
    u_char *,           /* sender protocol address */
    u_char *,           /* target hardware address */
    u_char *,           /* target protocol address */
    const u_char *,     /* payload or NULL if none */
    int,                /* payload length */
    u_char *            /* packet buffer memory */
    );

/*
 *  TCP packet assembler.
 */

void
build_tcp(
    u_short,            /* Source port */
    u_short,            /* Destination port */
    u_long,             /* Sequence Number */
    u_long,             /* Acknowledgement Number */
    u_char,             /* Control bits */
    u_short,            /* Advertised Window Size */
    u_short,            /* Urgent Pointer */
    const u_char *,     /* Pointer to packet data (or NULL) */
    int,                /* Packet payload size */
    u_char *            /* Pointer to packet header memory */
    );


/*
 * UDP packet assembler.
 */

void
build_udp(
    u_short,            /* Source port */
    u_short,            /* Destination port */
    const u_char *,     /* Pointer to packet data (or NULL) */
    int,                /* Packet payload size */
    u_char *            /* Pointer to packet header memory */
    );

/*
 *  ICMP_ECHO packet assembler.
 */

void
build_icmp_echo(
    u_char,             /* icmp type */
    u_char,             /* icmp code */
    u_short,            /* id */
    u_short,            /* sequence number */
    const u_char *,     /* Pointer to packet data (or NULL) */
    int,                /* Packet payload size */
    u_char *            /* Pointer to packet header memory */
    );

/*
 *  ICMP_MASK packet assembler.
 */

void
build_icmp_mask(
    u_char,             /* icmp type */
    u_char,             /* icmp code */
    u_short,            /* id */
    u_short,            /* sequence number */
    u_long,             /* address mask */
    const u_char *,     /* Pointer to packet data (or NULL) */
    int,                /* Packet payload size */
    u_char *            /* Pointer to packet header memory */
    );


/*
 *  ICMP_UNREACH packet assembler.
 */

void
build_icmp_unreach(
    u_char,             /* icmp type */
    u_char,             /* icmp code */
    u_short,            /* Original Length of packet data */
    u_char,             /* Original IP tos */
    u_short,            /* Original IP ID */
    u_short,            /* Original Fragmentation flags and offset */
    u_char,             /* Original TTL */
    u_char,             /* Original Protocol */
    u_long,             /* Original Source IP Address */
    u_long,             /* Original Destination IP Address */
    const u_char *,     /* Pointer to original packet data (or NULL) */
    int,                /* Packet payload size (or 0) */
    u_char *            /* Pointer to packet header memory */
    );


/*
 *  ICMP_UNREACH packet assembler.
 */

void
build_icmp_timeexceed(
    u_char,             /* icmp type */
    u_char,             /* icmp code */
    u_short,            /* Original Length of packet data */
    u_char,             /* Original IP tos */
    u_short,            /* Original IP ID */
    u_short,            /* Original Fragmentation flags and offset */
    u_char,             /* Original TTL */
    u_char,             /* Original Protocol */
    u_long,             /* Original Source IP Address */
    u_long,             /* Original Destination IP Address */
    const u_char *,     /* Pointer to original packet data (or NULL) */
    int,                /* Packet payload size (or 0) */
    u_char *            /* Pointer to packet header memory */
    );

/*
 *  ICMP_TIMESTAMP packet assembler.
 */

void
build_icmp_timestamp(
    u_char,             /* icmp type */
    u_char,             /* icmp code */
    u_short,            /* id */
    u_short,            /* sequence number */
    n_time,             /* original timestamp */
    n_time,             /* receive timestamp */
    n_time,             /* transmit timestamp */
    const u_char *,     /* Pointer to packet data (or NULL) */
    int,                /* Packet payload size */
    u_char *            /* Pointer to packet header memory */
    );

/*
 *  IGMP packet builder.
 */

void
build_igmp(
    u_char,             /* igmp type */
    u_char,             /* igmp code */
    u_long,             /* ip addr */
    u_char *            /* Pointer to packet header memory */
    );


/*
 *  IP packet assembler.
 */

void
build_ip(
    u_short,            /* Length of packet data */
    u_char,             /* IP tos */
    u_short,            /* IP ID */
    u_short,            /* Fragmentation flags and offset */
    u_char,             /* TTL */
    u_char,             /* Protocol */
    u_long,             /* Source IP Address */
    u_long,             /* Destination IP Address */
    const u_char *,     /* Pointer to packet data (or NULL) */
    int,                /* Packet payload size */
    u_char *            /* Pointer to packet header memory */
    );


void
build_dns(
    u_short,            /* Packet ID */
    u_short,            /* Flags */
    u_short,            /* Number of questions */
    u_short,            /* Number of answer resource records */
    u_short,            /* Number of authority resource records */
    u_short,            /* Number of additional resource records */
    const u_char *,     /* Payload (or NULL) */
    int,                /* Payload size */
    u_char *            /* Header memory */
    );


void
build_rip(
    u_char,             /* Command */
    u_char,             /* Version */
    u_short,            /* Zero (v1) or Routing Domain (v2) */
    u_short,            /* Address family */
    u_short,            /* Zero (v1) or Route Tag (v2) */
    u_long,             /* IP address */
    u_long,             /* Zero (v1) or Subnet Mask (v2) */
    u_long,             /* Zero (v1) or Next hop IP address (v2) */
    u_long,             /* Metric */
    const u_char *,     /* Payload (or NULL) */
    int,                /* Payload size */
    u_char *            /* Header memory */
    );


/*
 *  Insert IP options to a prebuilt IP packet.
 */

int                     /* 1 on success, -1 on failure */
insert_ipo(
    struct ipoption *,  /* Pointer to the ip options structure */ 
    u_char,             /* IP option list size */
    u_char *            /* Pointer to packet buf */
    );

/*
 *  Insert TCP options to a prebuilt IP packet.
 */

int                     /* 1 on success, -1 on failure */
insert_tcpo(
    struct tcpoption *, /* Pointer to the tcp options structure */ 
    u_char,             /* TCP option list size */
    u_char *            /* Pointer to packet buf */
    );

/*
 *  Writes a prebuild IP packet to the network with a supplied raw socket.
 *  To write a link layer packet, use the write_link_layer function.
 */

int                     /* number of bytes written if successful, -1 on error */
write_ip(
    int sock,           /* Previously opened raw socket */
    u_char *,           /* Pointer a complete IP datagram */
    int                 /* Packet size */
    );

/*
 *  Writes a prebuild IP/ethernet packet to the network with a supplied
 *  link_layer interface.  To write just an IP packet, use the write_link_layer
 *  function.
 */

int                     /* number of bytes written if successful, -1 on error */
write_link_layer(
    struct link_int *,  /* Pointer to a link interface structure */
    const u_char *,     /* Pointer to the device */
    u_char *,           /* Pointer the u_char buf (the packet)to be written */
    int                 /* Packet length */
    );


/*
 *  Opens a link layer interface.  Analogous to open_raw_sock.
 */

struct link_int *       /* Pointer to a link layer interface struct */
open_link_interface(
    char *,             /* Device name */
    char *              /* Error buffer */
    );


int                     /* 1 on success, -1 on failure */
close_link_interface(
    struct link_int *   /* Pointer to a link layer interface struct */
    );


char *                  /* String error message */
ll_strerror(
    int                 /* Errno */
    );


/*
 *  Returns the IP address of the interface.
 */

u_long                  /* 0 upon error, address upon success */
get_ipaddr(
    struct link_int *,  /* Pointer to a link interface structure */
    const u_char *,     /* Device */
    char *              /* Error buf */
    );


/*
 *  Returns the MAC address of the interface.
 */

struct ether_addr *     /* 0 upon error, address upon success */
get_hwaddr(
    struct link_int *,  /* Pointer to a link interface structure */
    const u_char *,     /* Device */
    char *              /* Error buf */
    );


/*
 *  Simple interface for initializing a packet.
 *  Basically a malloc wrapper.  
 */

int                         /* -1 on error, 1 on ok */
init_packet(
    size_t,                 /* 0 and we make a good guess, otherwise you choose. */
    u_char **               /* Pointer to the pointer to the packet */
    );      


/*
 *  Simple interface for destoying a packet.
 *  Don't call this without a corresponding call to init_packet() first.
 */

void
destroy_packet(
    u_char **               /* Pointer to the packet addr. */
    );


/*
 *  Memory pool initialization routine.
 */

int
init_packet_arena(
    struct libnet_arena **, /* Pointer to an arena pointer */
    u_short,
    u_short
    );


/*
 *  Returns the next chunk of memory from the pool.
 */

u_char *
next_packet_from_arena(
    struct libnet_arena **, /* Pointer to an arena pointer */
    u_short
    );


/*
 *  Memory pool destructor routine.
 */

void
destroy_packet_arena(
    struct libnet_arena **  /* Pointer to an arena pointer */
    );


/* 
 *  More or less taken from tcpdump code.
 */

void
libnet_hex_dump(
    u_char *,               /* Packet to be dumped */
    int,                    /* Packet size (in bytes */
    int,                    /* To swap or not to swap */
    FILE *                  /* Stream pointer to dump to */
    );

#if (RAW_IS_COOKED)
/*
 * Gets IP of the local interface corresponding to a destination IP.
 */

u_long
libnet_get_interface_by_dest(
    u_long ,               /* Destination IP */
    char *                 /* Error buffer */
    );

/*
 *  Writes a prebuilt IP packet to the network using the link layer API.
 */

int                     /* number of bytes written if successful, -1 on error */
write_ip_via_datalink(
    u_char *,           /* Pointer to complete IP datagram */
    int                 /* Packet size */
    );

/*
 *  Selects a device given an IP that corresponds to it.
 */

int
libnet_select_device_by_ip(
    u_char **,          /* Pointer to character * to place device name */
    u_long ,            /* IP address of desired interface */
    u_char *            /* Error buffer */
    );

/*
 *  Queries the ARP cache for an IP address resolution.
 */

int
libnet_query_arp(
    u_long,             /* IP address */
    char *,             /* pointer to place to put hardware address */
    char *              /* Error buffer */
    );

int
libnet_active_query_arp(
    u_long,             /* IP address */
    char *,             /* pointer to place to put hardware address */
    u_char *,           /* device to query on */
    char *              /* Error buffer */
    );

u_long
libnet_get_next_hop(
    u_long ,            /* Destination IP */
    char *              /* Error buffer */
    );
#endif /* RAW_IS_COOKED */

/* STUB MACROS / FUNCTION PROTOTYPES */

int
libnet_seed_prand();

u_long
libnet_get_prand(int);

int
libnet_do_checksum(u_char *, int, int);

u_short
libnet_tcp_check(struct tcphdr *, int, u_long, u_long);

u_short
libnet_ip_check(u_short *, int);


u_char *
libnet_host_lookup(u_long, u_short);

void
libnet_host_lookup_r(u_long, u_short, u_char *);

u_long
libnet_name_resolve(u_char *, u_short);

u_short
libnet_tcp_check(struct tcphdr *, int, u_long, u_long);

u_short
libnet_ip_check(u_short *, int);

int
libnet_open_raw_sock(int);

int
libnet_close_raw_sock(int);

void
libnet_build_ethernet(u_char *, u_char *, u_short, const u_char *, int,
        u_char *);

void
libnet_build_arp(u_short, u_short, u_char, u_char, u_short, u_char *,
        u_char *, u_char *, u_char *, const u_char *, int, u_char *);

void
libnet_build_tcp(u_short, u_short, u_long, u_long, u_char, u_short, u_short,
         const u_char *, int, u_char *);

void
libnet_build_udp(u_short, u_short, const u_char *, int, u_char *);

void
libnet_build_icmp_echo(u_char, u_char, u_short, u_short, const u_char *, int,
        u_char *);

void
libnet_build_icmp_mask(u_char,u_char, u_short, u_short, u_long,
        const u_char *, int, u_char *);

void
libnet_build_icmp_unreach(u_char, u_char, u_short, u_char, u_short, u_short,
        u_char, u_char, u_long, u_long, const u_char *, int, u_char *);

void
libnet_build_icmp_timeexceed(u_char, u_char, u_short, u_char, u_short, u_short, 
        u_char, u_char, u_long, u_long, const u_char *, int, u_char *);

void
libnet_build_icmp_timestamp(u_char, u_char, u_short, u_short, n_time, n_time,
        n_time, const u_char *, int, u_char *);

void
libnet_build_igmp(u_char, u_char, u_long, u_char *);

void
libnet_build_ip(u_short, u_char, u_short, u_short, u_char, u_char, u_long,
        u_long, const u_char *, int, u_char *);

void
libnet_build_dns(u_short, u_short, u_short, u_short, u_short, u_short,
        const u_char *, int, u_char *);

void
libnet_build_rip(u_char, u_char, u_short, u_short, u_short, u_long, u_long,
        u_long, u_long, const u_char *, int, u_char *);

int
libnet_insert_ipo(struct ipoption *, u_char, u_char *);

int
libnet_insert_tcpo(struct tcpoption *, u_char, u_char *);

int
libnet_write_ip(int sock, u_char *, int);

int
libnet_write_link_layer(struct link_int *, const u_char *, u_char *, int);

struct link_int *
libnet_open_link_interface(char *, char *);

int
libnet_close_link_interface(struct link_int *);

char *
libnet_ll_strerror(int);

u_long
libnet_get_ipaddr(struct link_int *, const u_char *, u_char *);

struct ether_addr *
libnet_get_hwaddr(struct link_int *, const u_char *, u_char *);

int
libnet_init_packet(size_t, u_char **);      

void
libnet_destroy_packet(u_char **);

int
libnet_init_packet_arena(struct libnet_arena **, u_short, u_short);

u_char *
libnet_next_packet_from_arena(struct libnet_arena **, u_short);

void
libnet_destroy_packet_arena(struct libnet_arena **);

#endif  /* __LIBNET_FUNCTIONS_H */

/* EOF */
