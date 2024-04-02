#pragma once

#include "CregTypes.hpp"
#include <memory>
#include <type_traits>

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
}