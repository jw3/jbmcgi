
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
#  makefile to build all cgis...
# 

#
# Configuration
#
SHELL=/bin/sh
ECHO=/bin/echo

# "target" directory -- where the executables are put
TDIR=.
# current (root) directory
HDIR=.

# Targets
TGTS=libjbm.a jbmbcv.cgi jbmbct.cgi jbmdrag.cgi jbmmdist.cgi \
     jbmpbr.cgi jbmsun.cgi jbmtraj.cgi

#
# Individual targets
#
usage: dousage

all: ${TGTS}

libjbm.a:
	cd lib; ${MAKE}

jbmbct.cgi: libjbm.a
	cd ${HDIR}/bct; ${MAKE}
	mv ${HDIR}/bct/$@ ${TDIR}; 

jbmbcv.cgi: libjbm.a
	cd ${HDIR}/bcv; ${MAKE}
	mv ${HDIR}/bcv/$@ ${TDIR}; 

jbmdrag.cgi: libjbm.a
	cd ${HDIR}/drag; ${MAKE} 
	mv ${HDIR}/drag/$@ ${TDIR};

jbmmdist.cgi: libjbm.a
	cd ${HDIR}/maxdist; ${MAKE} 
	mv ${HDIR}/maxdist/$@ ${TDIR};

jbmpbr.cgi: libjbm.a
	cd ${HDIR}/pbrange; ${MAKE}
	mv ${HDIR}/pbrange/$@ ${TDIR};  

jbmsun.cgi: libjbm.a
	cd ${HDIR}/sun; ${MAKE}
	mv ${HDIR}/sun/$@ ${TDIR}; 

jbmtraj.cgi: libjbm.a
	cd ${HDIR}/traj; ${MAKE}
	mv ${HDIR}/traj/$@ ${TDIR};

#
# Housekeeping
#
clean:
	find ${TDIR} -name "*.o" -exec rm -f {} \;

clobber: clean
	for TGT in ${TGTS}; do \
	  find ${TDIR} -name $$TGT -exec rm -f {} \; ; \
	done

dousage:
	@ ${ECHO} "Targets:"
	@ ${ECHO} "  usage        -- shows this message"
	@ ${ECHO} "  all          -- make all CGIs"
	@ ${ECHO} " "
	@ ${ECHO} "  clean        -- removes ALL object files"
	@ ${ECHO} "  clobber      -- cleans, and removes programs/libraries"
	@ ${ECHO} " "
	@ ${ECHO} "  libjbm.a     -- common library for all CGIs"
	@ ${ECHO} "  jbmbct.cgi   -- BC (time)"
	@ ${ECHO} "  jbmbcv.cgi   -- BC (velocity)"
	@ ${ECHO} "  jbmdrag.cgi  -- Drag/Twist"
	@ ${ECHO} "  jbmmdist.cgi -- Maximum Distance"
	@ ${ECHO} "  jbmpbr.cgi   -- Point Blank Range"
	@ ${ECHO} "  jbmsun.cgi   -- Sunrise/Sunset"
	@ ${ECHO} "  jbmtraj.cgi  -- Trajectories"
