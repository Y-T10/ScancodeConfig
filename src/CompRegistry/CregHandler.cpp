#include "CregHandler.hpp"

#include <optional>
#include <cstdint>

using namespace CompReg;

namespace {
    const std::optional<size_t> GetRegValueSize(const RegKey& hanlder, const win32str& name) noexcept {
        DWORD binary_size = 0;
        const auto result = RegQueryValueEx(
            hanlder.get(), name.c_str(),
            NULL, NULL, NULL, &binary_size);
        
        return result == ERROR_SUCCESS ?
            std::make_optional(binary_size):
            std::nullopt;
    };
}

namespace CompReg {
    const RegKey OpenHKLMKey(const HKEY rootKey, const win32str& keyPath, const REGSAM access) noexcept {
        HKEY hkey = nullptr;
        // 読み取り専用のレジストリハンドラを得る
        const LSTATUS result = RegOpenKeyEx(
            rootKey, keyPath.c_str(),
            0, access, &hkey);
        // ハンドラの取得に成功すればアドレスを返す
        if (result == ERROR_SUCCESS) {
            return RegKey(hkey);
        }
        return nullptr;
    }

    const std::optional<std::vector<uint8_t>> ReadKeyValueBin(const RegKey& key, const win32str& valueName) noexcept {
        const auto binary_size = GetRegValueSize(key, valueName);
        if(!binary_size) {
            return std::nullopt;
        }
        // バイナリデータを得る
        std::vector<BYTE> buff = std::vector<uint8_t>(*binary_size);
        const LSTATUS result = RegQueryValueEx(
            key.get(), valueName.c_str(),
            NULL, NULL, buff.data(), NULL);
        return result == ERROR_SUCCESS?
            std::make_optional(buff):
            std::nullopt;
    }
}