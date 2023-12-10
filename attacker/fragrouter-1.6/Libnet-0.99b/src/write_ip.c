/*
 *  $Id: write_ip.c,v 1.3 1999/06/24 18:35:34 dugsong Exp $
 *
 *  libnet
 *  write_ip.c - writes a prebuilt IP packet to the network
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
write_ip(int sock, u_char *buf, int len)
{
    int c;
    struct sockaddr_in sin;
    struct ip *ip_hdr;

#if (RAW_IS_COOKED)
    return write_ip_via_datalink(buf,len);
#endif /* RAW_IS_COOKED */

    ip_hdr = (struct ip *)buf;
    
#if (BSD_BYTE_SWAP)
    /*
     *  For link access, we don't need to worry about the inconsistencies of
     *  certain BSD kernels.  However, raw socket nuances abound.  Certain
     *  BSD implmentations require the ip_len and ip_off fields to be in host
     *  byte order.  It's MUCH easier to change it here than inside the bpf
     *  writing routine.
     */
    ip_hdr->ip_len = FIX(ip_hdr->ip_len);
    ip_hdr->ip_off = FIX(ip_hdr->ip_off);
#endif

    memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family      = AF_INET;
    sin.sin_addr.s_addr = ip_hdr->ip_dst.s_addr;

    c = sendto(sock, buf, len, 0, (struct sockaddr *)&sin,
	       sizeof(struct sockaddr));
    if (c != len)
    {
#if (__DEBUG)
        fprintf(stderr, "write_ip: %d bytes written (%s)\n", c, strerror(errno));
#endif
    }
#if (BSD_BYTE_SWAP)
    ip_hdr->ip_len = UNFIX(ip_hdr->ip_len);
    ip_hdr->ip_off = UNFIX(ip_hdr->ip_off);
#endif
	
    return (c);
}

#if (RAW_IS_COOKED)
/*
 * write_ip_via_datalink
 *
 * The intent of this function is to provide an easy mechanism for someone
 * to send an IP packet that is inserted into the network via the datalink
 * access interfaces. This is useful in situations where raw sockets are 
 * cooked, which can prevent programs using write_ip from working on certain 
 * Unixes.
 * To pull this off, we have to determine a source and destination MAC
 * address for the packet, as well as the correct interface to send it on.
 * 
 * This has currently only been implemented and tested on solaris 2.6/2.5.1.
 */

int
write_ip_via_datalink(u_char *buf, int len)
{
    char ebuf[1024];
    const u_char *broadcast_hwaddr="\xff\xff\xff\xff\xff\xff";

    static u_long cached_dest_ip=0;
    static u_char *cached_device=NULL;
    static struct link_int *cached_linkent=NULL;

    struct link_int *linkent=NULL;
    u_long local_ip;
    u_long dest_ip;

    u_char *sbuf;
    static u_char source_hwaddr[6];
    static u_char dest_hwaddr[6];
    u_long gateway_ip;
    u_char *device;

    struct ip *ip_hdr;
    int c;

    ip_hdr = (struct ip *)(buf);
    dest_ip = ip_hdr->ip_dst.s_addr;
    
#if (__DEBUG)
    fprintf(stderr, "entering write_ip_via_datalink\n");
#endif

    if (cached_dest_ip == dest_ip)
    {
#if (__DEBUG)
        fprintf(stderr, "taking cached shortcut\n");
#endif
        linkent=cached_linkent;
        device=cached_device;
        goto send_it;
    }

    /*  
     * Step one: Pick the local interface to send the packet on. 
     *
     * This requires support for the udp socket trick. If it isn't there
     * then we will just pick the first available interface and try it out.
     */

    local_ip=libnet_get_interface_by_dest(dest_ip,ebuf);

    if (local_ip==-1)
    {
        fprintf(stderr,"libnet_get_interface_by_dest: %s\n",ebuf);
        return -1;
    }
    if (local_ip==0) /* We must be in Solaris 2.5.1 or lower. */
    {
        /*struct sockaddr_in tempsin;*/

#if (__DEBUG)
        fprintf(stderr, "libnet_get_interface_by_dest is not supported\n");
#endif
        /* XXX - This function is currently broken under Solaris 2.5.1
        if (libnet_select_device(&tempsin,&device,ebuf)==-1)
        {
            fprintf(stderr,"libnet_select_device: %s\n",ebuf);
            return -1;
        }
        local_ip=tempsin.sin_addr.s_addr;*/
         
        /* Instead, we will try a couple of devices and see what happens. */

        if ((linkent=libnet_open_link_interface("le0",ebuf))==NULL)
        {
            if ((linkent=libnet_open_link_interface("hme0",ebuf))==NULL)
            {
                fprintf(stderr,"I can't find the interface to use.\n");
                return -1;
            }
            else
		device="hme0";
        }
        else
            device="le0";
	 
        if ((local_ip=get_ipaddr(linkent,device,ebuf))==-1)
        {
            fprintf(stderr,"get_ipaddr: %s\n",ebuf);
            return -1;
        }
    }
    else
    {
        if (libnet_select_device_by_ip(&device,local_ip,ebuf)==-1)
        {
            fprintf(stderr,"libnet_select_device_by_ip: %s\n",ebuf);
            return -1;
        }
    }

#if (__DEBUG)
    fprintf(stderr, "Using device %s with local IP of %s\n",device,
            host_lookup(local_ip,0));
#endif 

    /*
     * Step two: Get the source MAC address.
     */

    if (libnet_query_arp(local_ip,source_hwaddr,ebuf)==-1)
    {
        fprintf(stderr,"libnet_query_arp_cache: %s\n",ebuf);
        return -1;
    }

#if (__DEBUG)
    fprintf(stderr, "Using source MAC address %x:%x:%x:%x:%x:%x\n",
            source_hwaddr[0], source_hwaddr[1], source_hwaddr[2],
            source_hwaddr[3], source_hwaddr[4], source_hwaddr[5]);
#endif 

    /*
     * Step three: Get the destination MAC address.
     *
     * To really do this right, we use a routing socket to query the kernel
     * for the appropriate gateway. If it is our own interface then we need
     * to do an ARP resolution of the target IP. Otherwise, we need to do
     * an ARP resolution of the gateway IP. If there is no support for routing
     * sockets then we can use the broadcast MAC address which seems to work
     * fine.
     */

    if ((gateway_ip=libnet_get_next_hop(dest_ip, ebuf))==-1)
    {
#if (__DEBUG)
        fprintf(stderr, "libnet_get_next_hop failed: %s\n",ebuf);
#endif 
        memcpy(dest_hwaddr,broadcast_hwaddr,6);
    }
    else
    {
        if (gateway_ip==local_ip)
        {
            /* This is on our subnet. We need to request the ARP address of
             * the destination IP. */
            if (libnet_active_query_arp(dest_ip,dest_hwaddr,device,ebuf)==-1)
            {
#if (__DEBUG)
                fprintf(stderr, "libnet_query_arp failed: %s\n",ebuf);
#endif 
                memcpy(dest_hwaddr,broadcast_hwaddr,6);
            }
        }
        else /* We need to send to a gateway. */
        {
            if (libnet_active_query_arp(gateway_ip,dest_hwaddr,device,ebuf)==-1)
            {
#if (__DEBUG)
                fprintf(stderr, "libnet_query_arp failed: %s\n",ebuf);
#endif 
                memcpy(dest_hwaddr,broadcast_hwaddr,6);
            }
        }
    }

#if (__DEBUG)
    fprintf(stderr, "Using destination MAC address %x:%x:%x:%x:%x:%x\n",
            dest_hwaddr[0], dest_hwaddr[1], dest_hwaddr[2],
            dest_hwaddr[3], dest_hwaddr[4], dest_hwaddr[5]);
#endif 

    /*
     * Step four: ship it.
     */

    if (!linkent && (linkent=libnet_open_link_interface(device,ebuf))==NULL)
    {
        fprintf(stderr,"libnet_open_link_interface: %s\n",ebuf);
        return -1;
    }

send_it:

    if (libnet_init_packet(ETH_H + len, &sbuf)==-1)
    {
        fprintf(stderr,"libnet_init_packet: %s\n",ebuf);
        return -1;
    }

    libnet_build_ethernet(dest_hwaddr,source_hwaddr,ETHERTYPE_IP,NULL,0,sbuf);

    memcpy(sbuf+ETH_H,buf,len);
    libnet_do_checksum(sbuf+ETH_H, IPPROTO_IP, ip_hdr->ip_hl*4);
    if ((c=libnet_write_link_layer(linkent,device,sbuf,ETH_H+len))<0)
    {
        fprintf(stderr,"libnet_write_link_later: %s\n",strerror(errno));
        return -1;
    }

    cached_dest_ip=dest_ip;
    cached_device=device;
    if (cached_linkent && (cached_linkent != linkent))
    {
        libnet_close_link_interface(cached_linkent);
    }
    cached_linkent=linkent;

    libnet_destroy_packet(&sbuf);

    if (c != len +ETH_H)
    {
#if (__DEBUG)
        fprintf(stderr, "write_ip_via_datalink: %d bytes written (%s)\n", c, strerror(errno));
#endif
    }

    return (c-ETH_H);

}
#endif /* RAW_IS_COOKED */

/* EOF */
