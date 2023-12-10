SRCS= ether_mod.c ether_mod_load.c
OBJS=$(SRCS:.c=.o)

MODOBJ=ether_spoof.o
CFLAGS+= -D_KERNEL -I/sys

.include <bsd.subdir.mk>

all:	$(MODOBJ)

$(MODOBJ): $(OBJS)
	$(LD) -r -o $(MODOBJ) $(OBJS) 
clean:
	rm -f *.o

