
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

#ifndef _HTML_H_
#define _HTML_H_

#include <stdio.h>

/*
  Defines...
*/

#define HTML_LENGTH_COLOR           (7)
#define HTML_FORMAT_COLOR           ("#%.6X")

#define HTML_TEXT_BOLD              (0)
#define HTML_TEXT_BIG               (1)
#define HTML_TEXT_BLINK             (2)
#define HTML_TEXT_CENTER            (3)
#define HTML_TEXT_CITATION          (4)
#define HTML_TEXT_CODE              (5)
#define HTML_TEXT_DEFINITION        (6)
#define HTML_TEXT_DIVISION          (7)
#define HTML_TEXT_EMPHASIS          (8)
#define HTML_TEXT_EXAMPLE           (9)
#define HTML_TEXT_ITALIC            (10)
#define HTML_TEXT_KEYBOARD          (11)
#define HTML_TEXT_PARAGRAPH         (12)
#define HTML_TEXT_PREFORMATTED      (13)
#define HTML_TEXT_SAMPLE            (14)
#define HTML_TEXT_SMALL             (15)
#define HTML_TEXT_STRIKEOUT         (16)
#define HTML_TEXT_STRONG            (17)
#define HTML_TEXT_SUBSCRIPT         (18)
#define HTML_TEXT_SUPSCRIPT         (19)
#define HTML_TEXT_TYPEWRITER        (20)
#define HTML_TEXT_VARIABLE          (21)

#define HTML_TEXT_FIRST             (HTML_TEXT_BOLD)
#define HTML_TEXT_LAST              (HTML_TEXT_VARIABLE)

#define HTML_HREF_RELREV_NEXT	    ("NEXT")
#define HTML_HREF_RELREV_PREV	    ("PREV")
#define HTML_HREF_RELREV_HEAD	    ("HEAD")
#define HTML_HREF_RELREV_TOC 	    ("TOC")
#define HTML_HREF_RELREV_PARENT     ("PARENT")
#define HTML_HREF_RELREV_CHILD      ("CHILD")
#define HTML_HREF_RELREV_INDEX      ("INDEX")
#define HTML_HREF_RELREV_GLOSSARY   ("GLOSSARY")

#define HTML_HREF_TARGET_BLANK      ("_blank")
#define HTML_HREF_TARGET_PARENT     ("_parent")
#define HTML_HREF_TARGET_SELF       ("_self")
#define HTML_HREF_TARGET_TOP        ("_top")

#define HTML_IMG_ALIGN_TOP          ("top")
#define HTML_IMG_ALIGN_MIDDLE       ("middle")
#define HTML_IMG_ALIGN_BOTTOM       ("bottom")

#define HTML_HEADING_SIZE_1         (1)
#define HTML_HEADING_SIZE_2         (2)
#define HTML_HEADING_SIZE_3         (3)
#define HTML_HEADING_SIZE_4         (4)
#define HTML_HEADING_SIZE_5         (5)
#define HTML_HEADING_SIZE_6         (6)

#define HTML_HEADING_SIZE_FIRST     (HTML_HEADING_SIZE_1)
#define HTML_HEADING_SIZE_LAST      (HTML_HEADING_SIZE_6)

#define HTML_LIST_DEFINITION        (1)
#define HTML_LIST_DEFINITIONTERM    (2)
#define HTML_LIST_DIRECTORY         (3)
#define HTML_LIST_MENU              (4)
#define HTML_LIST_ORDERED           (5)
#define HTML_LIST_UNORDERED         (6)

#define HTML_LIST_TYPE_CAP          (0)
#define HTML_LIST_TYPE_LOW          (1)
#define HTML_LIST_TYPE_CAPROMAN     (2)
#define HTML_LIST_TYPE_LOWROMAN     (3)
#define HTML_LIST_TYPE_ARABIC       (4)

#define HTML_LIST_TYPE_FIRST        (HTML_LIST_TYPE_CAP)
#define HTML_LIST_TYPE_LAST         (HTML_LIST_TYPE_ARABIC)

#define HTML_TABLE_ALIGN_NONE       (0)
#define HTML_TABLE_ALIGN_CENTER     (1)
#define HTML_TABLE_ALIGN_LEFT       (2)
#define HTML_TABLE_ALIGN_RIGHT      (3)

#define HTML_ENV_AUTH_TYPE          (1)
#define HTML_ENV_CONTENT_LENGTH     (2)
#define HTML_ENV_CONTENT_TYPE       (3)
#define HTML_ENV_GATEWAY_INTERFACE  (4)
#define HTML_ENV_HTTP_USER_AGENT    (5)
#define HTML_ENV_HTTP_ACCEPT        (6)
#define HTML_ENV_PATH_INFO          (7)
#define HTML_ENV_PATH_TRANSLATED    (8)
#define HTML_ENV_QUERY_STRING       (9)
#define HTML_ENV_REQUEST_METHOD     (10)
#define HTML_ENV_REMOTE_HOST        (11)
#define HTML_ENV_REMOTE_ADDR        (12)
#define HTML_ENV_REMOTE_USER        (13)
#define HTML_ENV_REMOTE_IDENT       (14)
#define HTML_ENV_SCRIPT_NAME        (15)
#define HTML_ENV_SERVER_SOFTWARE    (16)
#define HTML_ENV_SERVER_NAME        (17)
#define HTML_ENV_SERVER_PORT        (18)
#define HTML_ENV_SERVER_PROTOCOL    (19)

#define HTML_ENV_FIRST              (1)
#define HTML_ENV_LAST               (19)

static char *HTML_ENV[] =
{
  "AUTH_TYPE",
  "CONTENT_LENGTH",
  "CONTENT_TYPE",
  "GATEWAY_INTERFACE",
  "HTTP_USER_AGENT",
  "HTTP_ACCEPT",
  "PATH_INFO",
  "PATH_TRANSLATED",
  "QUERY_STRING",
  "REQUEST_METHOD",
  "REMOTE_HOST",
  "REMOTE_ADDR",
  "REMOTE_USER",
  "REMOTE_IDENT",
  "SCRIPT_NAME",
  "SERVER_SOFTWARE",
  "SERVER_NAME",
  "SERVER_PORT",
  "SERVER_PROTOCOL"
};

static char *HTML_LIST_TYPES[] =
{
  "A",
  "a",
  "I",
  "i",
  "1"
};

static char *HTML_TEXT_CODES[] = 
{
  "B",
  "BIG",
  "BLINK",
  "CENTER",
  "CITE",
  "CODE",
  "DFN",
  "DIV",
  "XMP",
  "EM",
  "I",
  "KBD",
  "P",
  "PRE",
  "SAMP",
  "SMALL",
  "STRIKE",
  "STRONG",
  "SUB",
  "SUP",
  "TT",
  "VAR"
};

/* 
  Prototypes 
*/

/* Helper functions */

FILE *html_setfile(FILE *file);

/* Generic Routines */

int html_string(char *fmt, char *text);
int html_int(char *fmt, int number);
int html_float(char *fmt, double number);
char *html_formatcolor(int color, char *buffer, int size);
int html_datetime(char *fmt);

/* Other functions */

int html_return(void);
int html_comment(char *text);
char *html_env(int variable);
int html_info(void);

/* Skeletal Tags */

int html_cgitext(void);
int html_openhead(char *title);
int html_closehead(void);
int html_openbody(char *background, char *bgcolor, 
  char *text, char *link, char *vlink);
int html_closebody(void);

/* Table Tags */

int html_tablealign(int align);
int html_opentable(int align, int border, int width);
int html_closetable(void);
int html_tableopenrow(int align);
int html_tablecloserow(void);
int html_tablecaption(int align, char *text);
int html_tablecontents(char *content,
  int align, int colspan, int rowspan, int nowrap, char *text);
int html_tabledata(int align, 
  int colspan, int rowspan, int nowrap, char *text);
int html_tableheader(int align, 
  int colspan, int rowspan, int nowrap, char *text);


/* Text and Content Markup Tags */

int html_break(void);
int html_writeheader(char *text, int size);
int html_opentext(int code);
int html_closetext(int code);
int html_text(int code, char *text);

/* Hyperlinks */

int html_href_name(char *name, char* methods, 
   char *rel, char *rev, char *target, char *title);
int html_href_url(char *url, char* methods, 
   char *rel, char *rev, char *target, char *title);

/* Images */

int html_img(char *src, char *align, 
  int height, int width, int hspace, int vspace, char *alt);

/* Rules */

int html_horzrule(char *align, int noshade, int size, int width, int percent);

/*
  Lists
*/

int html_openlist(int list, int start, int type);
int html_closelist(int list);
int html_listitem(int list, char *text);
int html_list(int list, int start, int type, char *item, ...);

/*
  Compound functions
*/
int html_writetablevalue(char *descrip, 
  char *units, char *format, double value);
int html_writetablebreak(void);
int html_writetablestring(char *descrip, 
  char *units, char *format, char *value);
int html_writetabledata(char *format, double value);


#endif /* _HTML_H_ */
