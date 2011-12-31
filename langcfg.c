/*
 * Ismail Yenigul's famous language config file reader/parser
 * This code derived from EnderUNIX isoqlog project.
 *
 * http://www.enderunix.org/isoqlog
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "defs.h"
#include "langcfg.h"

void loadlang(char *langfile) 
{
	FILE *fd;
	char buf[BIGBUFSIZE];
	char keyword[TINYBUFSIZE];
	char value[BIGBUFSIZE];
	char *cp1, *cp2;
	char *variables[] = { "Invalid",
		"l_charset",
		"l_title",
		"l_create_date",
		"l_no",
		"l_domain",
		"l_ns",
		"l_mx",
		"l_www",
		"l_customer",
		"l_not_customer",
		"l_domain_not_used",
		"l_domain_total",
		"l_link_index",
		"l_link_gsorted",
		"l_link_asorted",
		"l_software_sign1",
		"l_software_sign2",
		"l_software_sign3",
		"l_extra",
		"l_query"
	};

	int i, key, line, keyword_nums = sizeof(variables)/sizeof(char *);

	
	if ((fd = fopen(langfile, "r")) == NULL) {
		fprintf(stderr, "loadLang: cannot open language configuration file %s, exiting...\n", langfile);
		exit(-1);
	}
	line = 0;
	while ((fgets(buf, BIGBUFSIZE, fd)) != NULL) {
		line++;
		if (buf[0] == '#') 
			continue;
		if ((strlen(buf)) <= 1)
			continue;

		cp1 = buf;
		cp2 = keyword;
		while (isspace((int)*cp1)) 
			cp1++;
		while (isgraph((int)*cp1) && (*cp1 != '=')) 
			*cp2++ = *cp1++;
		*cp2 = '\0';
		cp2 = value;
		while ((*cp1 != '\0') && (*cp1 !='\n') && (*cp1 !='='))
			cp1++;
		cp1++; 
		while (isspace((int)*cp1))
			cp1++; 
		if (*cp1 == '"') 
			cp1++;
		while ((*cp1 != '\0') && (*cp1 !='\n') && (*cp1 !='"'))
			*cp2++ = *cp1++;
		*cp2-- = '\0';
		if (keyword[0] =='\0' || value[0] =='\0')
			continue;
		key = 0;
		for (i = 0; i < keyword_nums; i++) {
			if ((strcmp(keyword, variables[i])) == 0) {
				key = i;
				break;
			}
		}

		switch(key) {
		case 0:
			fprintf(stderr, "Illegal Keyword: %s\n", keyword);
			break;
		case 1:
			strncpy(l_charset, value, TINYBUFSIZE);
			break;
		case 2:
			strncpy(l_title, value, TINYBUFSIZE);
			break;
		case 3:
			strncpy(l_create_date, value, TINYBUFSIZE);
			break;
		case 4:
			strncpy(l_no, value, TINYBUFSIZE);
			break;
		case 5:
			strncpy(l_domain, value, TINYBUFSIZE);
			break;
		case 6:
			strncpy(l_ns, value, TINYBUFSIZE);
			break;
		case 7:
			strncpy(l_mx, value, TINYBUFSIZE);
			break;
		case 8:
			strncpy(l_www, value, TINYBUFSIZE);
			break;
		case 9:
			strncpy(l_customer, value, TINYBUFSIZE);
			break;
		case 10:
			strncpy(l_not_customer, value, TINYBUFSIZE);
			break;
		case 11:
			strncpy(l_domain_not_used, value, TINYBUFSIZE);
			break;
		case 12:
			strncpy(l_domain_total, value, TINYBUFSIZE);
			break;
		case 13:
			strncpy(l_link_index, value, TINYBUFSIZE);
			break;
		case 14:
			strncpy(l_link_gsorted, value, TINYBUFSIZE);
			break;
		case 15:
			strncpy(l_link_asorted, value, TINYBUFSIZE);
			break;
		case 16:
			strncpy(l_software_sign1, value, TINYBUFSIZE);
			break;
		case 17:
			strncpy(l_software_sign2, value, TINYBUFSIZE);
			break;
		case 18:
			strncpy(l_software_sign3, value, TINYBUFSIZE);
			break;
		case 19:
			strncpy(l_extra, value, TINYBUFSIZE);
			break;
		case 20:
			strncpy(l_query, value, TINYBUFSIZE);
			break;
		}
	}
	fclose(fd);
	/* If Encoding is left blank, default it to iso-8859-1 */
	if (strlen(l_charset) == 0)
		strcpy(l_charset, "iso-8859-1");
}
