/*
 * $Id: checksum.c,v 1.3 1999/05/28 14:30:45 dugsong Exp $
 *
 * checksum.c - IP checksum routines
 *
 * Copyright (c) 1999 Dug Song <dugsong@monkey.org>
 * All rights reserved, all wrongs reversed.
 *
 * Disclaimer: i claim dis.
 */

#if (HAVE_CONFIG_H)
#include "../include/config.h"
#endif
#include "../include/libnet.h"

#define CKSUM_CARRY(x) (x = (x >> 16) + (x & 0xffff), \
			(~(x + (x >> 16)) & 0xffff))

int
in_cksum(u_short *addr, int len)
{
  int sum = 0;
  int nleft = len;
  u_short tmp = 0, *w = addr;
  
  while (nleft > 1)  {
    sum += *w++;
    nleft -= 2;
  }
  if (nleft == 1) {
    *(u_char *)(&tmp) = *(u_char *)w;
    sum += tmp;
  }
  return (sum);
}

int
do_checksum(u_char *buf, int protocol, int len)
{
  struct ip *iph = (struct ip *)buf;
  int ip_hl = iph->ip_hl * 4;
  int sum;
  
  switch (protocol) {

  case IPPROTO_TCP:
    {
      struct tcphdr *tcph = (struct tcphdr *)(buf + ip_hl);
      
      tcph->th_sum = 0;
      sum = in_cksum((u_short *)&iph->ip_src, 8);
      sum += ntohs(IPPROTO_TCP + len);
      sum += in_cksum((u_short *)tcph, len);
      tcph->th_sum = CKSUM_CARRY(sum);
      break;
    }
  case IPPROTO_UDP:
    {
      struct udphdr *udph = (struct udphdr *)(buf + ip_hl);

      udph->uh_sum = 0;
      sum = in_cksum((u_short *)&iph->ip_src, 8);
      sum += ntohs(IPPROTO_UDP + len);
      sum += in_cksum((u_short *)udph, len);
      udph->uh_sum = CKSUM_CARRY(sum);
      break;
    }
  case IPPROTO_ICMP:
    {
      struct libnet_icmp_hdr *icmph = (struct libnet_icmp_hdr *)(buf + ip_hl);

      icmph->icmp_sum = 0;
      sum = in_cksum((u_short *)icmph, len);
      icmph->icmp_sum = CKSUM_CARRY(sum);
      break;
    }
  case IPPROTO_IGMP:
    {
      struct libnet_igmp_hdr *igmph = (struct libnet_igmp_hdr *)(buf + ip_hl);

      igmph->igmp_sum = 0;
      sum = in_cksum((u_short *)igmph, len);
      igmph->igmp_sum = CKSUM_CARRY(sum);
      break;
    }
  case IPPROTO_IP:
    iph->ip_sum = 0;
    sum = in_cksum((u_short *)iph, len);
    iph->ip_sum = CKSUM_CARRY(sum);
    break;

  default:
#if (__DEBUG)
    fprintf(stderr, "do_checksum: UNSUPPORTED protocol %d\n", protocol);
#endif
    break;
  }
  return (1);
}

u_short
ip_check(u_short *addr, int len)
{
  int sum;

  sum = in_cksum(addr, 5);
  sum += in_cksum(addr + 6, len - 7);

  return CKSUM_CARRY(sum);
}
