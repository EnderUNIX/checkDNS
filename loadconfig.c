/*
 * Ismail Yenigul's famous config reader/parser
 * This code derived from EnderUNIX isoqlog project.
 *
 * http://www.enderunix.org/isoqlog
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

#include "defs.h"
#include "loadconfig.h"

void loadconfig(char *cfgfile)
{
	FILE *fd = NULL;
	char buf[BIGBUFSIZE];
	char keyword[TINYBUFSIZE];
	char value[BIGBUFSIZE];
	int lenbuf = 0;
	char *cp1 = NULL, *cp2 = NULL;
	char *variables[] = { "Invalid",
		"query_name_server1",
		"query_name_server2",
		"name_server1",
		"name_server2",
		"name_server1_extra",
		"name_server2_extra",
		"dns_srv_sfw",
		"dns_conf_file",
		"html_output_dir",
		"lang_file"
	};

	int i, j, key, line, keyword_nums = sizeof(variables)/sizeof(char *);

	if ((fd = fopen(cfgfile, "r")) == NULL) {
		fprintf(stderr, "loadconfig: cannot open checkdns configuration file %s, exiting...\n", cfgfile);
		exit(-1);
	}
	line = 0;
	while ((fgets(buf, BIGBUFSIZE - 1, fd)) != NULL) {
		line++;
		if (buf[0] == '#')
			continue;
		if ((lenbuf = strlen(buf)) <= 1)
			continue;
		cp1 = buf;
		cp2 = keyword;
		j = 0;
		while (isspace((int)*cp1) && ((cp1 - buf) < lenbuf)) 
			cp1++;
		while(isgraph((int)*cp1) && *cp1 != '=' && (j++ < TINYBUFSIZE - 1) && (cp1 - buf) < lenbuf)
			*cp2++ = *cp1++;
		*cp2 = '\0';
		cp2 = value;
		while ((*cp1 != '\0') && (*cp1 !='\n') && (*cp1 !='=') && ((cp1 - buf) < lenbuf))
			cp1++;
		cp1++; 
		while (isspace((int)*cp1) && ((cp1 - buf) < lenbuf))
			cp1++; 
		if (*cp1 == '"') 
			cp1++;
		j = 0;
		while ((*cp1 != '\0') && (*cp1 !='\n') && (*cp1 !='"') && (j++ < TINYBUFSIZE - 1) && ((cp1 - buf) < lenbuf))
			*cp2++ = *cp1++;
		*cp2-- = '\0';
		if (keyword[0] =='\0' || value[0] =='\0')
			continue;
		key = 0;
		for (i = 0; i < keyword_nums; i++) {
			if ((strncmp(keyword, variables[i], TINYBUFSIZE)) == 0) {
				key = i;
				break;
			}
		}

		switch(key) {
		case 0:
			fprintf(stderr, "illegal keyword in config file: %s\n", keyword);
			break;
		case 1: {
			strncpy(query_name_server1, value, TINYBUFSIZE-1);
			break;
		}
		case 2: {
			strncpy(query_name_server2, value, TINYBUFSIZE-1);
			break;
		}
		case 3:
			strncpy(name_server1, value, TINYBUFSIZE-1);
			break;
		case 4:
			strncpy(name_server2, value, TINYBUFSIZE-1);
			break;
		case 5:
			strncpy(name_server1_extra, value, TINYBUFSIZE-1);
			break;
		case 6:
			strncpy(name_server2_extra, value, TINYBUFSIZE-1);
			break;
		case 7:
			strncpy(dns_srv_sfw, value, TINYBUFSIZE-1);
			break;
		case 8:
			strncpy(dns_conf_file, value, TINYBUFSIZE-1);
			break;
		case 9:
			strncpy(html_output_dir, value, TINYBUFSIZE-1);
			break;
		case 10:
			strncpy(lang_file, value, TINYBUFSIZE-1);
			break;
		}
	}

	fclose(fd);
}
