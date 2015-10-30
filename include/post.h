/*

  Copyright (c) 1995, 1996, 1997, All Rights Reserved
  
  James B. Millard, P.O. Box 3648, University Park, NM  88003  

  The following two functions:
    char post_x2c(char*);
    void post_unescape_url(char*);
  are from "The CGI Book", by William E. Weinman.  I claim NO
  copyright (or any other rights) these functions. 

  Copyright (C) 2000 James B. Millard, jbm@cybermesa.com

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
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
  02111-1307, USA.

*/

#ifndef _POST_H_
#define _POST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* 
  this is the structure we use 
  for the query-string elements 
*/
#define POST_NOCONTENTLENGTH   (-1)
#define POST_INVALIDQUERY      (-2)
#define POST_ZEROCONTENTLENGTH (-3)
#define POST_ERRORMEMALLOC     (-4)
#define POST_ERRORREADQUERY    (-5)

static char *posterrors[] =
{
  "None",
  "No content length",
  "Invalid query",
  "Zero content length",
  "Error allocating content memory",
  "Error reading query"
};

/*
  version
*/
#define POST_VERSION   "post.c, version 2.0, 1997, jbm@lascruces.com"

typedef struct postelement *lpPostElement;

typedef struct postelement
{
  lpPostElement next;
  char         *name;
  char         *val;
} PostElement;

/* function prototypes */

/* miscellaneous processing */
char post_x2c(char*);
void post_unescape_url(char*);
char *post_removefield(char*, char);

/* error handling */
void posterr_error(int, char*);

/* creation functions */
char *post_getquerystring(int*);
lpPostElement post_createelement(lpPostElement);
lpPostElement post_destroyelement(lpPostElement);
void post_destroyelements(lpPostElement);
lpPostElement post_processelements(int*);

/* Querying functions */
lpPostElement post_findelement(lpPostElement, char*);
int post_asint(lpPostElement, char*);
float post_asfloat(lpPostElement, char*);
char *post_aschar(lpPostElement, char*);
int post_ascheck(lpPostElement, char*);

#endif /* _POST_H_ */
