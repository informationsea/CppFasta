AM_LDFLAGS = -no-undefined
AM_CPPFLAGS = -Wall -Wextra

#bin_PROGRAMS = frozenhashmgr
#frozenhashmgr_SOURCES = manager.cpp
#frozenhashmgr_LDADD = libfrozenhashmap.la

lib_LTLIBRARIES = libcppfasta.la
libcppfasta_la_SOURCES = cppfasta.cpp filereader.cpp
libcppfasta_la_LIBADD = -lz -lbz2

libcppfasta_la_includedir = $(includedir)/cppfasta
libcppfasta_la_include_HEADERS = \
	cppfasta.hpp filereader.hpp


