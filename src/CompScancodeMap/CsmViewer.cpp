#include "CsmViewer.hpp"
#include <cstdio>
#include <cstddef>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>
#include <winnls.h>

#include "WindowsScancodes.hpp"

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

    const std::expected<std::wstring, int> ACPToUTF16(const std::string& str) noexcept {
        if(str.empty()) {
            return L"";
        }
        const int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), -1, nullptr, 0);
        if(!size) {
            return std::unexpected<int>(GetLastError());
        }
        std::vector<wchar_t> output(size, '\0');
        if(!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), -1, output.data(), output.size())) {
            return std::unexpected<int>(GetLastError());
        }
        return std::wstring(output.begin(), output.end()-1);
    };

    const std::expected<std::string, int> UTF16To8(const std::wstring& str) noexcept {
        if(str.empty()) {
            return "";
        }
        const int size = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if(!size) {
            return std::unexpected<int>(GetLastError());
        }
        std::vector<char> output(size, '\0');
        if(!WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, output.data(), output.size(), nullptr, nullptr)) {
            return std::unexpected<int>(GetLastError());
        }
        return std::string(output.begin(), output.end()-1);
    };

    template <class char_type>
    const std::expected<std::string, int> ToUTF8(const std::basic_string<char_type>& str) noexcept {
        static_assert(std::is_same_v<char_type, char> || std::is_same_v<char_type, wchar_t>);

        if constexpr (std::is_same_v<char_type, wchar_t>) {
            return UTF16To8(str);
        } else {
            return ACPToUTF16(str)
                    .and_then(&UTF16To8);
        }
    }
}

namespace CompScanMap {
    const std::optional<std::string> WindowsScancodeName(const Scancode code) noexcept {
        for(const auto& pair: WinScancodeNames) {
            if(pair.first == code) {
                return pair.second;
            }
        }

        return std::nullopt;
    }

    const std::expected<std::string, int> KeyboardKeyName(const Scancode code) noexcept {
        constexpr size_t keyNameLength = 128;
        TCHAR keyName[keyNameLength]="";

        if (code == 0x0000) {
            return "Null";
        }
        
        const int NameSize = GetKeyNameText(
            MakeLParam(code), keyName, keyNameLength
        );

        // キー名がある
        if (NameSize > 0){
            return ToUTF8<TCHAR>(keyName);
        }

        // キー名がない
        if (GetLastError() == ERROR_SUCCESS) {
            return std::string("");
        }

        // それ以外
        return std::unexpected<int>(GetLastError());
    }
}