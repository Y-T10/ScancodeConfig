#pragma once

#include <type_traits>
#include <string>

#include <windows.h>

namespace CompReg {
    /// Win32APIのChar型．
    using win32api_char = std::remove_pointer_t<LPSTR>;
    /// Win32APIのChar型を扱う文字列型．
    using win32str = std::basic_string<win32api_char>;
};