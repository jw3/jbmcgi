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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "post.h"
#include "setup.h"

/*
  miscellaneous processing routines
*/
char post_x2c(char *x)
{
  register char c;

  /* note: (x & 0xdf) makes x upper case */
  c  = (x[0] >= 'A' ? ((x[0] & 0xdf) - 'A') + 10 : (x[0] - '0'));
  c *= 16;
  c += (x[1] >= 'A' ? ((x[1] & 0xdf) - 'A') + 10 : (x[1] - '0'));
  return(c);
}

/* 
  this function goes through the URL char-by-char 
  and converts all the "escaped" (hex-encoded) 
  sequences to characters 

  this version also converts pluses to spaces. I've 
  seen this done in a separate step, but it seems 
  to me more efficient to do it this way. 
*/

void post_unescape_url(char *url)
{
  register int i, j;

  for(i = 0, j = 0; url[j]; ++i, ++j) 
  {
    if((url[i] = url[j]) == '%') 
    {
        url[i] = post_x2c(&url[j + 1]);
        j += 2;
    }
    else if (url[i] == '+') url[i] = ' ';
  }
  url[i] = '\0';  /* terminate it at the new length */
}

char *post_removefield(char *in, char stop)
{
  register int i, j;
  char *s = NULL;

  if (in == NULL) return NULL;

  /* skip past any spaces */
  while ((*in != '\0') && (*in == ' ')) in++; 

  /* count the number of characters to 'stop' */
  for(i = 0; (in[i] != '\0') && (in[i] != stop); i++);

  s = malloc(i + 1);
  if (s != NULL)
  {
    if (i != 0) strncpy(s, in, i);
    s[i] = '\0';
    post_unescape_url(s);
  }

  /* position past the stop */
  if (in[i] == stop) i++;
  
  for (j = 0; in[j]; i++, j++) in[j] = in[i];

  return s;
}

/*
  error handling functions
*/

/* 
  Prints an error html page to standard out.  This
  function assumes that negative error codes are 
  the codes returned by this header file.  This 
  allows applications to use positive numbers for
  error codes.
*/
void posterr_error(int errornum, char *error)
{
  html_cgitext();
  setup_openhead();
  setup_body();
  setup_info(POST_VERSION);
  setup_header(2);

  html_horzrule(NULL, 0, 0, 0, 0);

  html_writeheader("Error", 2);
  if (errornum < 0) 
    html_string("%s\n", posterrors[-errornum]);
  else 
    html_string("%s\n", error);

  html_closebody();
  html_closehead();
}

char *post_getquerystring(int *error)
{
  char * ct; /* for content-type */
  char * cl; /* for content-length */
  int   icl; /* content-length */
  char * qs; /* query string */
  int i;

  /* grab the content-type and content-length
     and check them for validity */

  *error = 0;

  ct = getenv("CONTENT_TYPE");
  cl = getenv("CONTENT_LENGTH");
  if(cl == NULL)
  {
    *error = POST_NOCONTENTLENGTH;
    return NULL;
  }
  icl = atoi(cl);

  /* do we have a valid query? */
  if(strcmp(ct, "application/x-www-form-urlencoded"))
  {
    *error = POST_INVALIDQUERY;
    return NULL;
  }
  else if (icl == 0)
  {
    *error = POST_ZEROCONTENTLENGTH;
    return NULL;
  }

  /* allocate memory for the input stream */
  if((qs = malloc(icl + 1)) == NULL)
  {
    *error = POST_ERRORMEMALLOC;
    return NULL;
  }

  if((i = fread(qs, icl, 1, stdin)) != 1)
  {
    *error = POST_ERRORREADQUERY;
    return NULL;
  }
  qs[icl] = '\0';

  return qs;
}

lpPostElement post_createelement(lpPostElement next)
{
  lpPostElement e = malloc(sizeof(PostElement));
  if (e != NULL) e->next = next;
  return e;
}

/* destroy a single element, return is the "next" element */
lpPostElement post_destroyelement(lpPostElement element)
{
  lpPostElement e = NULL;
  if (element != NULL)
  {
    e = element->next;
    if (element->name != NULL) free(element->name);
    if (element->val != NULL) free(element->val);
    free(element);
  }
  return e;
}

/* destroy the linked list of elements */
void post_destroyelements(lpPostElement elements)
{
  while (elements != NULL)
  {
    elements = post_destroyelement(elements);
  }
}

/* return a linked list of elements with the name and val fields
   filled */
lpPostElement post_processelements(int *error)
{
  char *qs, *p; 
  lpPostElement e = NULL;

  /* query string */
  qs = post_getquerystring(error);

  if (*error != 0) return NULL;

  if (qs != NULL)
  {
    p = qs;
    while (*p != '\0') 
    {
      e = post_createelement(e);
      if (e != NULL)
      {
        e->name = post_removefield(p, '=');
        e->val = post_removefield(p, '&');
      }
      else break; 
    }
    free(qs);
  }
 
  return e;
}

lpPostElement post_findelement(lpPostElement elements, char *name)
{
  lpPostElement e;
  for (e = elements; e != NULL; e = e->next)
  {
    if(!strcmp(e->name, name)) return e;
  }
  return NULL;
}

float post_asfloat(lpPostElement elements, char *name)
{
  lpPostElement e = post_findelement(elements, name);
  if (e != NULL) return atof(e->val);
  return 0.0;
}

int post_asint(lpPostElement elements, char *name)
{
  lpPostElement e = post_findelement(elements, name);
  if (e != NULL) return atoi(e->val);
  return 0;
}

char *post_aschar(lpPostElement elements, char *name)
{
  lpPostElement e = post_findelement(elements, name);
  if (e != NULL) return e->val;
  return NULL;
}

int post_ascheck(lpPostElement elements, char *name)
{
  return (post_findelement(elements, name) != NULL);
}
