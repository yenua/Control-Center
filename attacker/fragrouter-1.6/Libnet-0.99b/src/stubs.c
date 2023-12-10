/*
 *  $Id: stubs.c,v 1.2 1999/05/27 02:32:19 dugsong Exp $
 *
 *  libnet
 *  stubs.c - planned function cohesion renaming scheme stubs
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
libnet_init_packet(size_t p_size, u_char **buf)
{
    return (init_packet(p_size, buf));
}


void
libnet_destroy_packet(u_char **buf)
{
    destroy_packet(buf);
}


int
libnet_init_packet_arena(struct libnet_arena **arena, u_short p_num, 
        u_short p_size)
{
    return (init_packet_arena(arena, p_num, p_size));
}


u_char *
libnet_next_packet_from_arena(struct libnet_arena **arena, u_short p_size)
{
    return (next_packet_from_arena(arena, p_size));
}


void
libnet_destroy_packet_arena(struct libnet_arena **arena)
{
    destroy_packet_arena(arena);
}


u_char *
libnet_host_lookup(u_long in, u_short use_name)
{
    return (host_lookup(in, use_name));
}


void
libnet_host_lookup_r(u_long in, u_short use_name, u_char *buf)
{
    host_lookup_r(in, use_name, buf);
}


u_long
libnet_name_resolve(u_char *hostname, u_short use_name)
{
    return (name_resolve(hostname, use_name));
}


u_long
libnet_get_ipaddr(struct link_int *l, const u_char *device, u_char *buf)
{
    return (get_ipaddr(l, (const u_char *)device, buf));
}


struct ether_addr *
libnet_get_hwaddr(struct link_int *l, const u_char *device, u_char *buf)
{
    return (get_hwaddr(l, (const u_char *)device, buf));
}

       
int
libnet_open_raw_sock(int protocol)
{
    return (open_raw_sock(protocol));
}


int
libnet_close_raw_sock(int fd)
{
    return (close_raw_sock(fd));
}


struct link_int *
libnet_open_link_interface(char *device, char *ebuf)
{
    return (open_link_interface(device, ebuf));
}


int
libnet_close_link_interface(struct link_int *l)
{
    return (close_link_interface(l));
}


int
libnet_write_ip(int sock, u_char *packet, int len)
{
    return (write_ip(sock, packet, len));
}
       

int
libnet_write_link_layer(struct link_int *l, const u_char *device, u_char *buf,
        int len)
{
    return (write_link_layer(l, (const u_char *)device, buf, len));
}
       

int
libnet_do_checksum(u_char *buf, int protocol, int len)
{
    return (do_checksum(buf, protocol, len));
}


u_short
libnet_ip_check(u_short *buf, int len)
{
    return (libnet_ip_check(buf, len));
}


void
libnet_build_arp(u_short hrd, u_short pro, u_char hln, u_char pln,
        u_short op, u_char *sha, u_char *spa, u_char *tha, u_char *tpa,
        const u_char *payload, int payload_s, u_char *buf)
{
    build_arp(hrd, pro, hln, pln, op, sha, spa, tha, tpa, payload, payload_s,
        buf);
}


void
libnet_build_dns(u_short id, u_short flags, u_short num_q, u_short num_anws_rr,
        u_short num_auth_rr, u_short num_addi_rr, const u_char *payload,
        int payload_s, u_char *buf)
{
    build_dns(id, flags, num_q, num_anws_rr, num_auth_rr, num_addi_rr, payload,
        payload_s, buf);
}


void
libnet_build_ethernet(u_char *daddr, u_char *saddr, u_short id,
        const u_char *payload, int payload_s, u_char *buf)
{
    build_ethernet(daddr, saddr, id, payload, payload_s, buf);
}


void
libnet_build_icmp_echo(u_char type, u_char code, u_short id, u_short seq,
        const u_char *payload, int payload_s, u_char *buf)
{
    build_icmp_echo(type, code, id, seq, payload, payload_s, buf);
}


void
libnet_build_icmp_mask(u_char type, u_char code, u_short id, u_short seq,
        u_long mask, const u_char *payload, int payload_s, u_char *buf)
{
    build_icmp_mask(type, code, id, seq, mask, payload, payload_s, buf);
}


void
libnet_build_icmp_unreach(u_char type, u_char code, u_short orig_len,
        u_char orig_tos, u_short orig_id, u_short orig_frag, u_char orig_ttl,
        u_char orig_prot, u_long orig_src, u_long orig_dst,
        const u_char *orig_payload, int payload_s, u_char *buf)
{
    build_icmp_unreach(type, code, orig_len, orig_tos, orig_id, orig_frag,
        orig_ttl, orig_prot, orig_src, orig_dst, orig_payload, payload_s, buf);
}
       

void
libnet_build_icmp_timeexceed(u_char type, u_char code, u_short orig_len,
        u_char orig_tos, u_short orig_id, u_short orig_frag, u_char orig_ttl,
        u_char orig_prot, u_long orig_src, u_long orig_dst,
        const u_char *orig_payload, int payload_s, u_char *buf)
{
    build_icmp_timeexceed(type, code, orig_len, orig_tos, orig_id, orig_frag,
        orig_ttl, orig_prot, orig_src, orig_dst, orig_payload, payload_s, buf);
}


void
libnet_build_icmp_timestamp(u_char type, u_char code, u_short id, u_short seq,
        n_time otime, n_time rtime, n_time ttime, const u_char *payload,
        int payload_s, u_char *buf)
{
    build_icmp_timestamp(type, code, id, seq, otime, rtime, ttime, payload,
        payload_s, buf);
}


void
libnet_build_igmp(u_char type, u_char code, u_long ip, u_char *buf)
{
    build_igmp(type, code, ip, buf);
}

 
void
libnet_build_ip(u_short len, u_char tos, u_short id, u_short frag, u_char ttl,
        u_char prot, u_long saddr, u_long daddr, const u_char *payload,
        int payload_s, u_char *buf)
{
    build_ip(len, tos, id, frag, ttl, prot, saddr, daddr, payload, payload_s,
        buf);
}


void
libnet_build_rip(u_char command, u_char ver, u_short rd, u_short af,
        u_short rt, u_long addr, u_long mask, u_long next_hop, u_long metric,
        const u_char *payload, int payload_s, u_char *buf)
{
    build_rip(command, ver, rd, af, rt, addr, mask, next_hop, metric, payload,
        payload_s, buf);
}


void
libnet_build_tcp(u_short sport, u_short dport, u_long seq, u_long ack,
        u_char control, u_short win, u_short urg, const u_char *payload,
        int payload_s, u_char *buf)
{
    build_tcp(sport, dport, seq, ack, control, win, urg, payload, payload_s,
        buf);
}


void
libnet_build_udp(u_short sport, u_short dport, const u_char *payload,
                   int payload_s, u_char *buf)
{
    build_udp(sport, dport,payload, payload_s, buf);
}


int
libnet_insert_ipo(struct ipoption *opt, u_char opt_len, u_char *buf)
{
    return (insert_ipo(opt, opt_len, buf));
}


int
libnet_insert_tcpo(struct tcpoption *opt, u_char opt_len, u_char *buf)
{
    return (insert_tcpo(opt, opt_len, buf));
}


int
libnet_seed_prand()
{
    return (seed_prand());
}


u_long
libnet_get_prand(int type)
{
    return (get_prand(type));
}


u_char *
libnet_build_asn1_int(u_char *data, int *datalen, u_char type, long *int_p,
        int int_s)
{
    return (build_asn1_int(data, datalen, type, int_p, int_s));
}


u_char *
libnet_build_asn1_uint(u_char *data, int *datalen, u_char type, u_long *int_p,
        int int_s)
{
    return (build_asn1_uint(data, datalen, type, int_p, int_s));
}


u_char *
libnet_build_asn1_string(u_char *data, int *datalen, u_char type,
    u_char *string, int str_s)
{
    return (build_asn1_string(data, datalen, type, string, str_s));
}


u_char *
libnet_build_asn1_header(u_char *data, int *datalen, u_char type, int len)
{
    return (build_asn1_header(data, datalen, type, len));
}


u_char *
libnet_build_asn1_length(u_char *data, int *datalen, int len)
{
    return (build_asn1_length(data, datalen, len));
}


u_char *
libnet_build_asn1_sequence(u_char *data, int *datalen, u_char type, int len)
{
    return (build_asn1_sequence(data, datalen, type, len));
}


u_char *
libnet_build_asn1_objid(u_char *data, int *datalen, u_char type, oid *objid,
        int oid_s)
{
    return (build_asn1_objid(data, datalen, type, objid, oid_s));
}


u_char *
libnet_build_asn1_null(u_char *data, int *datalen, u_char type)
{
    return (build_asn1_null(data, datalen, type));
}


u_char *
libnet_build_asn1_bitstring(u_char *data, int *datalen, u_char type,
        u_char *string, int str_s)
{
    return (build_asn1_bitstring(data, datalen, type, string, str_s));
}
 
/* EOF */
