#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "loadconfig.h"
#include "langcfg.h"
#include "dns.h"
#include "tools.h"
#include "main.h"
#include "html.h"

int
html_output_start(FILE *fd)
{
	/*
	fprintf(fd, "Content-Type: text/html\r\n\r\n");
	fprintf(fd, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
	*/
	fprintf(fd, "<html>\n");
	fprintf(fd, "<head>\n");
	fprintf(fd, "<title>%s</title>\n", l_title);
	fprintf(fd, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=%s\">\n", l_charset);
	fprintf(fd, "<link rel=\"stylesheet\" href=\"checkdns.css\" type=\"text/css\">\n");
	fprintf(fd, "</head>\n");
	fprintf(fd, "<body bgcolor=\"#FFFFFF\" text=\"#000000\">\n");

	fprintf(fd, "<span class=\"create_date\">%s : %s</span><br><br>\n", l_create_date, get_date());

	fprintf(fd, "<span class=\"customer\">%s : %d</span><br>\n", l_customer, tp0);
	fprintf(fd, "<span class=\"not_customer\">%s : %d</span><br>\n", l_not_customer, tp1);
	fprintf(fd, "<span class=\"domain_not_used\">%s : %d</span><br>\n", l_domain_not_used, tp2);
	fprintf(fd, "<span class=\"domain_total\">%s : %d</span><br><br>\n", l_domain_total, tp0+tp1+tp2);

	fprintf(fd, "<table width=\"500\" border=\"1\" cellspacing=\"0\" cellpadding=\"0\" class=\"imformation\">");
	fprintf(fd, "  <tr>\n");
	fprintf(fd, "    <td class=\"table_header\">&nbsp;%s</td>\n", l_ns);
	fprintf(fd, "    <td class=\"table_header\">&nbsp;%s %s</td>\n", l_extra, l_ns);
	fprintf(fd, "    <td class=\"table_header\">&nbsp;%s %s</td>\n", l_query, l_ns);
	fprintf(fd, "  </tr>\n");
	fprintf(fd, "  <tr>\n");
	fprintf(fd, "    <td class=\"txt\">&nbsp;%s</td>\n", name_server1);
	fprintf(fd, "    <td class=\"txt\">&nbsp;%s</td>\n", name_server1_extra);
	fprintf(fd, "    <td class=\"txt\">&nbsp;%s</td>\n", query_name_server1);
	fprintf(fd, "  </tr>\n");
	fprintf(fd, "  <tr>\n");
	fprintf(fd, "    <td class=\"txt\">&nbsp;%s</td>\n", name_server2);
	fprintf(fd, "    <td class=\"txt\">&nbsp;%s</td>\n", name_server2_extra);
	fprintf(fd, "    <td class=\"txt\">&nbsp;%s</td>\n", query_name_server2);
	fprintf(fd, "  </tr>\n");
	fprintf(fd, "</table>\n");

	fprintf(fd, "<br>\n");
	fprintf(fd, "<a href=\"index.html\" class=\"top_link\">%s</a> |\n", l_link_index);
	fprintf(fd, "<a href=\"gsorted.html\" class=\"top_link\">%s</a> |\n", l_link_gsorted);
	fprintf(fd, "<a href=\"asorted.html\" class=\"top_link\">%s</a>\n", l_link_asorted);
	fprintf(fd, "<br>\n");

	fprintf(fd, "<br>\n");
	fprintf(fd, "<table width=\"100%\" border=\"1\" cellspacing=\"0\" cellpadding=\"0\" class=\"domains\">\n");
	fprintf(fd, "  <tr align=\"left\" valign=\"middle\">\n");
	fprintf(fd, "    <td class=\"table_header\">&nbsp;%s</td>\n", l_no);
	fprintf(fd, "    <td class=\"table_header\">&nbsp;%s</td>\n", l_domain);
	fprintf(fd, "    <td class=\"table_header\">&nbsp;%s 1</td>\n", l_ns);
	fprintf(fd, "    <td class=\"table_header\">&nbsp;%s 2</td>\n", l_ns);
	fprintf(fd, "    <td class=\"table_header\">&nbsp;%s 1</td>\n", l_mx);
	fprintf(fd, "    <td class=\"table_header\">&nbsp;%s 2</td>\n", l_mx);
	fprintf(fd, "    <td class=\"table_header\">&nbsp;%s</td>\n", l_www);
	fprintf(fd, "  </tr>\n");

	return 0;
}

int
html_output_table(FILE *fd, int no, const dnsrec *drec)
{
	char *css_type[] = {"customer", "not_customer", "domain_not_used"};

	fprintf(fd, "  <tr align=\"left\" valign=\"middle\">\n");
	fprintf(fd, "    <td class=\"txt\">&nbsp;%d</font></td>\n", no);
	fprintf(fd, "    <td class=\"%s\">&nbsp;%s</font></td>\n", css_type[drec->status], drec->name);
	fprintf(fd, "    <td class=\"%s\">&nbsp;%s</font></td>\n", css_type[drec->status], drec->ns1);
	fprintf(fd, "    <td class=\"%s\">&nbsp;%s</font></td>\n", css_type[drec->status], drec->ns2);
	fprintf(fd, "    <td class=\"txt\">&nbsp;%s</font></td>\n", drec->mx1);
	fprintf(fd, "    <td class=\"txt\">&nbsp;%s</font></td>\n", drec->mx2);
	fprintf(fd, "    <td>&nbsp;<a href=\"http://www.%s/\" target=\"_blank\" class=\"www_link\">%s</a></td>\n", drec->name, drec->www);
	fprintf(fd, "  </tr>\n");

	return 0;
}

int
html_output_finish(FILE *fd)
{
	fprintf(fd, "</table>\n");

	fprintf(fd, "<p><span class=\"txt\">%s <a href=\"http://www.enderunix.org/checkdns\" target=\"_blank\">CheckDNS</a> %s %s <a href=\"http://www.enderunix.org/\" target=\"_blank\">%s</a></span></p>\n", l_software_sign1, l_software_sign2, vers, l_software_sign3);

	fprintf(fd, "</body>\n");
	fprintf(fd, "</html>\n");

	return 0;
}

int
write_html(const char *type)
{
	FILE *fd;
	char htmlfile[MIDBUFSIZE] = "";
	dnsrec *cur_drec = all_drec;
	int no = 1, dstatus = 0;

	if (html_output_dir[strlen(html_output_dir)-1] != '/')
		snprintf(htmlfile, MIDBUFSIZE - 1, "%s/%s.html\0", html_output_dir, type);
	else
		snprintf(htmlfile, MIDBUFSIZE - 1, "%s%s.html\0", html_output_dir, type);

	if ((fd = fopen(htmlfile, "w")) == NULL) {
#ifdef DEBUG
		fprintf(stderr, "write_html - fopen : %s - %s\n", htmlfile, strerror(errno));
#endif
		return -1;
	}

	(void) html_output_start(fd);

	if (strcmp(type, "index") == 0) {
#ifdef DEBUG
		fprintf(stderr, "--------------------------- Listing Normal ---------------------------\n");
#endif
		for (; cur_drec != NULL; cur_drec = cur_drec->next) {
#ifdef DEBUG
			fprintf(stderr, "name -> %s : status -> %d\n", cur_drec->name, cur_drec->status);
#endif
			(void) html_output_table(fd, no++, cur_drec);
		}

	} else if (strcmp(type, "gsorted") == 0) {
#ifdef DEBUG
		fprintf(stderr, "---------------------- Listing Sorted by status ----------------------\n");
#endif
		while (1) {
			if (cur_drec->status == dstatus ) {
#ifdef DEBUG
				fprintf(stderr, "name -> %s : status -> %d\n", cur_drec->name, cur_drec->status);
#endif
				(void) html_output_table(fd, no++, cur_drec);
			}
			if (cur_drec->next != NULL)
				cur_drec = cur_drec->next;
			else {
				cur_drec = all_drec;
				if ((dstatus += 1) > 3) break;
			}
		}
	} else if (strcmp(type, "asorted") == 0) {
		dnsrec *drec1 = all_drec, *drec2 = all_drec;
		dnsrec *drec_tmp;

#ifdef DEBUG
		fprintf(stderr, "---------------------- Listing Sorted by letter ----------------------\n");
#endif

		while (drec1 != NULL){
			drec2 = drec1;
			while (drec2 != NULL) {
				if ( strcmp(drec1->name, drec2->name) > 0) {
					if ((drec_tmp = (dnsrec *) malloc(sizeof(dnsrec))) == NULL) {
#ifdef DEBUG
						fprintf(stderr, "write_html - malloc : %s\n", strerror(errno));
#endif
						return -1;
					}
					copy_drec(drec_tmp, drec1);
					copy_drec(drec1, drec2);
					copy_drec(drec2, drec_tmp);
				}
				drec2 = drec2->next;
			}
			drec1 = drec1->next;
		}

		drec1 = all_drec;
		for (; drec1 != NULL; drec1 = drec1->next) {
#ifdef DEBUG
			fprintf(stderr, "name -> %s : status -> %d\n", drec1->name, drec1->status);
#endif
			(void) html_output_table(fd, no++, drec1);
		}
	}

	(void) html_output_finish(fd);

	fclose(fd);

	return 0;
}
