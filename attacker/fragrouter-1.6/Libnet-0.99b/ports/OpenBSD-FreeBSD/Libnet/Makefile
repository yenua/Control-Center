# New ports collection makefile for:    libnet
# Version required:                     0.10
# Dated Created:                        1 January 1999
# Whom:                                 Mike D. Schiffman
#
# $Id: Makefile,v 1.1.1.1 1999/05/18 15:33:43 dugsong Exp $
#

#EXTRACT_SUFX=	.tgz

DISTNAME=       libnet
PKGNAME=        libnet-0.9
CATEGORIES=     net
MASTER_SITES=   http://www.infonexus.com/~daemon9/Libnet/
MAINTAINER=     route@infonexus.com
GNU_CONFIGURE=  yes

# Only during the fetch phase.
# This can be removed along with the lynx fetch command when someone puts
# Libnet up on an ftp site.
IS_INTERACTIVE= yes
FETCH_CMD=      /usr/bin/lynx

WRKSRC=         ${WRKDIR}/Libnet

.include <bsd.port.mk>

# EOF
