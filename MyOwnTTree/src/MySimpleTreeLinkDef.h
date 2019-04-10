#ifdef __CINT__
// The + tells root to use root v3 streamers
// (again root'ish for we want to save this
// on a file). This will at some point will become the default
// (maybe it is nowadays)
#pragma link C++ class MySimpleTree+;

#endif /* __CINT__ */
