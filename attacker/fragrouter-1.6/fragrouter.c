/*
  fragrouter.c

  network IDS evasion toolkit.

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
  
  $Id: fragrouter.c,v 1.23 1999/09/21 15:47:32 dugsong Exp $
*/

#include "config.h"

#ifdef STDC_HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <sys/types.h>
#include <unistd.h>
#endif
#include "ip_frag.h"
#include "sniff.h"
#include "send.h"
#include "attack.h"
#include "version.h"

void
usage(void)
{
  int i;
  char *str;
  
  fprintf(stderr, "Version " FRAGROUTER_VERSION "\n"
      "Usage: fragrouter [-i interface] [-p] [-g hop] [-G hopcount] ATTACK\n\n"
      " where ATTACK is one of the following:\n\n");

  for (i = 1; i < 10; i++)
    if ((str = attack_string(ATTACK_BASE, i)) != NULL)
      fprintf(stderr, " -B%d: %s\n", i, str);

  for (i = 1; i < 10; i++)
    if ((str = attack_string(ATTACK_FRAG, i)) != NULL)
      fprintf(stderr, " -F%d: %s\n", i, str);
  
  for (i = 1; i < 10; i++)
    if ((str = attack_string(ATTACK_TCP, i)) != NULL)
      fprintf(stderr, " -T%d: %s\n", i, str);
  
  for (i = 1; i < 10; i++)
    if ((str = attack_string(ATTACK_TCBC, i)) != NULL)
      fprintf(stderr, " -C%d: %s\n", i, str);
  
  for (i = 1; i < 10; i++)
    if ((str = attack_string(ATTACK_TCBT, i)) != NULL)
      fprintf(stderr, " -R%d: %s\n", i, str);

  for (i = 1; i < 10; i++)
    if ((str = attack_string(ATTACK_INSERT, i)) != NULL)
      fprintf(stderr, " -I%d: %s\n", i, str);

  for (i = 1; i < 10; i++)
    if ((str = attack_string(ATTACK_EVADE, i)) != NULL)
      fprintf(stderr, " -E%d: %s\n", i, str);

  for (i = 1; i < 10; i++)
    if ((str = attack_string(ATTACK_MISC, i)) != NULL)
      fprintf(stderr, " -M%d: %s\n", i, str);
  
  fprintf(stderr, "\n");
  exit(1);
}

int
main(int argc, char *argv[])
{
  char c, ebuf[BUFSIZ], hops[BUFSIZ], *dev = NULL;
  int num = 0, type = -1, hopptr = 4;
  attack_handler attack;

  hops[0] = '\0';
  
  while ((c = getopt(argc, argv, "B:F:T:C:R:I:E:M:i:g:G:pVh")) != EOF) {
    switch (c) {
    case 'B':
      type = ATTACK_BASE;
      num = atoi(optarg);
      break;
    case 'F':
      type = ATTACK_FRAG;
      num = atoi(optarg);
      break;
    case 'T':
      type = ATTACK_TCP;
      num = atoi(optarg);
      break;
    case 'C':
      type = ATTACK_TCBC;
      num = atoi(optarg);
      break;
    case 'R':
      type = ATTACK_TCBT;
      num = atoi(optarg);
      break;
    case 'I':
      type = ATTACK_INSERT;
      num = atoi(optarg);
      break;
    case 'E':
      type = ATTACK_EVADE;
      num = atoi(optarg);
      break;
    case 'M':
      type = ATTACK_MISC;
      num = atoi(optarg);
      break;
    case 'p':
      ip_frag_init(1);
      break;
    case 'g':
      strcat(hops, optarg);
      strcat(hops, " ");
      break;
    case 'G':
      hopptr = atoi(optarg);
      break;
    case 'i':
      dev = strdup(optarg);
      break;
    default:
      usage();
    }
  }
  argc -= optind;
  argv += optind;
  
  if (argc != 0 || type == -1) usage();

  if (!sniff_init(dev, ebuf)) {
    fprintf(stderr, "fragrouter: sniff_init failed: %s\n", ebuf);
    exit(1);
  }
  if (!send_init(ebuf, hops, hopptr)) {
    fprintf(stderr, "fragrouter: send_init failed: %s\n", ebuf);
    exit(1);
  }
  if (!(attack = attack_init(type, num, ebuf))) {
    fprintf(stderr, "fragrouter: attack_init failed: %s\n", ebuf);
    exit(1);
  }
  fprintf(stderr, "fragrouter: %s\n", attack_string(type, num));
  
  sniff_loop(attack);

  exit(0);
}

/* 5000. */
