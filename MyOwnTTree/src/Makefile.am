AUTOMAKE_OPTIONS = foreign

BUILT_SOURCES = \
  testexternals.cc

INCLUDES = -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include 

lib_LTLIBRARIES = libmytree.la   

AM_LDFLAGS = -L$(libdir) -L$(OFFLINE_MAIN)/lib

libmytree_la_LIBADD = \
  -lfun4all \
  -lphool \
  -lSubsysReco

libmytree_la_SOURCES = \
  AnalyzeSimpleTree.cc \
  MakeSimpleTree.cc \
  MyTClonesArray.cc \
  MySimpleTree.cc \
  MyTreeDict.cc

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

MyTreeDict.cc : \
  AnalyzeSimpleTree.h \
  MakeSimpleTree.h \
  MySimpleTree.h \
  MyTClonesArray.h \
  MyTreeLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

clean-local:
	rm -f *Dict.*
