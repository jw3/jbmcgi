
/*
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

#include <time.h>
#include <string.h>

#include "html.h"
#include "post.h"
#include "setup.h"

/*
*/

char setup_title[64];
char setup_background[256];
char setup_bgcolor[32];
char setup_text[32];
char setup_link[32];
char setup_vlink[32];
char setup_forminfo[256];

int setup_info(char *version)
{
  char build[256];
  setup_build(build, sizeof(build));

  return html_return() +
         html_info() +
         html_return() +
         html_comment(version) +
         html_comment(build) + 
         html_comment(setup_forminfo) +
         html_datetime(SETUP_FORMAT_DATETIME) +
         html_return();
}

int setup_build(char *buffer, int size)
{
  strncpy(buffer, "Build date-time: " __DATE__ " " __TIME__, size);
}

int setup_footer()
{
}

int setup_openhead()
{
  return html_openhead(setup_title);
}

int setup_body()
{
  return html_openbody(setup_background,
    setup_bgcolor, setup_text, setup_link, setup_vlink);
}

int setup_header(int size)
{
  return html_writeheader(setup_title, size);
}

int setup_input(lpPostElement elements)
{
  char *c;

  if (elements != NULL)
  {
    c = post_aschar(elements, SETUP_TITLE);
    if (c != NULL) 
      strncpy(setup_title, c, sizeof(setup_title));
    else
      strncpy(setup_title, SETUP_DEF_TITLE, sizeof(setup_title));
      
  
    c = post_aschar(elements, SETUP_BACKGROUND);
    if (c != NULL) 
      strncpy(setup_background, c, sizeof(setup_background));
    else
      strncpy(setup_background, "", sizeof(setup_background));
  
    c = post_aschar(elements, SETUP_BGCOLOR);
    if (c != NULL) 
      strncpy(setup_bgcolor, c, sizeof(setup_bgcolor));
    else
      strncpy(setup_bgcolor, SETUP_DEF_BGCOLOR, sizeof(setup_bgcolor));
  
    c = post_aschar(elements, SETUP_TEXT);
    if (c != NULL) 
      strncpy(setup_text, c, sizeof(setup_text));
    else
      strncpy(setup_text, SETUP_DEF_TEXT, sizeof(setup_text));
  
    c = post_aschar(elements, SETUP_LINK);
    if (c != NULL) 
      strncpy(setup_link, c, sizeof(setup_link));
    else
      strncpy(setup_link, SETUP_DEF_LINK, sizeof(setup_link));
  
    c = post_aschar(elements, SETUP_VLINK);
    if (c != NULL) 
      strncpy(setup_vlink, c, sizeof(setup_vlink));
    else
      strncpy(setup_vlink, SETUP_DEF_VLINK, sizeof(setup_vlink));
  
    c = post_aschar(elements, SETUP_FORMINFO);
    if (c != NULL) 
      strncpy(setup_forminfo, c, sizeof(setup_forminfo));
    else
      strncpy(setup_forminfo, SETUP_DEF_FORMINFO, sizeof(setup_forminfo));
  }
  
  return (1);
}
