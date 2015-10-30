
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

#ifndef _SETUP_H_
#define _SETUP_H_

#include "post.h"

#define SETUP_BACKGROUND       ("setup_bkg")
#define SETUP_BGCOLOR          ("setup_bgc")
#define SETUP_TEXT             ("setup_txt")
#define SETUP_TITLE            ("setup_ttl")
#define SETUP_LINK             ("setup_lnk")
#define SETUP_VLINK            ("setup_vlk")
#define SETUP_FORMINFO         ("setup_frm")

/* #define __LBOUD__ */

#define SETUP_DEF_BACKGROUND   ("")
#define SETUP_DEF_BGCOLOR      ("#000000")
#define SETUP_DEF_TEXT         ("#FFFFFF")
#define SETUP_DEF_TITLE        ("Unknown")
#define SETUP_DEF_LINK         ("#00FFFF")
#define SETUP_DEF_VLINK        ("#C1C1C1")
#define SETUP_DEF_FORMINFO     ("")

#define SETUP_FORMAT_DATETIME  ("%A, %B %d, %Y, %I:%M:%S MST")

int setup_info(char *version);
int setup_openhead(void);
int setup_body(void);
int setup_header(int size);
int setup_input(lpPostElement elements);

#endif /* _SETUP_H_ */
