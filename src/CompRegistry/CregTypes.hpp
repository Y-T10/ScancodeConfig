#pragma once

#include <string>

#ifndef STRICT
#define STRICT
#endif
#ifndef NOMIMAX
#define NOMIMAX
#endif
#include <windows.h>

namespace CompReg {
    /// Win32APIの文字型を扱う文字列型．
    using win32str = std::basic_string<TCHAR>;
};