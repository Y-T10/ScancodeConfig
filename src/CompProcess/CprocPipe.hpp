#pragma once

#include "CregTypes.hpp"
#include "CproHandle.hpp"

#include <cstdint>
#include <expected>
#include <vector>

namespace CmpProc {
    /**
     * @brief パイプの名前を生成する
     * @return パイプ名を持つ文字列
     */
    const CompReg::win32str CreateRandomPipeName() noexcept;

    /**
     * @brief 名前付きパイプを作成する
     * @param name パイプ名
     * @param bufferSize パイプのバッファサイズ
     * @return 成功したらパイプのハンドルを、失敗したらエラーコードを返す．
     */
    const std::expected<object_handle, DWORD> CreatePipe(const CompReg::win32str& name, const size_t bufferSize) noexcept;

    /**
     * @brief パイプにデータを書き込む
     * 
     * @param handle パイプのハンドル
     * @param byte 書き込むデータ
     * @return 失敗した時のみエラーコードを返す
     */
    const std::expected<void, DWORD> WritePipe(const object_handle& handle, const std::vector<uint8_t>& byte) noexcept;

    /**
     * @brief パイプからデータを読み込む
     * @param handle パイプのハンドル
     * @return 失敗したらエラーコードを、成功したらバイナリを返す．
     */
    const std::expected<std::vector<uint8_t>, DWORD> ReadPipe(const object_handle& handle) noexcept;
}