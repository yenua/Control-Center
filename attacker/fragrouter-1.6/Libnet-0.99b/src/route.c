/*
 *  libnet
 *  route.c - Utility routines for doing routing socket queries.
 *  This is pilfered from UNP.
 */

#if (HAVE_CONFIG_H)
#include "../include/config.h"
#endif
#include "../include/libnet.h"

#if (RAW_IS_COOKED)
#include <net/route.h>

/* 2.6 and higher have the routing sockets */

#if (STUPID_SOLARIS_CHECKSUM_BUG) /* It's 2.5.1 or lower. */
u_long
libnet_get_next_hop(u_long ip, char *ebuf)
{    
    return -1;
}
#else /* It's 2.6 or higher. */
u_long
libnet_get_next_hop(u_long ip, char *ebuf)
{    
    int fd,n;
    char buf[4096];
    struct rt_msghdr *rtm;
    struct sockaddr_in *sin;
    pid_t pid;
    u_long seq;

    if ((fd=socket(PF_ROUTE, SOCK_RAW, 0))==-1)
    {
        sprintf( ebuf, "socket: %s", strerror(errno));
        return -1;
    }

    memset(buf,0,4096);
    rtm=(struct rt_msghdr *)buf;

    rtm->rtm_msglen=sizeof(struct rt_msghdr) + sizeof(struct sockaddr_in);
    rtm->rtm_version=RTM_VERSION;
    rtm->rtm_type=RTM_GET;
    rtm->rtm_addrs=RTA_DST;
    rtm->rtm_pid=pid=getpid();
    rtm->rtm_seq=seq=libnet_get_prand(PRu32);

    sin=(struct sockaddr_in *)(rtm+1);
    sin->sin_family=AF_INET;
    sin->sin_addr.s_addr=ip;

    n=write(fd, rtm, rtm->rtm_msglen);
    if (n<0)
    {
        sprintf( ebuf, "write: %s", strerror(errno));
        close(fd); 
        return -1;
    }
    if (n!=rtm->rtm_msglen)
    {
        sprintf( ebuf, "write: couldn't write full message");
        close(fd); 
        return -1;
    }
  
    do
    {
        n=read(fd, rtm, sizeof buf); 
    } while (rtm->rtm_type != RTM_GET || rtm->rtm_seq != seq || 
             rtm->rtm_pid != pid);

    rtm=(struct rt_msghdr *)buf;

    if (!(rtm->rtm_addrs & 2))
    {
        sprintf( ebuf, "can't retrieve gateway");
        close(fd); 
        return -1;
    }

    sin=(struct sockaddr_in *)(((struct sockaddr_in *)(rtm+1))+1);
    close(fd);
    return sin->sin_addr.s_addr;
}
#endif /* STUPID_SOLARIS_CHECKSUM_BUG */
#endif /* RAW_IS_COOKED */
/* EOF */
