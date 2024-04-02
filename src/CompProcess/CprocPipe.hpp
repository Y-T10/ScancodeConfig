#pragma once

#include "CregTypes.hpp"
#include <memory>
#include <type_traits>
#include <expected>

namespace CmpProc {
    struct pipe_deleter {
        void operator()(HANDLE h) noexcept {
            CloseHandle(h);
        }
    };

    /// パイプのハンドラをラップした型
    using PipeHandle = std::unique_ptr<
        std::remove_pointer_t<HANDLE>,
        pipe_deleter
    >;

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
    const std::expected<PipeHandle, DWORD> CreatePipe(const CompReg::win32str& name, const size_t bufferSize) noexcept;
}