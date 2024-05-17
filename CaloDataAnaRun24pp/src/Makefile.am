AUTOMAKE_OPTIONS = foreign

CINTINCLUDES = -pthread -I$(includedir) -I$(OFFLINE_MAIN)/lib -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include -I$(ROOTSYS)/lib/include -I$(OPT_SPHENIX)/include

INCLUDES = $(CINTINCLUDES)

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I$(ROOTSYS)/include

AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -L$(OFFLINE_MAIN)/lib64 \
  -lcalo_io \
  -lCLHEP \
  -lglobalvertex \
  -lglobalvertex_io \
  -lffarawmodules

ROOTDICTS = caloTreeGen_Dict.cc

pcmdir = $(libdir)

nobase_dist_pcm_DATA = caloTreeGen_Dict_rdict.pcm

pkginclude_HEADERS = \
  caloTreeGen.h

lib_LTLIBRARIES = \
  libcaloTreeGen.la

libcaloTreeGen_la_SOURCES = \
  caloTreeGen.cc  \
  $(ROOTDICTS)

libcaloTreeGen_la_LIBADD = \
  -lphool \
  -lSubsysReco \
  -lffarawmodules

BUILT_SOURCES = testexternals.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = testexternals.cc
testexternals_LDADD   = libcaloTreeGen.la

CALOTREEGENOBJS=caloTreeGen.o derp.o

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@
#rootcint -f $@ -c $(DEFAULT_INCLUDES) $(CINTINCLUDES) $^ caloTreeGen.cc -c 
caloTreeGen_Dict.cc: caloTreeGen.h caloTreeGenLinkDef.h
	rootcint -f $@ @CINTDEFS@ $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

caloTreeGen_Dict_rdict.pcm: caloTreeGen_Dict.cc ;

clean-local:
	rm -f *Dict*  $(BUILT_SOURCES) *.pcm
