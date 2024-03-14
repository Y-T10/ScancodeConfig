#include "CsmViewer.hpp"
#include <cstdio>
#include <cstddef>
#include <optional>
#include <string>
#include <vector>
#include <winnls.h>

#ifdef _WIN32
    #ifndef STRICT
    #define STRICT
    #endif
    #ifndef NOMIMAX
    #define NOMIMAX
    #endif
    #include <windows.h>
#endif

namespace {
    const LONG MakeLParam(const CompScanMap::Scancode code) noexcept {
        const LONG LpScancode = (LONG)(0xFF0000) & (LONG)(code << 16);
        if(!(code & 0xff00)){
        return LpScancode;
        }
        return 0x1000000 | LpScancode;
    }

    const std::optional<std::wstring> ACPToUTF16(const std::string& str) noexcept {
        if(str.empty()) {
            return L"";
        }
        const int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), -1, nullptr, 0);
        if(!size) {
            return std::nullopt;
        }
        std::vector<wchar_t> output(size, '\0');
        if(!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), -1, output.data(), output.size())) {
            return std::nullopt;
        }
        return std::wstring(output.begin(), output.end()-1);
    };

    template <class wide_char_type = wchar_t>
    const std::optional<std::string> UTF16To8(const std::basic_string<wide_char_type>& str) noexcept {
        if(str.empty()) {
            return "";
        }
        const int size = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if(!size) {
            return std::nullopt;
        }
        std::vector<char> output(size, '\0');
        if(!WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, output.data(), output.size(), nullptr, nullptr)) {
            return std::nullopt;
        }
        return std::string(output.begin(), output.end()-1);
    };

    const std::optional<std::string> ToUTF8(const std::basic_string<TCHAR>& str) noexcept {
        constexpr auto UNICODEDefined = [](){
            #ifdef UNICODE
            return true;
            #else
            return false;
            #endif
        };

        if constexpr (UNICODEDefined()) {
            return UTF16To8(str);
        } else {
            const auto wstr = ACPToUTF16(str);
            if (!wstr.has_value()) {
                return std::nullopt;
            }
            return UTF16To8(wstr.value());
        }
    }
}

namespace CompScanMap {
    const std::optional<std::string> ScancodeName(const Scancode code) noexcept {
        constexpr size_t keyNameLength = 128;
        TCHAR keyName[keyNameLength]="";

        if (code == 0x0000) {
            return "Null";
        }
        
        const int NameSize = GetKeyNameText(
            MakeLParam(code), keyName, keyNameLength
        );

        // キー名がある
        if (NameSize>0){
            return ToUTF8(keyName);
        }

        // キー名がない
        if (GetLastError() == ERROR_SUCCESS) {
            return std::string("");
        }

        // それ以外
        return std::nullopt;
    }
}