#ifndef LOADCONFIG_H
#define LOADCONFIG_H

#include "defs.h"

char query_name_server1[TINYBUFSIZE];
char query_name_server2[TINYBUFSIZE];

char name_server1[TINYBUFSIZE];
char name_server2[TINYBUFSIZE];

char name_server1_extra[TINYBUFSIZE];
char name_server2_extra[TINYBUFSIZE];

char dns_srv_sfw[TINYBUFSIZE];
char dns_conf_file[TINYBUFSIZE];

char html_output_dir[TINYBUFSIZE];

char lang_file[TINYBUFSIZE];

void loadconfig(char *cfgfile);

#endif
