
AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = \
    libPHFlowJetMaker.la

INCLUDES = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I$(ROOTSYS)/include

noinst_HEADERS = \
  PHFlowJetMakerLinkDef.h \
  PHFlowJetMaker.h

libPHFlowJetMaker_la_SOURCES = \
  PHFlowJetMaker.cc 

nodist_libPHFlowJetMaker_la_SOURCES = \
  PHFlowJetMaker_Dict.cc

libPHFlowJetMaker_la_LDFLAGS = \
   -L$(libdir) \
   -L$(OFFLINE_MAIN)/lib \
   -lcemc \
   -lfun4all \
   -lphg4hit \
   -lg4detectors \
   -lg4testbench \
   -lfastjet \
   -lg4hough \
   -lCGAL

################################################
# linking tests

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = testexternals.cc
testexternals_LDADD = libPHFlowJetMaker.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

CLEANFILES = PHFlowJetMaker_Dict.cc

# rules to make root dictionaries
PHFlowJetMaker_Dict.cc: \
  PHFlowJetMaker.h \
  PHFlowJetMakerLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^
