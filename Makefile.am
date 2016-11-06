utk_includedir = $(top_srcdir)/include/utk

ACLOCAL_AMFLAGS = -I m4 --install
AUTOMAKE_OPTIONS = foreign

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = utk.pc

pkginclude_HEADERS = $(utk_includedir)/array.h \
		     $(utk_includedir)/list.h \
		     $(utk_includedir)/math.h \
		     $(utk_includedir)/log.h \
		     $(utk_includedir)/vt102.h \
		     $(utk_includedir)/str.h \
		     $(utk_includedir)/io.h \
		     $(utk_includedir)/unit.h

SUBDIRS = src tests
