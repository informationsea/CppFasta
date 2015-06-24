AUTOMAKE_OPTIONS = subdir-objects
SUBDIRS = data gtest
AM_CPPFLAGS = -Wall -Wextra

noinst_PROGRAMS = test_fasta test_filereader test_filewriter
TESTS = $(noinst_PROGRAMS)

LDADD = -lpthread $(top_builddir)/src/libcppfasta.la $(top_builddir)/test/gtest/libgtest.la

test_fasta_SOURCES = test_fasta.cpp
test_filereader_SOURCES = test_filereader.cpp
test_filewriter_SOURCES = test_filewriter.cpp
