
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "html.h"

/*
  Variables
*/
FILE *html_file = NULL;

/*
  Helper functions
*/
FILE *html_setfile(FILE *file)
{
  FILE *f; 
  if (file == NULL) file = stdout;
  f = html_file;
  html_file = file;
  return f;
}

/*
  Generic Routines
*/
int html_string(char *fmt, char *text)
{
  if ((fmt != NULL) && (text != NULL) && (strlen(text) > 0))
    return fprintf(html_file, fmt, text);
  else
    return 0;
}

int html_int(char *fmt, int number)
{
  if (fmt != NULL)
    return fprintf(html_file, fmt, number);
  else
    return 0;
}

int html_float(char *fmt, double number)
{
  if (fmt != NULL)
    return fprintf(html_file, fmt, number);
  else
    return 0;
}

char *html_formatcolor(int color, char *buffer, int size)
{
  char buf[HTML_LENGTH_COLOR+1]; 
  sprintf(buf, HTML_FORMAT_COLOR, color);
  return strncpy(buffer, buf, size);
}

/*
  Other functions...
*/
int html_return()
{
  return html_string("%s", "\n");
}

int html_comment(char *text)
{
  return (text != NULL ? html_string("<!-- %s -->\n", text) : 0);
}

char *html_env(int variable)
{
  if ((variable >= HTML_ENV_FIRST) && (variable <= HTML_ENV_LAST))
    return (HTML_ENV[variable - HTML_ENV_FIRST]);
  else
    return (NULL);
}

int html_info()
{
  int i, l = 0;
  char *env;
  for (i = HTML_ENV_FIRST; i <= HTML_ENV_LAST; i++)
  {
    env = getenv(html_env(i));
    l += fprintf(html_file, "<!-- %.24s: ", html_env(i));
    l += fprintf(html_file, "\"%s\" -->\n", (env != NULL ? env : ""));
  }
  return l;
}

int html_datetime(char *fmt)
{
  time_t tp;
  char   buf[64];
  tp = time(&tp);
  strftime(buf, sizeof(buf), fmt, localtime(&tp));
  return html_comment(buf);
}

/*
  Skeletal Tags
*/

int html_cgitext()
{
  return fprintf(html_file, "%s\n\n", "Content-type: text/html");
}

int html_openhead(char *title)
{
  return fprintf(html_file, 
    "<HTML><HEAD>\n<TITLE>%s</TITLE>\n</HEAD>\n", title);
}

int html_closehead()
{
  return fprintf(html_file, "</HTML>\n");
}

int html_openbody(char *background, char *bgcolor, 
  char *text, char *link, char *vlink)
{
  int l = html_string("%s", "<BODY");
  if ((background) && (strlen(background) > 0))
    l += html_string(" BACKGROUND=\"%s\"", background);
  if ((bgcolor) && (strlen(bgcolor) > 0))
    l += html_string(" BGCOLOR=\"%s\"", bgcolor);
  if ((text) && (strlen(text)))
    l += html_string(" TEXT=\"%s\"", text);
  if ((link) && (strlen(link)))
    l += html_string(" LINK=\"%s\"", link);
  if ((vlink) && (strlen(vlink)))
    l += html_string(" VLINK=\"%s\"", vlink);
  l += html_string("%s\n", ">");
  return (l);
}

int html_closebody()
{
  return fprintf(html_file, "</BODY>");
}

/* 
  Table Tags
*/

int html_tablealign(int align)
{
  switch (align)
  {
    case HTML_TABLE_ALIGN_CENTER:
      return html_string(" ALIGN=\"%s\"", "CENTER");
    case HTML_TABLE_ALIGN_LEFT:
      return html_string(" ALIGN=\"%s\"", "LEFT");
    case HTML_TABLE_ALIGN_RIGHT:
      return html_string(" ALIGN=\"%s\"", "RIGHT");
  }
  return 0;
}

int html_opentable(int align, int border, int width)
{
  int l = html_string("%s", "<TABLE");
  l += html_tablealign(align);
  if (border) l += html_string(" %s", "BORDER");
  if (width) l += html_int(" WIDTH=\"%d%%\"", width);
  l += html_string("%s\n", ">");
  return l;
}

int html_closetable()
{
  return fprintf(html_file, "</TABLE>\n");
}

int html_tableopenrow(int align)
{
  int l = html_string("%s", "<TR");
  l += html_tablealign(align);
  l += html_string("%s\n", ">");
  return l;
}

int html_tablecloserow()
{
  return html_string("%s\n", "</TR>");
}

int html_tablecaption(int align, char *text)
{
  int l = html_string("<%s", "CAPTION");
  l += html_tablealign(align);
  l += html_string(">%s</CAPTION>\n", text);
  return l;
}

int html_tablecontents(char *content, int align, 
  int colspan, int rowspan, int nowrap, char *text)
{
  int l = html_string("<%s", content);
  l += html_tablealign(align);
  if (colspan) l += html_int(" COLSPAN=\"%d\"", colspan);
  if (rowspan) l += html_int(" ROWSPAN=\"%d\"", rowspan);
  if (nowrap) l += html_string(" %s", "NOWRAP");
  l += html_string(">%s", text);
  l += html_string("</%s>\n", content);
  return l;
}

int html_tabledata(int align, 
  int colspan, int rowspan, int nowrap, char *text)
{
  return html_tablecontents("TD", align, colspan, rowspan, nowrap, text);
}

int html_tableheader(int align, 
  int colspan, int rowspan, int nowrap, char *text)
{
  return html_tablecontents("TH", align, colspan, rowspan, nowrap, text);
}

/*
  Text and Content Markup Tags
*/

int html_break()
{
  return html_string("%s\n", "<BR>");
}

int html_writeheader(char *text, int size)
{
  if ((size >= HTML_HEADING_SIZE_FIRST) && (size <= HTML_HEADING_SIZE_LAST))
    return fprintf(html_file, "<H%d>%s</H%d>\n", size, text, size);
  else
    return 0;
}

int html_opentext(int code)
{
  if ((code <= HTML_TEXT_LAST) && (code >= HTML_TEXT_FIRST))
    return html_string("<%s>", HTML_TEXT_CODES[code]);
  else
    return 0;
}

int html_closetext(int code)
{
  if ((code <= HTML_TEXT_LAST) && (code >= HTML_TEXT_FIRST))
    return html_string("</%s>", HTML_TEXT_CODES[code]);
  else
    return 0;
}

int html_text(int code, char *text)
{
  return html_opentext(code) + 
         html_string("%s", text) + 
         html_closetext(code);
}

/*
  Hyperlinks
*/
int html_href_name(char *name, char* methods, 
   char *rel, char *rev, char *target, char *title)
{
  return html_string("<A NAME=\"%s\"", name)
         + html_string(" METHODS=\"%s\"", methods)
         + html_string(" REL=\"%s\"", rel)
         + html_string(" REV=\"%s\"", rev)
         + html_string(" TARGET=\"%s\"", target)
         + html_string(" TITLE=\"%s\"", title)
         + html_string("%s", ">");
}

int html_href_url(char *url, char* methods, 
   char *rel, char *rev, char *target, char *title)
{
  return html_string("<A HREF=\"%s\"", url)
         + html_string(" METHODS=\"%s\"", methods)
         + html_string(" REL=\"%s\"", rel)
         + html_string(" REV=\"%s\"", rev)
         + html_string(" TARGET=\"%s\"", target)
         + html_string(" TITLE=\"%s\"", title)
         + html_string("%s", ">");
}

/*
  Images
*/
int html_img(char *src, char *align, 
  int height, int width, int hspace, int vspace, char *alt)
{
  return html_string("<IMG SRC=\"%s\"", src)
         + html_string(" ALIGN=\"%s\"", align)
         + html_int(" HEIGHT=\"%d\"", height)
         + html_int(" WIDTH=\"%d\"", width)
         + html_int(" HSPACE=\"%d\"", hspace)
         + html_int(" VSPACE=\"%d\"", vspace)
         + html_string(" ALT=\"%s\"", alt)
         + html_string("%s", ">");
}

/*
  Rules
*/
int html_horzrule(char *align, int noshade, int size, int width, int percent)
{
  int l =  html_string("%s", "<HR") + html_string(" ALIGN=\"%s\"", align);
  if (noshade) l += html_string("%s", " NOSHADE");
  if (size) l += html_int(" SIZE=\"%d\" ", size);
  if (width)
  {
    l += html_int((percent ? " WIDTH=\"%d\"": " WIDTH=\"%d%%\""), size);
  }
  l += html_string("%s\n", ">");
  return l;
}

/*
  Lists
*/

int html_openlist(int list, int start, int type)
{
  int l;
  switch (list)
  {
    case HTML_LIST_DEFINITION: 
      return html_string("<%s>\n", "DL");
      break;
    case HTML_LIST_DIRECTORY:
      return html_string("<%s>\n", "DIR");
      break;
    case HTML_LIST_MENU:
      return html_string("<%s>\n", "MENU");
      break;
    case HTML_LIST_ORDERED:
      l = html_int("<OL START=\"%d\"", start);
      if ((type >= HTML_LIST_TYPE_FIRST) && (type <= HTML_LIST_TYPE_LAST))
        l += html_string(" TYPE=\"%s\"", HTML_LIST_TYPES[type]);
      l = html_string("%s\n", ">");
      break;
    case HTML_LIST_UNORDERED:
      return html_string("<%s>\n", "UL");
      break;
    default: return 0;
  }
}

int html_closelist(int list)
{
  switch (list)
  {
    case HTML_LIST_DEFINITION: 
      return html_string("</%s>\n", "DL");
      break;
    case HTML_LIST_DIRECTORY:
      return html_string("</%s>\n", "DIR");
      break;
    case HTML_LIST_MENU:
      return html_string("</%s>\n", "MENU");
      break;
    case HTML_LIST_ORDERED:
      return html_string("</%s>\n", "OL");
      break;
    case HTML_LIST_UNORDERED:
      return html_string("</%s>\n", "UL");
      break;
    default: return 0;
  }
}

int html_listitem(int list, char *text)
{
  if ((text == NULL) || (strlen(text) == 0)) return 0;

  switch (list)
  {
    case HTML_LIST_DEFINITION: 
      return html_string("  <DD>%s\n", text);
      break;
    case HTML_LIST_DEFINITIONTERM: 
      return html_string("  <DT>%s\n", text);
      break;
    case HTML_LIST_DIRECTORY:
    case HTML_LIST_MENU:
    case HTML_LIST_ORDERED:
    case HTML_LIST_UNORDERED:
      return html_string("  <LI>%s\n", text);
      break;
    default: return 0;
  }
}

int html_list(int list, int start, int type, char *item, ...)
{
  int l = 0;
  va_list ap;

  va_start(ap, item);

  if ((l = html_openlist(list, start, type)) != 0)
  {
    while ((item != NULL) && (strlen(item) > 0))
    {
      l += html_listitem(list, item);
      item = va_arg(ap, char *);
    }
    l += html_closelist(list);
  }

  va_end(ap);

  return l;
}

/*
  Compound functions
*/
int html_writetablevalue(char *descrip, char *units, char *format, double value)
{
  char buf[64];
  int l = html_tableopenrow(HTML_TABLE_ALIGN_NONE);
  l += html_tabledata(HTML_TABLE_ALIGN_NONE, 1, 1, 0, descrip);
  sprintf(buf, format, value);
  l += html_tabledata(HTML_TABLE_ALIGN_RIGHT, 1, 1, 0, buf);
  l += html_tabledata(HTML_TABLE_ALIGN_LEFT, 1, 1, 0, units);
  l += html_tablecloserow();
  return l;
}

int html_writetablestring(char *descrip, char *units, char *format, char *value)
{
  int l = html_tableopenrow(HTML_TABLE_ALIGN_NONE);
  l += html_tabledata(HTML_TABLE_ALIGN_NONE, 1, 1, 0, descrip);
  l += html_tabledata(HTML_TABLE_ALIGN_RIGHT, 1, 1, 0, value);
  l += html_tabledata(HTML_TABLE_ALIGN_LEFT, 1, 1, 0, units);
  l += html_tablecloserow();
  return l;
}

int html_writetablebreak()
{
  int l = html_tableopenrow(HTML_TABLE_ALIGN_NONE);
  l += html_tabledata(HTML_TABLE_ALIGN_NONE, 3, 1, 0, "<HR>");
  l += html_tablecloserow();
  return l;
}

int html_writetabledata(char *format, double value)
{
  char buf[64];
  sprintf(buf, format, value);
  return html_tabledata(HTML_TABLE_ALIGN_CENTER, 1, 1, 0, buf);
}
