AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I`root-config --incdir`

lib_LTLIBRARIES = \
   libmyjetanalysis.la

libmyjetanalysis_la_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib

libmyjetanalysis_la_LIBADD = \
  -lfun4all \
  -lg4dst \
  -lg4eval

if ! MAKEROOT6
  ROOT5_DICTS = \
    MyJetAnalysis_Dict.cc
endif

libmyjetanalysis_la_SOURCES = \
  $(ROOT5_DICTS) \
  MyJetAnalysis.cc

pkginclude_HEADERS = \
  MyJetAnalysis.h


%_Dict.cc: %.h %LinkDef.h
	rootcint -f $@ @CINTDEFS@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

#just to get the dependency, in case we ever add an i/o dictionary
%_Dict_rdict.pcm: %_Dict.cc ;

################################################
# linking tests

noinst_PROGRAMS = testexternals

BUILT_SOURCES = \
  testexternals.cc

testexternals_SOURCES = \
  testexternals.cc

testexternals_LDADD = \
  libmyjetanalysis.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f *Dict* $(BUILT_SOURCES) *.pcm 
