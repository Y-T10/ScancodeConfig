#pragma once

#include "CregTypes.hpp"

#include <optional>
#include <cstdint>
#include <memory>
#include <vector>

#ifdef _WIN32
#include <winreg.h>
#endif

namespace CompReg {
#ifdef _WIN32
    /// HKEYはポインタ型
    static_assert(std::is_pointer_v<HKEY>);
    /// HKEYのデリータ
    struct HKEY_deleter {
        void operator()(HKEY ptr) {
            RegCloseKey(ptr);
        }
    };
    /// HKEYのユニークポインタ
    using RegKey = std::unique_ptr<std::remove_pointer_t<HKEY>, HKEY_deleter>;

    /**
     * @brief レジストリのキーを開く
     * @param rootKey 始点となるキー．
     * @param keyPath rootKey下のキーを指すパス
     * @param access キーを扱う際の権限
     * @return 成功すればキーのハンドラを指すポインタ、失敗すればnullptrを返す．
     */
    const RegKey OpenRegKey(const HKEY rootKey, const win32str& keyPath, const REGSAM access) noexcept;

    /**
     * @brief キーが持つバイナリ値を読み込む
     * @param key キー
     * @param valueName 値の名前 
     * @return 読み取りに成功すればバイナリ値を、失敗すればnulloptを返す．
     */
    const std::optional<std::vector<uint8_t>> ReadKeyValueBin(const RegKey& key, const win32str& valueName) noexcept;

    /**
     * @brief キーが持つバイナリ値を読み込む
     * @param key キー
     * @param valueName 値の名前 
     * @param value 書き込む値
     * @return 読み取りに成功すればバイナリ値を、失敗すればnulloptを返す．
     */
    const bool WriteKeyValueBin(const RegKey& key, const win32str& valueName, const std::vector<uint8_t>& value) noexcept;
#endif
};