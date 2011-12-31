#ifndef LANGCFG_H
#define LANGCFG_H

#include "defs.h"

/* Language - Specific Strings	*/

char l_charset[TINYBUFSIZE];
char l_title[TINYBUFSIZE];
char l_create_date[TINYBUFSIZE];
char l_no[TINYBUFSIZE];
char l_domain[TINYBUFSIZE];
char l_ns[TINYBUFSIZE];
char l_mx[TINYBUFSIZE];
char l_www[TINYBUFSIZE];
char l_customer[TINYBUFSIZE];
char l_not_customer[TINYBUFSIZE];
char l_domain_not_used[TINYBUFSIZE];
char l_domain_total[TINYBUFSIZE];
char l_link_index[TINYBUFSIZE];
char l_link_gsorted[TINYBUFSIZE];
char l_link_asorted[TINYBUFSIZE];
char l_software_sign1[TINYBUFSIZE];
char l_software_sign2[TINYBUFSIZE];
char l_software_sign3[TINYBUFSIZE];
char l_extra[TINYBUFSIZE];
char l_query[TINYBUFSIZE];

void loadlang(char *);


#endif
