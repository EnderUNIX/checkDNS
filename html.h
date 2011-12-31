#ifndef HTML_H
#define HTML_H

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

int html_output_start(FILE *fd);
int html_output_table(FILE *fd, int no, const dnsrec *drec);
int html_output_finish(FILE *fd);
int write_html(const char *type);

#endif
