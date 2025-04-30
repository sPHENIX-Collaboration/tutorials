AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -isystem$(OFFLINE_MAIN)/include \
  -isystem$(ROOTSYS)/include

lib_LTLIBRARIES = libmytree.la   

AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib

libmytree_la_LIBADD = \
  -lfun4all \
  -lphool \
  -lSubsysReco

ROOTDICTS = \
  MyTClonesArray_Dict.cc \
  MySimpleTree_Dict.cc

pcmdir = $(libdir)

nobase_dist_pcm_DATA = \
  MyTClonesArray_Dict_rdict.pcm \
  MySimpleTree_Dict_rdict.pcm

libmytree_la_SOURCES = \
  $(ROOTDICTS) \
  AnalyzeSimpleTree.cc \
  MakeSimpleTree.cc \
  MyTClonesArray.cc \
  MySimpleTree.cc

pkginclude_HEADERS = \
  AnalyzeSimpleTree.h \
  MakeSimpleTree.h

BUILT_SOURCES = \
  testexternals.cc

testexternals_SOURCES = \
  testexternals.cc

noinst_PROGRAMS = \
  testexternals

testexternals_LDADD = \
  libmytree.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

%_Dict.cc: %.h %LinkDef.h
	rootcint -f $@ @CINTDEFS@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

#just to get the dependency
%_Dict_rdict.pcm: %_Dict.cc ;


clean-local:
	rm -f *Dict* $(BUILT_SOURCES) *.pcm 

