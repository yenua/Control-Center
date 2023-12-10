/*
 *  libnet
 *  arp.c - Utility routines for doing ARP resolution.
 *
 */

#if (HAVE_CONFIG_H)
#include "../include/config.h"
#endif
#include "../include/libnet.h"

#if (RAW_IS_COOKED)
#include <net/if_arp.h>

int
libnet_query_arp(u_long ip, char *hwaddr, char *ebuf)
{    
    int fd;
    struct arpreq arpreq;
    struct sockaddr_in *query;
    struct sockaddr *result;

    if ((fd=socket(PF_INET, SOCK_DGRAM, 0))==-1)
    {
        sprintf( ebuf, "socket: %s", strerror(errno));
        return -1;
    }

    memset(&arpreq, 0, sizeof arpreq);

    query=(struct sockaddr_in *)&arpreq.arp_pa;
    result=(struct sockaddr *)&arpreq.arp_ha;

    query->sin_family=AF_INET;
    query->sin_addr.s_addr=ip;
    result->sa_family=AF_UNSPEC;

    if (ioctl(fd, SIOCGARP, (caddr_t)&arpreq)==-1)
    {
        sprintf( ebuf, "ioctl: %s", strerror(errno));
        close(fd);
        return -1;
    }
    memcpy(hwaddr, (void *)arpreq.arp_ha.sa_data, 6);

    close(fd);
    return 0;
}

/* Taken from get_mac.c */

int
send_arp_request(u_long ip, u_char *device, char *ebuf)
{
    u_char *buf;
    char errbuf[1024];
    struct link_int *linkent;
    struct ether_addr *dev_hwaddr;
    u_long local_ip;
    u_char *enet_dst="\xff\xff\xff\xff\xff\xff";
    int n;

    if ((linkent=libnet_open_link_interface(device,errbuf))==NULL)
    {
        sprintf (ebuf,"libnet_open_link_interface: %s",errbuf);
        return -1;
    }

    if (libnet_init_packet(ARP_H + ETH_H, &buf) == -1)
    {
        sprintf (ebuf,"libnet_init_packet memory failure");
        return -1;
    }

    if ((dev_hwaddr = libnet_get_hwaddr(linkent, device, errbuf))==NULL)
    {
        sprintf(ebuf, "libnet_get_hwaddr: %s\n", errbuf);
        return (-1);
    }

    local_ip = htonl(libnet_get_ipaddr(linkent, device, errbuf));
    if (!local_ip)
    {
        sprintf(ebuf, "libnet_get_ipaddr: %s\n", errbuf);
        return (-1);
    }

    libnet_build_ethernet(
            enet_dst,               /* broadcast ethernet address */
            dev_hwaddr->ether_addr_octet,    /* source ethernet address */
            ETHERTYPE_ARP,          /* this frame holds an ARP packet */
            NULL,                   /* payload */
            0,                      /* payload size */
            buf);                   /* packet header memory */

    libnet_build_arp(
            ARPHRD_ETHER,           /* hardware address type */
            ETHERTYPE_IP,           /* protocol address type */
            ETHER_ADDR_LEN,         /* hardware address length */
            4,                      /* protocol address length */
            ARPOP_REQUEST,          /* packet type - ARP request */
            dev_hwaddr->ether_addr_octet, /* source (local) ethernet address */
            (u_char *)&local_ip,    /* source (local) IP address */
            enet_dst,               /* target's ethernet address (broadcast) */
            (u_char *)&ip,          /* target's IP address */
            NULL,                   /* payload */
            0,                      /* payload size */
            buf + ETH_H);           /* packet header memory */

    n = libnet_write_link_layer(linkent, device, buf, ARP_H + ETH_H);
    if (n == -1)
    {
        sprintf(ebuf, "libnet_write_link_layer failed\n");
        return (-1);
    }
    libnet_close_link_interface(linkent);
    libnet_destroy_packet(&buf);
    return 0;
}

int
libnet_active_query_arp(u_long ip, char *hwaddr, u_char *device, char *ebuf)
{    
    int tries=3;

    while (tries--)
    {
        if (libnet_query_arp(ip,hwaddr,ebuf)!=-1)
            return 0;

        send_arp_request(ip,device,ebuf);
 
        sleep(1);
    }
    return -1;
}

#endif /* RAW_IS_COOKED */

/* EOF */
