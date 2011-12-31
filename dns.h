#ifndef DNS_H
#define DNS_H

/*
Internet Domain Name Server Analyser and Reporter
Copyright (C) 2004 ATILIM BOY <aboy || enderunix.org>
All rights reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

typedef struct dns_rec {
	char name[TINYBUFSIZE];
	char ns1[TINYBUFSIZE];
	char ns2[TINYBUFSIZE];
	char mx1[TINYBUFSIZE];
	char mx2[TINYBUFSIZE];
	char www[TINYBUFSIZE];
	int status;
	struct dns_rec *next;
} dnsrec;

extern int tp0, tp1, tp2;

int get_dns_record(const char *domain, char *rec1, char *rec2, int type);
int get_www_ip(const char *domain, char *www);
int add_to_list(const char *domain);
dnsrec * get_cur_drec(void);
int copy_drec(dnsrec *dest_drec, const dnsrec *src_drec);
void ns_error(const char *domain, int error);

#endif
