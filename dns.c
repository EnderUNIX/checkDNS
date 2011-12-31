#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <resolv.h>

#include "defs.h"
#include "tools.h"
#include "dns.h"
#include "main.h"
#include "loadconfig.h"
#include "langcfg.h"

int tp0 = 0, tp1 = 0, tp2 = 0;

int
add_to_list(const char *domain)
{
	int extdns = 0, ret = 0;
	dnsrec *new_drec, *cur_drec;

	if ((new_drec = (dnsrec *) malloc(sizeof(dnsrec))) == NULL) {
#ifdef DEBUG
		fprintf(stderr, "add_to_list - malloc : %s\n", strerror(errno));
#endif
		return -1;
	}

	(void) strncpy(new_drec->name, domain, TINYBUFSIZE-1);

	if ((ret = get_dns_record(domain, new_drec->ns1, new_drec->ns2, T_NS)) == -1) {
		bzero(new_drec->mx1, TINYBUFSIZE-1);
		bzero(new_drec->mx2, TINYBUFSIZE-1);
		bzero(new_drec->www, TINYBUFSIZE-1);
	}

	if (ret != -1)
		(void) get_dns_record(domain, new_drec->mx1, new_drec->mx2, T_MX);

	if (strlen(name_server1_extra) > 0 && strlen(name_server2_extra) > 0)
		extdns = 1;

	if (strcmp(new_drec->ns1, "") == 0 && strcmp(new_drec->ns2, "") == 0) {
		new_drec->status = 2; tp2++;
		fprintf(stdout, "%s -> %s\n", domain, l_domain_not_used);
	} else if (strcmp(new_drec->ns1, name_server1) == 0 && strcmp(new_drec->ns2, name_server2) == 0 || strcmp(new_drec->ns1, name_server2) == 0 && strcmp(new_drec->ns2, name_server1) == 0) {
		new_drec->status = 0; tp0++;
		fprintf(stdout, "%s -> %s\n", domain, l_customer);
	} else if (extdns != 0 && strcmp(new_drec->ns1, name_server1_extra) == 0 && strcmp(new_drec->ns2, name_server2_extra) == 0 || extdns != 0 && strcmp(new_drec->ns1, name_server2_extra) == 0 && strcmp(new_drec->ns2, name_server1_extra) == 0) {
		new_drec->status = 0; tp0++;
		fprintf(stdout, "%s -> %s\n", domain, l_customer);
	} else {
		new_drec->status = 1;tp1++;
		fprintf(stdout, "%s -> %s\n", domain, l_not_customer);
	}

	if (ret != -1)
		if (get_www_ip(domain, new_drec->www) == -1)
			bzero(new_drec->www, TINYBUFSIZE-1);

	new_drec->next = NULL;

	if (all_drec != NULL) {
		cur_drec = get_cur_drec();
		cur_drec->next = new_drec;
	} else {
		all_drec = new_drec;
		all_drec->next = NULL;
	}

	return 0;
}

int
get_www_ip(const char *domain, char *www)
{
	struct hostent *host;
	char webaddr[136];

	snprintf(webaddr, sizeof(webaddr) - 1, "www.%.128s", domain);

	if ((host = gethostbyname(webaddr)) == NULL) {
#ifdef DEBUG
		fprintf(stderr, "error - gethostbyname : \"%s\", %s\n", hstrerror(h_errno), domain);
#endif
		return -1;
	}

	(void) strncpy(www, inet_ntoa(*((struct in_addr *)host->h_addr)), TINYBUFSIZE-1);

	return 0;
}

dnsrec *
get_cur_drec(void)
{
	dnsrec *cur_drec = all_drec;

	while (cur_drec->next != NULL)
		cur_drec = cur_drec->next;

	return cur_drec;
}


int
get_dns_record(const char *domain, char *rec1, char *rec2, int reqtype)
{
	char answer[BIGBUFSIZE], buf[BIGBUFSIZE]; /*, qdomain[128] = "www."; */
	int ret = 0, rd_len = 0, ttl, n = 0;
	unsigned char *msg, *eom, *rd;
	unsigned short pref, type;
	/* struct in_addr inaddr; */
	int16_t r_len, rc;
	int8_t ancount;
	HEADER *hdr;

	bzero(rec1, TINYBUFSIZE-1);
	bzero(rec2, TINYBUFSIZE-1);

	if (res_init() != 0) {
#ifdef DEBUG
		fprintf(stderr, "error - res_init : \"%s\", %s\n", strerror(errno), domain);
#endif
		return(-1);
	}
	
	/* Setting target name servers for sending DNS query...*/
	/*
	for (i = 0; i<2; i++) _res.nsaddr_list[i].sin_addr.s_addr = inet_addr(target_ns_addr[i]);
	*/
	_res.nsaddr_list[0].sin_addr.s_addr = inet_addr(query_name_server1);
	_res.nsaddr_list[1].sin_addr.s_addr = inet_addr(query_name_server2);

	/*
	if (reqtype == T_A)
		strncat(qdomain, domain, TINYBUFSIZE-1);
	else
		strncpy(qdomain, domain, TINYBUFSIZE-1);
	*/

	if ((r_len = res_search(domain, C_IN, reqtype, (u_char *)&answer, BIGBUFSIZE)) < 0) {
#ifdef DEBUG
		fprintf(stderr, "error - res_search : \"%s\", %s\n", hstrerror(h_errno), domain);
#endif
		return(-1);
	}

	hdr = (HEADER *) &answer;
	ancount = ntohs(hdr->ancount);

	if (ancount > 0) {
		msg = (u_char *)&answer;
		eom = (u_char *)&answer + r_len;
		rd = (u_char *)&answer + HFIXEDSZ;

		if ((rc = dn_skipname(rd, eom)) < 0)
			return(-2);

		rd += rc + QFIXEDSZ;

		while ((ancount > 0) && (rd < eom)) {
			if ((ret = dn_expand(msg, eom, rd, buf, sizeof(buf))) < 0) {
#ifdef DEBUG
				fprintf(stderr, "dn_expand 1 - error : \"%s\"\n", hstrerror(h_errno));
#endif
				return(-2);
			}

			rd += ret;
			GETSHORT(type, rd);
			rd += INT16SZ;
			GETLONG(ttl, rd);
			GETSHORT(rd_len, rd);

			switch (reqtype) {
				case T_MX:
					if (type != T_MX)
						rd += rd_len;
					else {
						GETSHORT(pref, rd);
						if ((ret = dn_expand(msg, eom, rd, buf, sizeof(buf))) < 0 ) {
#ifdef DEBUG
							fprintf(stderr, "dn_expand 2 - error : \"%s\"\n", hstrerror(h_errno));
#endif
							return(-2);
						}
					}
					break;
				case T_NS:
					if (type != T_NS)
						rd += rd_len;
					else {
						if ((ret = dn_expand(msg, eom, rd, buf, sizeof(buf))) < 0 ) {
#ifdef DEBUG
							fprintf(stderr, "dn_expand 3 - error : \"%s\"\n", hstrerror(h_errno));
#endif
							return(-2);
						}
					}
					break;
					/* This is not easy and useful method because of the CNAME.. gethostbyname prefered.
					case T_A:
						if (type != T_A)
							rd += rd_len;
						else {
							bcopy(rd, (char *)&inaddr, INADDRSZ);
							strncpy(buf, inet_ntoa(inaddr), BIGBUFSIZE-1);
							fprintf(stderr, "Buf , inaddr : %s, %d\n", buf, inaddr);
						}
						break;
					*/
			}
			rd += ret;
			ancount--;
			if (n == 0)
				(void) strncpy(rec1, buf, TINYBUFSIZE-1);
			else if ( n == 1)
				(void) strncpy(rec2, buf, TINYBUFSIZE-1);
			n++;
		}
	}

	return 0;
}

int
copy_drec(dnsrec *dest_drec, const dnsrec *src_drec)
{
	(void) strncpy(dest_drec->name, src_drec->name, TINYBUFSIZE-1);
	(void) strncpy(dest_drec->name, src_drec->name, TINYBUFSIZE-1);
	(void) strncpy(dest_drec->ns1, src_drec->ns1, TINYBUFSIZE-1);
	(void) strncpy(dest_drec->ns2, src_drec->ns2, TINYBUFSIZE-1);
	(void) strncpy(dest_drec->mx1, src_drec->mx1, TINYBUFSIZE-1);
	(void) strncpy(dest_drec->mx2, src_drec->mx2, TINYBUFSIZE-1);
	(void) strncpy(dest_drec->www, src_drec->www, TINYBUFSIZE-1);
	dest_drec->status = src_drec->status;

	return 0;
}
