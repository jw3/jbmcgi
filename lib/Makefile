
#
# Copyright (C) 2000 James B. Millard, jbm@cybermesa.com
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
# 02111-1307, USA.
#

#
# Make library libjbm.a
#

#
# Configuration
#
INCDIR=../include
LIBDIR=../lib

INC=-I${INCDIR}
LIB=-L${LIBDIR} -ljbm -lm
CFLAGS=${INC}

# Files
SRCS=atmos.c bc.c drag.c options.c post.c html.c setup.c vectors.c
OBJS=${SRCS:.c=.o}

#
# Targets
#
TGT=libjbm.a

${TGT}: ${OBJS}
	ar r $@ ${OBJS}

#
# Dependancies
#
atmos.o: atmos.c ${INCDIR}/atmos.h
bc.o: bc.c ${INCDIR}/bc.h ${INCDIR}/drag.h
drag.o: drag.c ${INCDIR}/drag.h
options.o: options.c ${INCDIR}/options.h
post.o: post.c ${INCDIR}/post.h
html.o: html.c ${INCDIR}/html.h
setup.o: setup.c ${INCDIR}/post.h ${INCDIR}/html.h ${INCDIR}/setup.h
vectors.c: ${INCDIR}/vectors.h

#
# Housekeeping
#
clean:
	rm -f ${OBJS}
	rm -f core

clobber: clean
	rm -f ${TGT}
