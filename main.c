#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "defs.h"
#include "loadconfig.h"
#include "dns.h"
#include "html.h"
#include "tools.h"
#include "main.h"
#include "langcfg.h"

int main(int argc, char **argv)
{
	char line[TINYBUFSIZE] = "", domain[TINYBUFSIZE];
	int c = 0;
	FILE *fd;
	char *tmp, *tmp1;

	(void) strncpy(conf_file, "/usr/local/etc/checkdns.conf", TINYBUFSIZE-1);

	while((c = getopt(argc, argv, "c:hv")) != -1) {
		switch(c) {
			case 'c' :
				(void) strncpy(conf_file, optarg ,TINYBUFSIZE-1);
				break;
			case 'v' :
				version();
				exit(0);
			default:
				usage();
				exit(0);
		}
	}

	loadconfig(conf_file);

	loadlang(lang_file);

	check_startup_variable();

	if ((fd = fopen(dns_conf_file, "r")) == NULL) {
		fprintf(stderr, "Can't open %s Configuration file !\n", dns_conf_file);
		exit(-1);
	}

	while(fgets(line, TINYBUFSIZE - 1, fd) != NULL) {
		if (strcmp(dns_srv_sfw, "bind") == 0) {
			if ((tmp = strstr(line, "zone \"")) == NULL)
				continue;
			if ((tmp1 = strchr(tmp+6, '\"')) != NULL)
				*tmp1 = '\0';
			if (strstr(tmp+6, "in-addr") == NULL && strstr(tmp+6, "IN-ADDR") == NULL && strcmp(tmp+6, ".") != 0)
				add_to_list(tmp+6);
		} else if (strcmp(dns_srv_sfw, "djbdns") == 0) {
			if (line[0] != '.' && line[0] != 'Z')
				continue;
			if (strchr(line, ':') == NULL) 
				continue;
			if ((tmp1 = strchr(line, ':')) != NULL)
				*tmp1 = '\0';
			if (strcmp(domain, line+1) == 0)
				continue;
			(void) strncpy(domain, line+1, TINYBUFSIZE-1);
			add_to_list(domain);
		} else if (strcmp(dns_srv_sfw, "none") == 0) {
			line[strlen(line)-1] = '\0';
			add_to_list(line);
		} else {
			fprintf(stderr, "Please specify 'dns_srv_sfw' variable to 'bind' or 'djbdns' in checkdns.conf file !\n");
			exit(-1);
		}
	}

	fprintf(stdout, "\n\n");
	fprintf(stdout, "%s : %d\n", l_customer, tp0);
	fprintf(stdout, "%s : %d\n", l_not_customer, tp1);
	fprintf(stdout, "%s : %d\n\n", l_domain_not_used, tp2);
	fprintf(stdout, "%s : %d\n\n", l_domain_total, tp0+tp1+tp2);

	if (all_drec == NULL) {
			fprintf(stderr, "DNS conf file have no data for analysing it !\n");
			exit(-1);
	}

	write_html("index");

	write_html("gsorted");

	write_html("asorted");

	return 0;
}

static void
check_startup_variable()
{
	struct stat stbuf;

	if (strlen(query_name_server1) < 1 && strlen(query_name_server2) < 1) {
		fprintf(stderr, "Please specify 'query_name_server1' or 'query_name_server2' variable in checkdns.conf file !\n");
		exit(-1);
	}

	if (strlen(name_server1) < 1 && strlen(name_server2) < 1) {
		fprintf(stderr, "Please specify 'name_server1' or 'name_server2' variable in checkdns.conf file !\n");
		exit(-1);
	}

	if (strlen(dns_srv_sfw) < 1) {
		fprintf(stderr, "Please specify 'dns_srv_sfw' variable in checkdns.conf file !\n");
		exit(-1);
	}

	if (strlen(dns_conf_file) < 1) {
		fprintf(stderr, "Please specify 'dns_conf_file' variable in checkdns.conf file !\n");
		exit(-1);
	}

	if (strlen(html_output_dir) < 1) {
		fprintf(stderr, "Please specify 'html_output_dir' variable in checkdns.conf file !\n");
		exit(-1);
	}

	if (lstat(html_output_dir, &stbuf) == -1) {
		fprintf(stderr, "'html_output_dir' variable is not a regular directory name !\n");
		exit(-1);
	}

	if (!S_ISDIR(stbuf.st_mode)) {
		fprintf(stderr, "'html_output_dir' variable is not a regular directory name !\n");
		exit(-1);
	}

	if (strlen(lang_file) < 1) {
		fprintf(stderr, "Please specify 'lang_file' variable in checkdns.conf file !\n");
		exit(-1);
	}
}

static void
version()
{
	fprintf(stdout, "version: %s\n", vers);
}

static void
usage()
{
	fprintf(stdout, "Usage: checkdns [options]\n");
	fprintf(stdout, "options:\n");
	fprintf(stdout, "-c\t\tconfiguration file\n");
	fprintf(stdout, "-h\t\tprints this help\n");
	fprintf(stdout, "-v\t\tversion number\n");
	fprintf(stdout, "\n");

	fprintf(stdout, "EnderUNIX CheckDNS %s\n", vers);
	fprintf(stdout, "http://www.enderunix.org/checkdns\n");
	fprintf(stdout, "ATILIM BOY <aboy@enderunix.org>\n");
}
