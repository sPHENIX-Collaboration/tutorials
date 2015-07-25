#ifdef __CINT__
// -! means no streamers which is root'ish for
// we don't want to save this class on a file
// (who wants to save a reconstruction module
// in a file - no point to that)
#pragma link C++ class AnalyzeSimpleTree-!;
#pragma link C++ class MakeSimpleTree-!;
// The + tells root to use root v3 streamers
// (again root'ish for we want to save this
// on a file). This will at some point will become the default
// (maybe it is nowadays)
#pragma link C++ class MySimpleTree+;
#pragma link C++ class MyTClonesArray+;

#endif /* __CINT__ */
