#include <cstdio>
#include <expected>
#include <format>
#include <print>
#include <vector>

#include "CregHandler.hpp"
#include "CsmCodec.hpp"
#include "utf8proc.h"

std::expected<std::string, DWORD> key_name(
    const CompScanMap::Scancode code) noexcept;
std::string utf16_to_utf8(const std::wstring& str) noexcept;
void print_scancode_map() noexcept;
const size_t calc_string_width(const std::string& str) noexcept;

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        print_scancode_map();
        return 0;
    }

    return 0;
}

void print_scancode_map() noexcept {
    using namespace CompScanMap;
    using namespace CompReg;

    constexpr auto EntryName = TEXT("Scancode Map");
    constexpr auto KeyPath =
        TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout");

    const auto Handle =
        OpenRegKey(HKEY_LOCAL_MACHINE, KeyPath, KEY_QUERY_VALUE);
    if (!Handle) {
        std::println(stderr, "failed to open registry key");
        return;
    }

    const auto Keymapping = ReadKeyValueBin(Handle, EntryName);
    if (!Keymapping) {
        std::println(stderr, "failed to read entry value");
        return;
    }

    const auto CurrentMap = DecodeScancodeMap(*Keymapping);
    if (!CurrentMap) {
        std::println(stderr, "failed to decode binary");
        return;
    }

    const auto PrintError = [](const DWORD& v) {
        return std::format("error:{:#08x}", v);
    };

    std::println("{:<.20s}    {:<.20s}", "keyboard input", "conversion result");
    std::println("{:-<44.44s}", "");
    for (const auto& pair : *CurrentMap) {
        const auto FromName = key_name(pair.from).transform_error(PrintError);
        const auto ToName = key_name(pair.to).transform_error(PrintError);
        std::println("{:<#04x} ({:<15.15s}) -> {:<#04x} ({:<15.15s})",
                     pair.from, FromName ? FromName.value() : FromName.error(),
                     pair.to, ToName ? ToName.value() : ToName.error());
    }
}

std::string utf16_to_utf8(const std::vector<wchar_t>& str) noexcept {
    if (str.empty()) {
        return "";
    }
    const int Size = WideCharToMultiByte(CP_UTF8, 0, str.data(), str.size(),
                                         nullptr, 0, nullptr, nullptr);
    if (Size == 0) {
        return "";
    }
    std::vector<char> output(Size, '\0');
    if (0 == WideCharToMultiByte(CP_UTF8, 0, str.data(), str.size(),
                                 output.data(), output.size() * sizeof(wchar_t),
                                 nullptr, nullptr)) {
        return "";
    }
    return std::string(output.begin(), output.end() - 1);
};

std::expected<std::string, DWORD> key_name(
    const CompScanMap::Scancode code) noexcept {
    constexpr auto NoumNamelessKey = "no name";

    if (code == 0x0000) {
        return "null";
    }

    const LONG Value =
        ((code & 0xff00) != 0 ? 0x1000000 : 0) | (0xff0000 & (code << 16));
    auto buffer = std::vector<wchar_t>(128, L'\0');
    const int NameLength = GetKeyNameTextW(Value, buffer.data(), buffer.size());
    if (NameLength == 0) {
        const auto Code = GetLastError();
        if (Code == ERROR_SUCCESS) {
            return NoumNamelessKey;
        }
        return std::unexpected{Code};
    }
    buffer.resize(NameLength);
    buffer.emplace_back(L'\0');
    if (0 == GetKeyNameTextW(Value, buffer.data(), buffer.size())) {
        const auto Code = GetLastError();
        if (Code == ERROR_SUCCESS) {
            return NoumNamelessKey;
        }
        return std::unexpected{Code};
    }

    return utf16_to_utf8(buffer);
}

const size_t calc_string_width(const std::string& str) noexcept {
    size_t stringWidth = 0;
    for (utf8proc_int32_t offset = 0; (size_t)(offset) < str.length();) {
        utf8proc_int32_t codepoint = 0;
        const auto Advance =
            utf8proc_iterate((utf8proc_uint8_t*)(str.c_str() + offset),
                             str.length() + 1 - offset, &codepoint);
        if (Advance < 0) {
            return 0;
        }
        offset += Advance;
        const auto CharWith = utf8proc_charwidth(codepoint);
        if (CharWith < 0) {
            return 0;
        }
        stringWidth += CharWith;
    }
    return stringWidth;
}