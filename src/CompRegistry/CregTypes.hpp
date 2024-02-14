#pragma once

#include <string>

#include <windows.h>

namespace CompReg {
    /// Win32APIの文字型を扱う文字列型．
    using win32str = std::basic_string<TCHAR>;
};