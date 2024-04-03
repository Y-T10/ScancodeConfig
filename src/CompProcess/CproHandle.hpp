#pragma once

#include <memory>

#include "CregWinCommonHeader.hpp"

namespace CmpProc {
    struct object_deleter {
        void operator()(HANDLE h) noexcept {
            CloseHandle(h);
        }
    };

    /// WIN32ZAPIのオブジェクトハンドルをラップした型
    using object_handle = std::unique_ptr<
        std::remove_pointer_t<HANDLE>,
        object_deleter
    >;
}