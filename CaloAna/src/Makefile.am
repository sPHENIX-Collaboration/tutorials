AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = \
    libcaloana.la

AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib

INCLUDES = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I$(ROOTSYS)/include


libcaloana_la_SOURCES = \
  CaloAna.cc 

nodist_libcaloana_la_SOURCES = \
  CaloAna_Dict.C

libcaloana_la_LDFLAGS = \
   -L$(libdir) \
   -L$(OFFLINE_MAIN)/lib \
   -lcemc \
   -lfun4all \
   -lphg4hit \
   -lg4detectors


################################################
# linking tests

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = testexternals.C
testexternals_LDADD = libcaloana.la

testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

CLEANFILES = CaloAna_Dict.C CaloAna_Dict.h

# rules to make root dictionaries
CaloAna_Dict.C: \
  CaloAna.h \
  CaloAnaLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

