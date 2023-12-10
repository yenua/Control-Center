/*
  sniff.c

  Dug Song <dugsong@anzen.com>

  Copyright (c) 1999 Anzen Computing. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
  3. All advertising materials mentioning features or use of this software
     must display the following acknowledgement:
     This product includes software developed by Anzen Computing.
  4. Neither the name of Anzen Computing nor the names of its
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  $Id: sniff.c,v 1.25 1999/07/30 13:02:35 dugsong Exp $
*/

#include "config.h"

#ifdef STDC_HEADERS
#include <stdio.h>
#endif
#ifdef HAVE_UNISTD_H
#include <sys/param.h>
#endif
#include <pcap.h>
#include <libnet.h>
#include "sniff.h"

pcap_t *sniff_pd;
int sniff_ll_len;

int
sniff_init(char *intf, char *ebuf)
{
  char *dev, filter[BUFSIZ];
  struct link_int *llif;
  struct ether_addr *llmac;
  u_long llip;
  u_int net, mask;
  struct bpf_program fcode;

  /* Get interface. */
  if (intf) dev = intf;
  else if (!(dev = pcap_lookupdev(ebuf)))
    return 0;

  /* Get interface IP and MAC address and link layer header length. */
  if (!(llif = libnet_open_link_interface(dev, ebuf)))
    return 0;

  if (!(llip = libnet_get_ipaddr(llif, dev, ebuf)))
    return 0;

  if (!(llmac = libnet_get_hwaddr(llif, dev, ebuf)))
    return 0;

  libnet_close_link_interface(llif);

  llip = ntohl(llip);
 
  if (!llif->linkoffset)
    sniff_ll_len = 14; /* XXX - assume Ethernet, if libnet fails us here. */
  else 
    sniff_ll_len = llif->linkoffset;

  /* Generate packet filter. We aren't sniffing in promiscuous mode,
     but be specific in case someone else is. */
  snprintf(filter, sizeof(filter),
	   "ip and ether dst %x:%x:%x:%x:%x:%x and not dst %s and not ip broadcast",
	   llmac->ether_addr_octet[0], llmac->ether_addr_octet[1],
	   llmac->ether_addr_octet[2], llmac->ether_addr_octet[3],
	   llmac->ether_addr_octet[4], llmac->ether_addr_octet[5],
	   libnet_host_lookup(llip, 0));
#ifdef DEBUG
  fprintf(stderr, "fragrouter: %s\n", filter);
#endif /* DEBUG */
  
  /* Open interface for sniffing, don't set promiscuous mode. */
  if (pcap_lookupnet(dev, &net, &mask, ebuf) == -1)
    return 0;

  if (!(sniff_pd = pcap_open_live(dev, 2048, 0, 1024, ebuf)))
    return 0;
  
  if (pcap_compile(sniff_pd, &fcode, filter, 1, mask) < 0) {
    strcpy(ebuf, pcap_geterr(sniff_pd));
    return 0;
  }
  if (pcap_setfilter(sniff_pd, &fcode) == -1) {
    strcpy(ebuf, pcap_geterr(sniff_pd));
    return 0;
  }
#ifdef DEBUG
  fprintf(stderr, "fragrouter: sniffing on %s\n", dev);
#endif /* DEBUG */
  
  return 1;
}

void
sniff_loop(sniff_handler attack)
{
  struct pcap_pkthdr pkthdr;
  struct ip *iph;
  u_char *pkt;
  int len;
  
  for (;;) {
    if ((pkt = (char *)pcap_next(sniff_pd, &pkthdr)) != NULL) {
      iph = (struct ip *)(pkt + sniff_ll_len);

      len = ntohs(iph->ip_len);
      if (len > pkthdr.len)
	len = pkthdr.len;
      
      attack(pkt + sniff_ll_len, len);
    }
  }
}
