/*
  mies.c
  
  "Less is more." -- Mies van der Rowe.

  Demonstrate OpenBSD 2.4 < 4 byte last frag ENOBUF sendto() bug.

  the OpenBSD definition in <sys/param.h> for 2.4 was skipped!
  code should check for OpenBSD < 199905 (2.5)...

  dugsong@monkey.org

  $Id: mies.c,v 1.2 1999/06/25 19:12:07 dugsong Exp $
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

/* Configurable parameters. */
#define TEST_PROTO	IPPROTO_TCP
#define IP_SRC		"10.0.0.1"
#define IP_DST		"192.0.0.1"
#define FRAG_NUM	2
#define FRAG_SZ		8
#define LAST_SZ 	1

/* Define BSD_BUG if testing on BSD/OS, etc. */
#ifdef BSD_BUG
#define FIX(x)	(x)
#else
#define FIX(x)	htons(x)
#endif

int
main()
{
  unsigned char buf[IP_MAXPACKET];
  struct ip *iph;
  struct sockaddr_in sin;
  int sock, pktlen, offset, i, one = 1;
  
  iph = (struct ip *)buf;
  pktlen = sizeof(struct ip) + FRAG_SZ;
  
  iph->ip_hl = 20 / 4;
  iph->ip_v = 4;
  iph->ip_tos = 0;
  iph->ip_len = FIX(pktlen);
  iph->ip_id = htons(getpid());
  iph->ip_ttl = 255;
  iph->ip_p = IPPROTO_TCP;
  iph->ip_sum = 0;
  iph->ip_src.s_addr = inet_addr(IP_SRC);
  iph->ip_dst.s_addr = inet_addr(IP_DST);
  
  if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
    perror("socket");
  
  if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) == -1)
    perror("setsockopt");
  
  memset(&sin, 0, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = iph->ip_dst.s_addr;

  /* Send fragments. */
  for (offset = 0 ; offset < FRAG_NUM ; offset++) {
    iph->ip_off = FIX(IP_MF | offset);
    
    if (sendto(sock, buf, pktlen, 0, (struct sockaddr *)&sin,
	       sizeof(struct sockaddr)) != pktlen)
      perror("sendto");

    printf(IP_SRC " > " IP_DST ": (frag %d:%d@%d+)\n",
	   iph->ip_id, FRAG_SZ, offset << 3);
  }
  /* Send last fragment. */
  pktlen = sizeof(struct ip) + LAST_SZ;
  iph->ip_len = FIX(pktlen);
  iph->ip_off = FIX(IP_MF | offset);

  if (sendto(sock, buf, pktlen, 0, (struct sockaddr *)&sin,
	     sizeof(struct sockaddr)) != pktlen) {
    perror("sendto");
    printf("packet NOT sent: ");
  }
  printf(IP_SRC " > " IP_DST ": (frag %d:%d@%d)\n",
	 iph->ip_id, LAST_SZ, offset << 3);
  
  exit(0);
}
