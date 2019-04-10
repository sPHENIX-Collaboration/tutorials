AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include 

lib_LTLIBRARIES = libmytree.la   

AM_LDFLAGS = -L$(libdir) -L$(OFFLINE_MAIN)/lib

libmytree_la_LIBADD = \
  -lfun4all \
  -lphool \
  -lSubsysReco

# I/O dictionaries have to exist for root5 and root6. For ROOT6 we need
# pcm files in addition. If someone can figure out how to make a list
# so this list of dictionaries is transformed into a list of pcm files
# following a simple naming convention, please change this accordingly and
# let me know
ROOTDICTS = \
  MyTClonesArray_Dict.cc \
  MySimpleTree_Dict.cc
# for root6 we need pcm and dictionaries but only for
# i/o classes. For root5 we need only dictionaries but
# those for i/o and classes available on the cmd line
# MAKEROOT6 is set in the configure.ac, true for root6
if MAKEROOT6
# this is a tweak to install files in $(libdir), automake refuses
# to install other files in libdir, this construct gets around this
pcmdir = $(libdir)
nobase_dist_pcm_DATA = \
  MyTClonesArray_Dict_rdict.pcm \
  MySimpleTree_Dict_rdict.pcm
else
  ROOT5_DICTS = \
    AnalyzeSimpleTree_Dict.cc \
    MakeSimpleTree_Dict.cc
endif

libmytree_la_SOURCES = \
  $(ROOTDICTS) \
  $(ROOT5_DICTS) \
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

