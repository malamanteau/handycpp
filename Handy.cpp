
/// Some things just simply can't be header only. If I inlined these 
/// implementations, I would be also including Windows.h in EVERYTHING
/// that included Handy, and I simply don't want that. At the same time,
/// I want using Handy to be as easy as including Handy.hpp and compiling
/// Handy.cpp.

/// So...
#include "isrc/SystemInfo.cpp"
#include "isrc/HandyConsole.cpp"
#include "isrc/HandyGuid.cpp"
#include "isrc/HandyLoader.cpp"
#include "isrc/stb_image.cpp"
#include "isrc/HandyThreadUtils.cpp"
#include "isrc/HandyMMFile.cpp"
