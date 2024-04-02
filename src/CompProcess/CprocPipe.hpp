#pragma once

#include "CregTypes.hpp"
#include "CproHandle.hpp"

#include <expected>

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
}